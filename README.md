# COMP410 - HW #1 - Bouncing Objects

## Team

- Ameer Taweel (ataweel20)
- Omar Al Asaad (oasaad20)

## Dependencies

- GCC Compiler `v12.2.0`
- libGL `v1.6.0`
- GLFW `v3.3.8`
- GLEW `v2.2.0`
- GLM `v0.9.9.8`

**NOTE:** The code was tested only using the exact version of these
dependencies. It compiles with no errors or warnings. The code might not compile
or work as intended if you don't have the correct versions of all dependencies.

**NOTE:** The project was developed and tested on Linux. We tried to write the
code in a way that supports macOS, but we don't have access to a macOS device
and we don't guarantee that it works. Please evaluate our code on a Linux
device.

**NOTE:** We took permission from the professor to use GLM.

## Directory Structure

- The root directory contains the top-level code `main.cpp` and `3dview.cpp`.
- `custom` contains custom helper modules.
- `models` contains 3D models in TLST (custom) format. More on TLST later.
- `shaders` contains vertex and fragment shaders for `main.cpp` and `3dview.cpp`.
- `photos` contains photos of the program to demonstrate that it works properly.

## Creative Additions

- Supports the bunny model (along with cube and sphere).
- The code is model-agnostic. It can use other models without change in code.
- Created the TLST 3D model format which is a modified version of OFF which
  removes the redundant information.
- Simple 3D model viewer and editor `3dview`. We used it to rotate and scale the
  bunny model, and to scale the cube and sphere models. So scaling does not
  happen during runtime in `main`. This reduces the code complexity.

## Compilation

### `main`

Run this command from the root project directory:

```bash
g++ -lGL -lglfw -lGLEW -Wall -o main.out main.cpp
```

### `3dview`

Run this command from the root project directory:

```bash
g++ -lGL -lglfw -lGLEW -Wall -o 3dview.out 3dview.cpp
```

## Usage

### `main`

Run this command from the root project directory:

```bash
./main.out
```

**NOTE:** The command above assumes you did the compilation step.

### `3dview`

Run this command from the root project directory:

```bash
./3dview.out
```

**NOTE:** The command above assumes you did the compilation step.

You can change which model to load, and which transformations to apply from the
constants section in `3dview.cpp`.
