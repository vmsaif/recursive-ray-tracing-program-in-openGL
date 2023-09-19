# Recursive Ray Tracing in OpenGL

[![Hits](https://hits.sh/github.com/vmsaif/recursive-ray-tracing-program-in-openGL.svg?label=Visits&color=100b75)](https://hits.sh/github.com/vmsaif/recursive-ray-tracing-program-in-openGL/)

This project delves into the world of computer graphics by implementing the recursive ray tracing algorithm in OpenGL. The user can choose between two programs: one that demonstrates basic recursive ray tracing and another that introduces shadows to the scene.

## Features

- **Recursive Ray Tracing**: Casts rays from the source to the scene, determining whether rays hit reflective or non-reflective surfaces.
- **Shadow Implementation**: Enhances the ray tracing algorithm by checking if intersection points can see the light source, introducing shadows where necessary.
- **User Interaction**: Allows users to select between two distinct ray tracing programs.

## Requirements

### This Repository

Clone this repository to your local machine with:

```bash
git clone https://github.com/vmsaif/recursive-ray-tracing-program-in-openGL
```

### Visual Studio

Ensure you have Visual Studio installed. If not, download it from [Visual Studio Official Website](https://visualstudio.microsoft.com/).

### GLUT Library

This project requires the OpenGL library or `glut.h`. The easiest way to set it up in Visual Studio is as follows:

1. Navigate to the directory where you cloned the project.
2. Open the `recursive-ray-tracing-program-in-openGL.sln` file in Visual Studio.
3. Click on `Project` in the menu bar.
4. Select `Manage NuGet Packages`.
5. Click on the "Browse" tab.
6. Search for "freeglut".
7. Select "nupengl.core" (which includes glut.h) and click `Install`.

## Running the Project

1. Open the project in Visual Studio.
2. Press `F5` or choose the `Run Without Debugging` option.
3. Follow the on-screen prompts to select and run a ray tracing program.

## Purpose

The primary goal of this project is to showcase the recursive ray tracing algorithm. Rays are cast from the source to the scene, determining if they hit walls (non-reflective surfaces) or floors (reflective surfaces). The algorithm either terminates upon hitting a wall, returning the wall's local color, or recursively invokes itself upon hitting the floor, calculating the mixed color. An additional feature tests intersections to determine if they can see the light source, introducing shadows where necessary.
