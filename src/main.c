#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blackbody.h"
#include "cie_xyz.h"
#include "units.h"
#include "util.h"

typedef struct CmdParameters {
	Kelvin temperature;
	Nanometer start;
	Nanometer end;
	size_t samples;
	bool printSamples;
	bool printNormalizedSamlples;
	const char* error;
} CmdParameters;

static CmdParameters parse_cmd_options(int argc, char* argv[STATIC_SIZE(argc + 1)]) {
	CmdParameters params = {
		.printSamples = false,
		.printNormalizedSamlples = false,
		.start = CIE_XYZ_LAMBDA_START,
		.end = CIE_XYZ_LAMBDA_END,
		.samples = CIE_XYZ_SAMPLES,
		.error = NULL
	};
	
	char* err = NULL;
	params.temperature.value = strtod(argv[1], &err);
	if(err == NULL || *err != '\0') {
		params.error = "could not convert temperature to double";
		return params;
	}
	if(params.temperature.value <= 0.0) {
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

int main(int argc, char* argv[STATIC_SIZE(argc + 1)]) {
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
	black_body_compute_samples(params.start, params.end, params.samples, params.temperature, spectralRadiance);
	
	// Weight the samples with the XYZ response
	const CieXyz xyz = cie_spectrum_to_xyz(spectralRadiance);
	const ColorRgb rgb = cie_xyz_to_rgb(xyz);
	
	const double normalizer = fmax(fmax(rgb.r, rgb.g), rgb.b);
	const ColorRgb normRgb = {
		.r = rgb.r / normalizer,
		.g = rgb.g / normalizer,
		.b = rgb.b / normalizer
	};
	
	if(params.printSamples) {
		for(size_t i = 0u; i < CIE_XYZ_SAMPLES; ++i)
			printf("%fnm: %f\n", CIE_XYZ_LAMBDA_START.value + (double)i, spectralRadiance[i].value);
	}
	if(params.printNormalizedSamlples) {
		// Compute normalization factor
		double max = 0.0;
		for(size_t i = 0u; i < CIE_XYZ_SAMPLES; ++i)
			max = fmax(max, spectralRadiance[i].value);

		for(size_t i = 0u; i < CIE_XYZ_SAMPLES; ++i)
			printf("%fnm: %f\n", CIE_XYZ_LAMBDA_START.value + (double)i, spectralRadiance[i].value / max);
	}
	
	printf("Black-body color for %fK:\nXYZ:\t\t\t[ %f, %f, %f ]\nRGB:\t\t\t[ %f, %f, %f ]\nRGB(normalized):\t[ %f, %f, %f ]\n",
		   params.temperature.value,
		   xyz.x, xyz.y, xyz.z,
		   rgb.r, rgb.g, rgb.b,
		   normRgb.r, normRgb.g, normRgb.b);
	
	return EXIT_SUCCESS;
}