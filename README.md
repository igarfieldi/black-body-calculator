# black-body-calculator
Computes the black-body spectrum converted to XYZ and RGB for a given temperature.

Build tool is CMake, but the project is trivial enough to be quickly compiled with any compiler/toolchain (e.g. clang -o blackbody src/main.c src/cie_xyz.c).
It is currently not possible to specify arbitrary wavelengths for computation - the tool will always take the 471 samples between 380 and 830nm.
