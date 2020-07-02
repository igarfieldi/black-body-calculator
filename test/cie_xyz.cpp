#include <gtest/gtest.h>
#include "cie_xyz.h"
#include <cmath>

// Check with reasonable precision (4 digits)
static const double precision = 0.0001;

TEST(cie_xyz_to_rgb, converts_zero) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.0, 0.0, 0.0 });
	EXPECT_NEAR(value.r, 0.0, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.0, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.0, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb, converts_one) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 1.0, 1.0, 1.0 });
	EXPECT_NEAR(value.r, 1.20478, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.9483, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.90884, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb_test, converts_half) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.5, 0.5, 0.5 });
	EXPECT_NEAR(value.r, 0.60239, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.47415, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.45442, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb, converts_x_only) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 1.0, 0.0, 0.0 });
	EXPECT_NEAR(value.r, 3.240454, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, -0.969266, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.055643, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb, converts_y_only) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.0, 1.0, 0.0 });
	EXPECT_NEAR(value.r, -1.53714, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 1.87601, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, -0.20403, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb, converts_z_only) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.0, 0.0, 1.0 });
	EXPECT_NEAR(value.r, -0.498531, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.041556, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 1.057225, precision) << "Failed .b test";
}

TEST(cie_spectrum_to_xyz, triangle_spectrum) {
	// Create a fictive triangle-like spectrum for testing
	SpectralRadiance spectrum[CIE_XYZ_SAMPLES];
	const double normalizer = static_cast<double>(CIE_XYZ_SAMPLES / 2u);
	for(std::size_t i = 0u; i < CIE_XYZ_SAMPLES; ++i) {
		const auto radiance = 1.f - std::abs(static_cast<double>(i) - normalizer) / normalizer;
		spectrum[i] = SpectralRadiance{ radiance };
	}

	const auto xyz = cie_spectrum_to_xyz(spectrum);
	EXPECT_NEAR(xyz.x, 0.765455, precision);
	EXPECT_NEAR(xyz.y, 0.773947, precision);
	EXPECT_NEAR(xyz.z, 0.382540, precision);
}