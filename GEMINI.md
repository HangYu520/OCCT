# GEMINI.md - Project Context

## Project Overview
This is a C++ project focused on learning and experimenting with **OpenCASCADE (OCCT)**. It provides a foundational setup to integrate OpenCASCADE libraries and build an executable that leverages them. The current primary application is a simple test that links against OCCT and prints its version information.

### Key Technologies
*   **Language:** C++ (Standard: C++20)
*   **Build System:** CMake (Minimum version: 3.10)
*   **Primary Dependency:** OpenCASCADE (OCCT)

### Architecture
*   `CMakeLists.txt`: The root CMake configuration file defining the project, compiler standards, output directories, and locating the OpenCASCADE dependency.
*   `app/`: Contains the main application source code.
    *   `app/main.cpp`: The entry point of the application, which includes OCCT headers to output version details.
    *   `app/CMakeLists.txt`: Configures the main executable target (`OCCT_MAIN`), setting up include directories and linking the necessary OpenCASCADE libraries.
*   `.vscode/`: Contains Visual Studio Code configuration files (`c_cpp_properties.json`, `launch.json`) for C/C++ development and debugging.

## Building and Running

### Prerequisites
*   A C++20 compatible compiler.
*   CMake (>= 3.10).
*   **OpenCASCADE:** Must be installed on your system. Note that `CMakeLists.txt` hardcodes an `OpenCASCADE_DIR` path (`/Users/hangyu/Documents/Mycode/OCCT/install/release/lib/cmake/opencascade`), which may need to be adjusted or overridden via CMake arguments depending on your local installation.

### Build Instructions
The project's `README.md` suggests building with MinGW Makefiles, but standard CMake commands can also be used.

**Standard CMake Build (Recommended for macOS/Linux):**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

**As per README (Targeting MinGW):**
```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
make
```

### Running the Application
After a successful build, the executable will be located in the `bin/` directory within your build folder.
```bash
./build/bin/OCCT_MAIN
```

## Development Conventions
*   **C++ Standard:** The project strictly enforces C++20 (`set(CMAKE_CXX_STANDARD_REQUIRED ON)`). Ensure any new code is C++20 compliant.
*   **CMake Build Types:** The default build type is set to `Debug` if not explicitly specified.
*   **Executable Target:** The main executable target is dynamically named based on the project name (`${PROJECT_NAME}_MAIN`, resulting in `OCCT_MAIN`).
*   **Linking:** The project links against OpenCASCADE libraries iterating over `${OpenCASCADE_LIBRARIES}`, distinguishing between `debug` and `optimized` (release) library paths.