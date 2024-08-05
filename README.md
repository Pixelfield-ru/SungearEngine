# Sungear Engine
A general purpose game engine written in C++

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

# Additional information
To build up make sure to:
1) Build and bootstrap vcpkg.
2) Set SG_VCPKG_ROOT system variable to bootstraped vcpkg.
3) Set SUNGEAR_LLVM_MINGW_PATH system variable to your MinGW (cygwin/other-cross-compiler?..) compiler.

Be sure to set up the correct path without illegal-characters (use / in path instead of \ for obvious reasons).