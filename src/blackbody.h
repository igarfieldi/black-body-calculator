#ifndef BLACKBODY_BLACKBODY_H_
#define BLACKBODY_BLACKBODY_H_

#include "units.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>

    /**
     * Takes a wavelength and temperature and computes the black-body radiation
     * per unit time, area, and solid angle perpendicular to the surface.
     */
    SpectralRadiance black_body_compute_sample(const Nanometer wavelength, const Kelvin T);

    // Computes N samples of black-body radiation between two given wavelengths
    void black_body_compute_samples(const Nanometer start, const Nanometer end,
                                    const size_t samples, const Kelvin temperature,
                                    SpectralRadiance spectralRadiance[static samples]);

    // Computes the peak wavelength and spectral radiance for the given temperature
    Nanometer black_body_compute_peak_wavelength(const Kelvin T);

#ifdef __cplusplus
} extern "C"
#endif // __cplusplus

#endif // BLACKBODY_BLACKBODY_H_