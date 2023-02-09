1 Compilation
----------------------
WadTools uses CMake to generate the required project files. By default it uses Qt6
but there is a QT5 option provided for convenient setup. To generate a Qt5 build
you can set the variable during setup:

	cmake .. -DQT5=ON

Built binaries will be placed in a "bin" folder in the configured with CMake directory.
The Qt frontend (WadTools) is built by default as is the console application (wdtls).
To disable building of the frontend, set the GUI variable to OFF during configuration:

	cmake .. -DGUI=OFF

1.1 Compilation on Windows
----------------------------
Create a build directory in the repository root, enter it then configure with CMake:

	mkdir build
	cd build
	cmake .. -G "{Your VS version}" -DCMAKE_PREFIX_PATH="{Path to qt installation}"

To know which version to pass to -G you can consult:

	<https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators>

After the build is complete and you have a WadTools.exe file, assuming you have
the correct Qt directory registered in your PATH:

	windeployqt --release .\WadTools.exe

This will copy over the required Dlls.

1.2 Static Compilation on Windows
-----------------------------------
For a static windows build you need to link with a static build of qtbase.
The easiest way to do this is through vcpkg. The CMake flag MSVC_STATIC
is provided for convenient setup. The following is an example for x64 builds:

VCPKG:

	git clone https://github.com/Microsoft/vcpkg.git
	cd vcpkg
	.\bootstrap-vcpkg.bat
	.\vcpkg install qtbase:x64-windows-static

BUILD:
enter the WadTools repository root where you cloned the project

	mkdir build
	cd build
	cmake .. -G "Visual Studio 17 2022" \
	-A x64 \
	-DCMAKE_BUILD_TYPE=Release
	-DMSVC_STATIC=ON \
	-DCMAKE_TOOLCHAIN_FILE="{vcpkg directory from previous step}\scripts\buildsystems\vcpkg.cmake" \
	-DVCPKG_TARGET_TRIPLET=x64-windows-static

For 32bit builds you can set the CMake platform to Win32 with -A Win32.
The 32bit triplet is x86-windows-static.

1.3 Compilation on Linux
-----------------------
CMake by default will generate the necessary makefile:

	mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc --all)
