#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cie_xyz.h"
#include "units.h"

// Takes a wavelength and temperature and computes the black-body radiation
// per unit time, area, and solid angle perpendicular to the surface.
static SpectralRadiance compute_black_body_sample(const Nanometer lambda, const Kelvin T) {
	// Planck's law: B_v(v, T) = 2hv³/(c²e^(hv/(kT)) - 1)
	// Given wavelengths instead of frequencies this leads to
	// S_λ = 2*h*c²/(λ^5 * e^(h*c/(λ*k*T)) - 1)
	//, which computes the amount of energy rediated per unit time and unit area into a unit of solid angle perpendicular to the surface.
	const double ePart = exp(PLANCK * SPEED_OF_LIGHT / (lambda.value * BOLTZMANN * T.value) * 1.0e6);
	const double nominator = 2.0 * PLANCK * SPEED_OF_LIGHT * SPEED_OF_LIGHT;
	const double denominator = (lambda.value*lambda.value)*(lambda.value*lambda.value)*lambda.value * (ePart - 1.0);
	const SpectralRadiance result = { nominator * 1.0e27 / denominator };
	return result;
}

// Computes N samples of black-body radiation between two given wavelengths
static void compute_black_body_samples(const Nanometer start, const Nanometer end,
									   const size_t samples, const Kelvin temperature,
									   SpectralRadiance spectralRadiance[static samples]) {
	assert(start.value < end.value);
	
	for(size_t i = 0u; i < samples; ++i) {
		const Nanometer lambda = { start.value + (end.value - start.value) * (double)i / (double)(samples - 1) };
		spectralRadiance[i] = compute_black_body_sample(lambda, temperature);
	}
}

// Converts a spectrum into XYZ color space.
// This method assumes that the spectral radiances are samples from 380 to 830nm in 1nm increments
static CieXyz spectrum_to_xyz(SpectralRadiance spectralRadiance[static CIE_XYZ_SAMPLES]) {
	CieXyz xyz = { 0.0, 0.0, 0.0 };
	for(size_t i = 0u; i < CIE_XYZ_SAMPLES; ++i) {
		xyz.x += CIE_X[i] * spectralRadiance[i].value;
		xyz.y += CIE_Y[i] * spectralRadiance[i].value;
		xyz.z += CIE_Z[i] * spectralRadiance[i].value;
	}
	const double scale = (double)(CIE_XYZ_LAMBDA_END.value - CIE_XYZ_LAMBDA_START.value) / (CIE_Y_INTEGRAL * (double)CIE_XYZ_SAMPLES);
	xyz.x *= scale;
	xyz.y *= scale;
	xyz.z *= scale;
	
	return xyz;
}

typedef struct CmdParameters {
	Kelvin temperature;
	Nanometer start;
	Nanometer end;
	size_t samples;
	bool printSamples;
	bool printNormalizedSamlples;
	const char* error;
} CmdParameters;

static CmdParameters parse_cmd_options(int argc, char* argv[static argc + 1]) {
	CmdParameters params = {
		.printSamples = false,
		.printNormalizedSamlples = false,
		.start = CIE_XYZ_LAMBDA_START,
		.end = CIE_XYZ_LAMBDA_END,
		.samples = CIE_XYZ_SAMPLES,
		.error = NULL
	};
	
	char* err = NULL;
	const Kelvin temperature = { strtod(argv[1], &err) };
	if(err == NULL || *err != '\0') {
		params.error = "could not convert temperature to double";
		return params;
	}
	if(temperature.value <= 0.0) {
		params.error = "temperature must be in range (0, inf)";
		return params;
	}

	// Parse the command parameters
	for(int i = 2; i < argc; ++i) {
		if(strncmp("--range", argv[i], 7) == 0) {
			// Parse the rest
			if(argc < i + 4) {
				params.error = "Error: missing option parameters for --range";
				break;
			}
			// TODO: parse
			params.start.value = strtod(argv[i + 1], &err);
			if(err == NULL) {
				params.error = "could not convert START to double";
				return params;
			}
			params.end.value = strtod(argv[i + 2], &err);
			if(err == NULL) {
				params.error = "could not convert END to double";
				return params;
			}
			params.samples = (size_t)strtol(argv[i + 3], &err, 10);
			if(err == NULL) {
				params.error = "could not convert SAMPLES to int";
				return params;
			}

			if(params.start.value < 0.0) {
				params.error = "START must not be < 0";
				return params;
			}
			if(params.end.value < 0.0) {
				params.error = "END must not be < 0";
				return params;
			}
			if(params.end.value < params.start.value) {
				params.error = "END must not be < START";
				return params;
			}
			if(params.samples <= 0) {
				params.error = "SAMPLES must not be <= 0";
				return params;
			}

			// Skip the parsed entries
			i += 3;
		} else if(strncmp("--print-samples", argv[i], 15) == 0) {
			params.printSamples = true;
		} else if(strncmp("--print-normalized-samples", argv[i], 26)) {
			params.printNormalizedSamlples = true;
		} else {
			printf("Warning: unrecognized option '%s'\n", argv[i]);
		}
	}

	return params;
}

int main(int argc, char* argv[static argc + 1]) {
	if(argc < 2) {
		if(argc > 0)
			fprintf(stderr, "Usage: %s [temperature in Kelvin] [Options]\n"
							"Options: --range START END N: replaces the standard range (380 to 830nm) with custom range and sample count\n"
							"         --print-samples: outputs the black-body samples to stdout\n"
							"         --print-normalized-samples: outputs the normalized black-body samples to stdout"
							"The program computes the XYZ/RGB representation of a black-body spectrum defined by the given temperature (in Kelvin).",
					argv[0]);
		else
			fprintf(stderr, "Program is not meant to be executed in free-standing environment\n");
		return 2;
	}

	// Command-line options
	CmdParameters params = parse_cmd_options(argc, argv);
	if(params.error != NULL) {
		fprintf(stderr, "Error: %s!\n", params.error);
		return 2;
	}

	SpectralRadiance* spectralRadiance = (SpectralRadiance*)malloc(sizeof(SpectralRadiance) * params.samples);


	// TODO: other sampling!
	compute_black_body_samples(params.start, params.end, params.samples, params.temperature, spectralRadiance);
	
	// Weight the samples with the XYZ response
	const CieXyz xyz = spectrum_to_xyz(spectralRadiance);
	const ColorRgb rgb = cie_xyz_to_rgb(xyz);
	
	const double normalizer = fmax(fmax(rgb.r, rgb.g), rgb.b);
	const ColorRgb normRgb = {
		.r = rgb.r / normalizer,
		.g = rgb.g / normalizer,
		.b = rgb.b / normalizer
	};
	
	for(size_t i = 0u; i < CIE_XYZ_SAMPLES; ++i)
		printf("%fnm: %f\n", CIE_XYZ_LAMBDA_START.value + (double)i, spectralRadiance[i].value);
	
	printf("Black-body color for %fK:\nXYZ:\t\t\t[ %f, %f, %f ]\nRGB:\t\t\t[ %f, %f, %f ]\nRGB(normalized):\t[ %f, %f, %f ]\n",
		   params.temperature.value,
		   xyz.x, xyz.y, xyz.z,
		   rgb.r, rgb.g, rgb.b,
		   normRgb.r, normRgb.g, normRgb.b);
	
	return EXIT_SUCCESS;
}