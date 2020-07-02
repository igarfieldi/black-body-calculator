#include <gtest/gtest.h>
#include "blackbody.h"
#include <cmath>

// Check with reasonable precision (4 digits)
static const double precision = 0.0001;

TEST(compute_black_body_sample, zero_temperature) {
	const Kelvin temp{ 0.0 };
	EXPECT_EQ(black_body_compute_sample(Nanometer{ 300.0 }, temp).value, 0.0);
	EXPECT_EQ(black_body_compute_sample(Nanometer{ 400.0 }, temp).value, 0.0);
	EXPECT_EQ(black_body_compute_sample(Nanometer{ 500.0 }, temp).value, 0.0);
	EXPECT_EQ(black_body_compute_sample(Nanometer{ 600.0 }, temp).value, 0.0);
	EXPECT_EQ(black_body_compute_sample(Nanometer{ 700.0 }, temp).value, 0.0);
	EXPECT_EQ(black_body_compute_sample(Nanometer{ 800.0 }, temp).value, 0.0);
	EXPECT_EQ(black_body_compute_sample(Nanometer{ 900.0 }, temp).value, 0.0);
}

TEST(compute_black_body_sample, candle_temperature) {
	// Candle color temp. is ~1500K
	// In combination with the peak wavelength test this should be representative
	const Kelvin temp{ 1500.0 };
	EXPECT_NEAR(black_body_compute_sample(Nanometer{ 300.0 }, temp).value, 637.82579, precision);
	EXPECT_NEAR(black_body_compute_sample(Nanometer{ 400.0 }, temp).value, 448139.4372896, precision);
	EXPECT_NEAR(black_body_compute_sample(Nanometer{ 500.0 }, temp).value, 17770758.437226, precision);
	EXPECT_NEAR(black_body_compute_sample(Nanometer{ 600.0 }, temp).value, 174727671.235562, precision);
	EXPECT_NEAR(black_body_compute_sample(Nanometer{ 700.0 }, temp).value, 793337734.117138, precision);
	EXPECT_NEAR(black_body_compute_sample(Nanometer{ 800.0 }, temp).value, 2256176320.533655, precision);
	EXPECT_NEAR(black_body_compute_sample(Nanometer{ 900.0 }, temp).value, 4744444883.371235, precision);
}

TEST(compute_black_body_sample, peak) {
	// Compute the spectral radiances for the peak wavelengths computed below
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 1500.0 }),
										  Kelvin{ 1500.0 }).value, 31101529569.608772, precision);
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 2000 }),
										  Kelvin{ 2000.0 }).value, 131061589626.66422, precision);
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 2600.0 }),
										  Kelvin{ 2600.0 }).value, 486622507972.53033, precision);
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 3000.0 }),
										  Kelvin{ 3000.0 }).value, 995248946227.48071, precision);
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 4000.0 }),
										  Kelvin{ 4000.0 }).value, 4193970868053.2549, precision);
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 5000.0 }),
										  Kelvin{ 5000.0 }).value, 12798983361978.928, precision);
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 6500.0 }),
										  Kelvin{ 6500.0 }).value, 47521729294192.414, precision);
	EXPECT_NEAR(black_body_compute_sample(black_body_compute_peak_wavelength(Kelvin{ 10000.0 }),
										  Kelvin{ 10000.0 }).value, 409567467583325.69, precision);
}

TEST(black_body_compute_peak_wavelength, samples) {
	//The chosen temperatures are candle, sodium-vapor lamp, 40W light bulb, 200W light bulb,
	// fluorescend lamp, D50 lamp, D65 (daylight), and some blue sky.

	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ -500.0 }).value, 0.0, precision);

	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 1500.0 }).value, 1931.847970, precision);
	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 2000.0 }).value, 1448.885978, precision);
	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 2600.0 }).value, 1114.527675, precision);
	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 3000.0 }).value, 965.923985, precision);
	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 4000.0 }).value, 724.442989, precision);
	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 5000.0 }).value, 579.554391, precision);
	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 6500.0 }).value, 445.811070, precision);
	EXPECT_NEAR(black_body_compute_peak_wavelength(Kelvin{ 10000.0 }).value, 289.777196, precision);
}

TEST(black_body_compute_samples, range_test) {
	SpectralRadiance samples[1024u];
	memset(samples, 0, sizeof(samples));

	// Check corner cases
	// Negative values
	black_body_compute_samples(Nanometer{ -100.0 }, Nanometer{ 50.0 }, 1u, Kelvin{ 6500.0 }, samples);
	EXPECT_EQ(samples[0].value, 0.0);
	black_body_compute_samples(Nanometer{ -100.0 }, Nanometer{ -50.0 }, 1u, Kelvin{ 6500.0 }, samples);
	EXPECT_EQ(samples[0].value, 0.0);
	black_body_compute_samples(Nanometer{ 100.0 }, Nanometer{ 500.0 }, 1u, Kelvin{ -6500.0 }, samples);
	EXPECT_EQ(samples[0].value, 0.0);
	// Inverted range
	black_body_compute_samples(Nanometer{ 100.0 }, Nanometer{ 50.0 }, 1u, Kelvin{ 6500.0 }, samples);
	EXPECT_EQ(samples[0].value, 0.0);
	// Zero samples
	black_body_compute_samples(Nanometer{ 100.0 }, Nanometer{ 500.0 }, 0u, Kelvin{ 6500.0 }, samples);
	EXPECT_EQ(samples[0].value, 0.0);

	// Daylight test (should be at 100nm, 300nm and 500nm)
	black_body_compute_samples(Nanometer{ 100.0 }, Nanometer{ 500.0 }, 3u, Kelvin{ 6500.0 }, samples);
	EXPECT_NEAR(samples[0u].value, 2902729253.085279, precision);
	EXPECT_NEAR(samples[1u].value, 30635070484501.422, precision);
	EXPECT_NEAR(samples[2u].value, 46097184673518.0, precision);
}