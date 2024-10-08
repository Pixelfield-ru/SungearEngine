<img src="./documentation/images/mainLogo.png" width="100%">
<p align="center">🎮 An Open Source Engine Written in C++! 🎮</p>

# <p align="center"> Features </p>

-   **General:**
    - ECS
    - Audio (OpenAL)
    - Parallel thread-safety physics (colliders of different forms, rigidbodies)
    - Octree with automatic subdivision
    - Support input from keyboard and mouse
    - Annotations and code generation
        > More information about annotations, see the source code: [Annotation](https://github.com/Pixelfield-ru/SungearEngine/tree/main/Sources/SGCore/Annotations)  
        > Example annotation
        ```cpp
        sg_struct()
        struct MyStruct
        {
            sg_member()
            float m_myMember = 3.0f;
        }
        ```
    - A set of classes for parallel computing
      > More information about parallel computing, see the source code: [Parallel computing](https://github.com/Pixelfield-ru/SungearEngine/tree/main/Sources/SGCore/Threading)
    - Small developments in the in-game UI (loading TTF fonts, creating text)
-   **Graphics Rendering:**
    - Preparation for multi-GAPI (GAPI - graphics API) (using only abstractions and avoiding native API functions in external code (outside the implementation of abstract classes for different GAPI))
    - Аn additional add-on to the GLSL shader language for dividing shaders into render passes, specifying pass parameters, including shaders, as well as writing different shaders (fragment, vertex, geometric, etc.) in one file
    - PBR (Physically Based Rendering) using Cook-Torrance BRDF. Different maps are supporting:
        - Albedo Map
        - Normal Map
        - Roughness Map
        - Metallic Map
        - Ambient Occlusion Map
        - Emission Map
    - Shadows (Directional Light only)
    - Atmosphere Scattering
      
-   **Post Processing:**
    - Layered postprocessing (you can add different objects to different layers and apply different postprocessing to them, be it blur, bloom, etc.)

-   **Asset Pipeline:**
    - Plugin Systems(generating a plugin project, connecting plugins to the engine in runtime)
    - Load Textures and svg files
    - Load many model formats such as .fbx, .obj, etc.

| **Octree** | **PBR** |
|:-:|:-:|
| <img src="./documentation/images/octree.jpg"/><br>Octree demonstartion | <img src="./documentation/images/pbr2.jpg"/><br>Demonstration pbr shading with shadows |
| **Post-Processing** | **Post-Processing** |
| <img src="./documentation/images/pp.jpg"/><br> Demonstration of layered post-processing (bloom) | <img src="./documentation/images/pp2.jpg"/><br> Demonstration of layered post-processing |

### Platforms
- 🛠️ Cross Platform
    - ✔️ Windows
    - ❌ MacOS
    - ✔️ Linux
    - ❌ Andriod
    - ❌ iOS
    - ❌ Web

## Contributors 🌟

- [Ilya (pfhgil)](https://github.com/pfhgil)
- [Nikita (8bitniksis)](https://github.com/8bitniksis)
- [Vlad (MisterChoose)](https://github.com/MisterChoose)

# License 📜
Distributed under the MIT License. See `LICENSE.txt` for more information.

# Contributing
## Cloning the repository using console commands:
Open your console application of choice and run the following console commands:

```
\> cd <Path to your folder>

\> git clone --recurse-submodules -j8 https://github.com/Pixelfield-ru/SungearEngine

\> cd SungearEngine

\> git checkout main (or the latest tag)
```

# Reporting Issues

If you encounter any bugs when using Sungear Engine, please report them in the bug tracker. This is hosted at:

https://github.com/Pixelfield-ru/SungearEngine/issues

Make sure to first use the search function to see if the bug has already been reported. When filling out a bug report, make sure that you include as much information as possible to help the developers track down the issue, such as your version of SungearEngine, operating system, architecture, and any code and models that are necessary for the developers to reproduce the issue.

# How to build
To build up make sure to:
1) Build and bootstrap vcpkg.
2) Set SUNGEAR_LLVM_MINGW_PATH system variable to your MinGW (cygwin/other-cross-compiler?..) compiler.

Be sure to set up the correct path without illegal-characters (use / in path instead of \ for obvious reasons).
