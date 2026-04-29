# JS Render Playground

A high-performance experiment in embedding the **QuickJS** engine within a **C++ / SDL3** environment. This application demonstrates a bi-directional bridge where a TypeScript-based simulation drives hardware-accelerated rendering.

This is a test application designed to explore:

- **QuickJS Embedding:** Running a lightweight JS runtime inside C++.
- **FFI (Foreign Function Interface):** Exposing C++ functions (`nativeDrawRect`, `nativeDrawText`) to JavaScript.
- **Event Bridging:** Passing SDL3 hardware events (input, window dimensions) into the JS global scope.
- **Physics in JS:** Running a particle simulation with gravity, collision, and bounce logic in TypeScript.

## Prerequisites

Before building, ensure you have the following installed:

- **CMake** (3.16+)
- **Clang/GCC** (C++17 support)
- **SDL3** & **SDL3_ttf** (via Homebrew: `brew install sdl3 sdl3_ttf`)
- **Node.js & npm** (for TypeScript compilation)

## Building the Project

### 1. Initialize Dependencies

This project uses Git submodules for the QuickJS engine.
`git submodule update --init --recursive`

### 2. Build the JavaScript Bundle

The C++ engine executes a transpiled JavaScript bundle. You must build the JS side before running the executable.
`npm install`
`npm run build`

_This generates `dist/bundle.js` from your TypeScript source files._

### 3. Build the C++ Engine

`mkdir build`
`cd build`
`cmake ..`
`make`

## Running the App

The application must be executed from the **project root** directory so it can correctly locate the `assets/` and `dist/` folders:

`./build/MyApp`

**Controls & Logic:**

- **Input:** Key events are passed from SDL3 into the `globalThis.onKeyPress` handler.
- **Physics:** Squares spawn and fall, shattering into bouncing fragments upon hitting a "virtual floor" calculated from the C++ window height.
- **Performance:** Real-time particle counts are rendered via the `nativeDrawText` bridge.

## Project Structure

- `/src`: C++ source code, bridge logic, and TypeScript source.
- `/deps`: Third-party dependencies (QuickJS via git submodule).
- `/assets`: Media files, specifically `font.ttf` required for text rendering.
- `/dist`: The final bundled JavaScript executed by the engine.
