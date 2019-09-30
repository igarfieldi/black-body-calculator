#ifndef BLACKBODY_CIE_CONSTANTS_H_
#define BLACKBODY_CIE_CONSTANTS_H_

#include "units.h"

// Constants for the CIE XYZ sample data
static const Nanometer CIE_XYZ_LAMBDA_START = { 380 };
static const Nanometer CIE_XYZ_LAMBDA_END = { 830 };
static const size_t CIE_XYZ_SAMPLES = 471u;
static const double CIE_Y_INTEGRAL = 106.856895;
extern const double CIE_X[];
extern const double CIE_Y[];
extern const double CIE_Z[];

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

ColorRgb cie_xyz_to_rgb(const CieXyz);

#endif // BLACKBODY_CIE_CONSTANTS_H_