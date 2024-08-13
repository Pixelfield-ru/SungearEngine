<img src="./documentation/images/mainLogo.png" width="100%">
<p align="center">🎮 An Open Source Engine Written in C++! 🎮</p>

# <p align="center">✨ Features ✨</p>

-   **General:**
    - ECS
    - Audio(OpenAL)
    - Parallel safety thread physics (Cooliders differents forms, RigidBodys)
    - Octotree with automatic subdivision
    - Support input from keyboard and mouse
    - Annotation and CodeGen  
        > (more information about annotations, see the source code: https://github.com/Pixelfield-ru/SungearEngine/tree/main/Sources/SGCore/Annotations)
        ```
        sg_struct()
        struct MyStruct
        {
            sg_member()
            float m_myMember = 3.0f;
        }
        ```
    - A set of classes for parallel computing
      > (more information about parallel computing, see the source code: (https://github.com/Pixelfield-ru/SungearEngine/tree/main/Sources/SGCore/Threading))
-   **Graphics Rendering:**
    - Shader system(Similiar as Unity)
    - PBR (Physically Based Rendering) using Cook-Torrance BRDF
    - Albedo Map
    - Normal Map
    - Roughness Map
    - Metallic Map
    - Ambient Occclusion Map
    - Emission Map
    - Shadows(Directional Light only)
    - Atmosphere Scattering
      
-   **Post Processing:**
    - Layered postprocessing (you can add different objects to different layers and apply different postprocessing to them, be it bluer, bloom, etc.)

-   **Asset Pipeline:**
    - Plugin Systems(Similiar as Unity)
    - Load Textures and svg files
    - Load many model formats such as .fbx, .obj, etc.


### Engine
- 🛠️ Cross Platform
    - ✔️ Windows
    - ❌ MacOS
    - ✔️ Linux
    - ❌ Andriod
    - ❌ iOS
    - ❌ Web

## Contributors 🌟

- [Ilia (pfhgil)](https://github.com/pfhgil)
- [Nikita (8bitniksis)](https://github.com/8bitniksis)
- [MisterChoose](https://github.com/MisterChoose)

# License 📜
Distributed under the MIT License. See `LICENSE.txt` for more information.

# Contributing
## Cloning the repository using console commands:
Open your console application of choice and run the following console commands:

```
\> cd <Path to your folder>

\> git clone https://github.com/Pixelfield-ru/SungearEngine

\> cd SungearEngine

\>  git checkout main (or the latest tag)
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
