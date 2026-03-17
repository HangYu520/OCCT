# GEMINI.md - Project Context

## Project Overview
This is a C++ project focused on learning and experimenting with **OpenCASCADE (OCCT)**. It provides a foundational setup to integrate OpenCASCADE libraries and build an executable that leverages them for 3D visualization and file operations.

### Key Technologies
*   **Language:** C++ (Standard: C++20)
*   **Build System:** CMake (Minimum version: 3.10)
*   **Primary Dependencies:** 
    *   **OpenCASCADE (OCCT):** Geometric modeling and STEP/BREP file I/O.
    *   **VTK:** 3D rendering and interactive visualization.

### Architecture
*   `CMakeLists.txt`: Root configuration.
*   `src/`: Core logic and encapsulated components.
    *   `src/Viewer.hpp/cpp`: `Viewer` class for interactive 3D visualization using VTK. Features a 2D message log overlay.
    *   `src/IO.hpp/cpp`: `IO` namespace for reading/writing `.brep` and `.stp/.step` files using OCCT's `BRepTools` and `STEPControl`.
*   `app/`: Main application.
    *   `app/main.cpp`: Entry point that supports command-line arguments and utilizes the `Viewer` and `IO` modules.
*   `.vscode/`: VS Code configuration for C++ development and debugging.

## Building and Running

### Prerequisites
*   A C++20 compatible compiler.
*   CMake (>= 3.10).
*   **OpenCASCADE & VTK:** Must be installed. `CMakeLists.txt` uses a specific `OpenCASCADE_DIR` path that may need local adjustment.

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make
```

### Running the Application
```bash
# Display default box
./bin/OCCT_MAIN

# Load a specific shape
./bin/OCCT_MAIN -i path/to/model.stp
```

## Interactive Features (Viewer)
When the visualization window is active, the following shortcuts are available:
*   **C:** Randomly change the color of the displayed shape.
*   **Ctrl + S:** Save the current shape as `save.stp` in the current working directory.
*   **R:** Reset camera to focus on the shape (VTK default).
*   **W/S:** Switch between Wireframe and Surface modes (VTK default).
*   **Message Log:** Real-time feedback and logs are displayed at the bottom-left of the window.

## Development Conventions
*   **C++ Standard:** C++20 is strictly enforced.
*   **Encapsulation:** Keep visualization and I/O logic in `src/` to maintain a clean `main.cpp`.
*   **Modern OCCT:** Prefer standard `bool` over deprecated `Standard_Boolean`.
*   **VTK Standards:** Use `AddViewProp()` instead of deprecated `AddActor2D()` for overlay actors.
