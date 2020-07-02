#include "blackbody.h"
#include <assert.h>
#include <math.h>

SpectralRadiance black_body_compute_sample(const Nanometer lambda, const Kelvin T) {
	if(lambda.value < 0.0 || T.value < 0.0) {
		const SpectralRadiance result = { 0.0 };
		return result;
	}

	// Planck's law: B_v(v, T) = 2hv³/(c²e^(hv/(kT)) - 1)
	// Given wavelengths instead of frequencies this leads to
	// S_λ = 2*h*c²/(λ^5 * e^(h*c/(λ*k*T)) - 1)
	//, which computes the amount of energy rediated per unit time and unit area into a unit of solid angle perpendicular to the surface.
	const double ePart = exp(PLANCK * SPEED_OF_LIGHT / (lambda.value * BOLTZMANN * T.value) * 1.0e6);
	const double nominator = 2.0 * PLANCK * SPEED_OF_LIGHT * SPEED_OF_LIGHT;
	const double denominator = (lambda.value * lambda.value) * (lambda.value * lambda.value) * lambda.value * (ePart - 1.0);
	const SpectralRadiance result = { nominator * 1.0e27 / denominator };
	return result;
}

void black_body_compute_samples(const Nanometer start, const Nanometer end,
								const size_t samples, const Kelvin temperature,
								SpectralRadiance spectralRadiance[static samples]) {
	if(start.value < 0.0 || end.value < 0.0 || start.value > end.value || temperature.value < 0.0)
		return;

	// We simply divide the sample domain into equally sized intervals
	// and compute the samples at the boundaries of these intervals
	for(size_t i = 0u; i < samples; ++i) {
		const Nanometer lambda = { start.value + (end.value - start.value) * (double)i / (double)(samples - 1) };
		spectralRadiance[i] = black_body_compute_sample(lambda, temperature);
	}
}

Nanometer black_body_compute_peak_wavelength(const Kelvin T) {
	if(T.value < 0.0) {
		const Nanometer result = { 0.0 };
		return result;
	}

	// The peak wavelength can be computed via Wien's displacement law as λ_peak = b / T,
	// with b being the displacement constant.

	// Don't forget the exponent to convert to nanometer
	const Nanometer peakWavelength = { 1.0e6 * WIEN_DISPLACEMENT / (T.value) };
	return peakWavelength;
}