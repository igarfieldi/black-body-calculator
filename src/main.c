#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "cie_xyz.h"
#include "units.h"

// Takes a wavelength and temperature and computes the black-body radiation
// per unit time, area, and solid angle perpendicular to the surface.
SpectralRadiance compute_black_body_sample(const Nanometer lambda, const Kelvin T) {
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
void compute_black_body_samples(const Nanometer start, const Nanometer end,
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
CieXyz spectrum_to_xyz(SpectralRadiance spectralRadiance[static CIE_XYZ_SAMPLES]) {
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

int main(int argc, char* argv[argc + 1]) {
	if(argc < 2) {
		if(argc > 0)
			fprintf(stderr, "Usage: %s [temperature in Kelvin]\n"
							"The program computes the XYZ/RGB representation of a black-body spectrum defined by the given temperature (in Kelvin).",
					argv[0]);
		else
			fprintf(stderr, "Program is not meant to be executed in free-standing environment\n");
		return 2;
	}
	
	char* err = NULL;
	const Kelvin temperature = { strtod(argv[1], &err) };
	if(err == NULL || *err != '\0') {
		fprintf(stderr, "Error: could not convert temperature '%s' to double\n", argv[1]);
		return EXIT_FAILURE;
	}
	if(temperature.value <= 0.0) {
		fprintf(stderr, "Error: temperature must be in range (0, inf)\n");
		return EXIT_FAILURE;
	}
	
	const Nanometer start = CIE_XYZ_LAMBDA_START;
	const Nanometer end = CIE_XYZ_LAMBDA_END;
	const size_t samples = CIE_XYZ_SAMPLES;
	SpectralRadiance spectralRadiance[samples];
	compute_black_body_samples(start, end, samples, temperature, spectralRadiance);
	
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
		   temperature.value,
		   xyz.x, xyz.y, xyz.z,
		   rgb.r, rgb.g, rgb.b,
		   normRgb.r, normRgb.g, normRgb.b);
	
	return EXIT_SUCCESS;
}