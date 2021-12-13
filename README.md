# glGetTool - OpenGL utility to get render states

## Building
You need a C99 compiler, CMake 3.10, OpenGL, and SDL 2.0.

```sh
# Try it out without installing.
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/glGet

# Installation is also supported; you can set the prefix to whatever you want.
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
sudo cmake --install build
glGet

# When built for Windows with MSVC/Visual Studio, SDL 2.0 is linked statically,
# so you don't need to worry about `SDL2.dll`. VCPKG can be used to produce
# such a build.
```

## Usage Examples

You use the command line utility, `glGet`, with a filter string. All enum names
that contain the filter string anywhere within them will be printed out with
their values.

```sh
# The default is the latest version of OpenGL, compatibility profile (4.6 for
# now). The default will not work on macOS, as macOS only allows core profile
# with versions 3.0 and above, and (for now, and likely always) macOS only
# supports up to 4.1.
glGet GL_MAX_TEXTURE_SIZE

# You can specify an OpenGL version to use. By default, compatibility profile
# is used.
glGet GL_MAX_TEXTURE_SIZE 1.0

# And, lastly, you can specify the context profile to use. Invalid combinations
# of version number and profile will default to 4.6 compatibility. macOS
# doesn't support ES profile at all.
glGet GL_MAX_TEXTURE_SIZE 3.0 Core
glGet GL_MAX_TEXTURE_SIZE 3.0 Compatibility
glGet GL_MAX_TEXTURE_SIZE 3.0 ES

# Case doesn't matter
glGet gl_max_texture_size 3.0 es
```

**Public domain** (CC0)
