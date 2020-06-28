#include <gtest/gtest.h>
#include "cie_xyz.h"
#include <cmath>

// Check with reasonable precision (4 digits)
static const double precision = 0.0001;

TEST(cie_xyz_to_rgb_test, ConvertsZero) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.0, 0.0, 0.0 });
	EXPECT_NEAR(value.r, 0.0, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.0, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.0, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb_test, ConvertsOne) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 1.0, 1.0, 1.0 });
	EXPECT_NEAR(value.r, 1.20478, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.9483, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.90884, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb_test, ConvertsHalf) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.5, 0.5, 0.5 });
	EXPECT_NEAR(value.r, 0.60239, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.47415, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.45442, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb_test, ConvertsXOnly) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 1.0, 0.0, 0.0 });
	EXPECT_NEAR(value.r, 3.240454, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, -0.969266, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 0.055643, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb_test, ConvertsYOnly) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.0, 1.0, 0.0 });
	EXPECT_NEAR(value.r, -1.53714, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 1.87601, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, -0.20403, precision) << "Failed .b test";
}

TEST(cie_xyz_to_rgb_test, ConvertsZOnly) {
	const ColorRgb value = cie_xyz_to_rgb(CieXyz{ 0.0, 0.0, 1.0 });
	EXPECT_NEAR(value.r, -0.498531, precision) << "Failed .r test";
	EXPECT_NEAR(value.g, 0.041556, precision) << "Failed .g test";
	EXPECT_NEAR(value.b, 1.057225, precision) << "Failed .b test";
}