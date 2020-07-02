#ifndef BLACKBODY_UNITS_H_
#define BLACKBODY_UNITS_H_

// Physical constants bar their exponents for increased numerical precision
static const double PLANCK = 6.62607015;				//e-34	[J*s]
static const double BOLTZMANN = 1.380649;				//e-23	[J/K]
static const double SPEED_OF_LIGHT = 2.99792458;		//e8	[m]
static const double WIEN_DISPLACEMENT = 2.897771955;	//e-3	[m*K]

// Strong typedef's for units to get type errors on mistake
typedef struct Nanometer {
	double value;	// [m]
} Nanometer;
typedef struct Kelvin {
	double value;	// [K]
} Kelvin;
typedef struct SpectralRadiance {
	double value;	// [W / (sr*m³)]
} SpectralRadiance;

#endif // BLACKBODY_UNITS_H_