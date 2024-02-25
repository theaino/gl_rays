# GL_RAYS

> This is not very detailed;
> just that I don't forget the build commands.

## Build

### Requirements

- CMake >= 3.5
- Vcpkg
- OpenGL >= 4.6

> [!NOTE]
> VCPKG_ROOT has to be set
> for CMake to find vcpkg/the libraries.

To build the project,
run following commands in the main project dir:

`cmake -B build -S . --preset debug`

`cmake --build build`

Then you can just run `build/gl_rays`.
