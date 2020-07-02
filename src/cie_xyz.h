#ifndef BLACKBODY_CIE_XYZ_H_
#define BLACKBODY_CIE_XYZ_H_

#include "units.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "util.h"
#include <stddef.h>

// Constants for the CIE XYZ sample data
static const Nanometer CIE_XYZ_LAMBDA_START = { 380 };
static const Nanometer CIE_XYZ_LAMBDA_END = { 830 };
static const double CIE_Y_INTEGRAL = 106.856895;
extern const double CIE_X[];
extern const double CIE_Y[];
extern const double CIE_Z[];
#define CIE_XYZ_SAMPLES 471llu

// Represents 
typedef struct CieXyz {
	double x;
	double y;
	double z;
} CieXyz;

// A simple RGB color struct
typedef struct ColorRgb {
	double r;
	double g;
	double b;
} ColorRgb;

// Takes a color in XYZ space (D65) and converts it to linear RGB (ITU-R BT.709 without gamma correction).
ColorRgb cie_xyz_to_rgb(const CieXyz);

/**
 * Converts a spectrum into XYZ color space.
 * This function assumes that the spectral radiances are samples from 380 to 830nm in 1nm increments
 * (see CIE_XYZ_LAMBDA_START and CIE_XYZ_LAMBDA_END).
 */
CieXyz cie_spectrum_to_xyz(SpectralRadiance spectralRadiance[STATIC_SIZE(CIE_XYZ_SAMPLES)]);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLACKBODY_CIE_XYZ_H_