# Ray Tracing Engine
> Xu Si , Tsinghua Univ.

This ray tracing engine is a rendering engine written in C++ using a path tracing algorithm to achieve high-quality image rendering. It supports a variety of rendering effects and functions, allowing for the creation of realistic scenes.

## Implemented Features

- Refraction, Reflection, and Shadows
- Parametric Surface Rendering
- Triangle Mesh Model Rendering
- Depth of Field
- Diffuse Reflection
- Soft Shadows
- Texture Mapping
- Anti-aliasing

## Rendering Acceleration

The engine employs the following rendering acceleration techniques to enhance rendering speed:

- Bounding Box Acceleration
- OpenMP Acceleration

## Execution Method

1. Compile the code using the following command:
   ```bash
   g++ main.cpp -o main -O3 -fopenmp -std=c++1z
   ```

2. Run the executable file:
   ```bash
   ./main <image width> <image height> <output path> <samples per pixel>
   ```
   (For example: `./main 1024 768 output/test.ppm 50`)

## Camera Position

In the `Main.cpp` file, you can set the camera's position and orientation:
```cpp
Ray camera(Vec3(150, 28, 260), Vec3(-0.45, 0.001, -1).norm()); // Camera settings
```

## Scene Setup

In the `Scene.hpp` file, you can choose different scene configurations:
```cpp
Object **scene = rabbit; // Available scenes: rabbit, vase, pink
int scene_num = 8; // rabbit: 8  vase: 11  pink: 12
```