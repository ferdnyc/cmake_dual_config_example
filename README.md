### CMake dual-config library example with dependencies

This example uses, and builds off of, the [tinyxml2][tinyxml2] model
for exporting CMake configuration for both static and shared libraries.
But unlike tinyxml2 itself, which has no dependencies, the xmltest
library in this project (a modified version of tinyxml2's own test
code) has a library dependency: tinyxml2 itself.

tinyxml2 can be built as either a shared or static library, and
installs a configuration which allows either to be selected when
importing the package into CMake. Ideally this is done by selecting
either the `shared` or `static` component of the package, i.e.:

```cmake
find_package(tinyxml2 REQUIRED COMPONENTS shared)
# loads the tinyxml2-shared-targets.cmake export definition
# tinyxml2::tinyxml2 is now a SHARED IMPORTED target for libtinyxml2.so.10

find_package(tinyxml2 REQUIRED COMPONENTS static)
# loads the tinyxml2-static-targets.cmake export definition
# tinyxml2::tinyxml2 is a STATIC IMPORTED target for libtinyxml2.a
```

The `xmltest` library project in this directory loads the corresponding
`tinyxml2` component based on whether it's being built as a `SHARED` or
`STATIC` library, and exports a configuration that accepts the same
`COMPONENTS` arguments to `find_package()`, then uses the CMake
`find_dependency()` macro to select the same `COMPONENTS` of its own
tinyxml2 dependency.

```cmake
find_package(xmltest REQUIRED COMPONENTS shared)
# loads the xmltest-config.cmake configuration, which will call
# find_dependency(xmltest2 REQUIRED COMPONENTS shared)
# then load its own xmltest-shared-targets.cmake export definition.
# 
# xmltest::xmltest is a SHARED IMPORTED target for libxmlest.so.0,
# with an IMPORTED_LINK_LIBRARY dependency on tinyxml2::tinyxml2,
# which is a SHARED IMPORTED target for libtinyxml2.so.10


find_package(xmltest REQUIRED COMPONENTS static)
# loads the xmltest-config.cmake configuration, which will call
# find_dependency(xmltest2 REQUIRED COMPONENTS static)
# then load its own xmltest-static-targets.cmake export definition.
# 
# xmltest::xmltest is a STATIC IMPORTED target for libxmlest.a,
# with an IMPORTED_LINK_LIBRARY dependency on tinyxml2::tinyxml2,
# which is a STATIC IMPORTED target for libtinyxml2.a
```

## To test the configurations (and libtinyxml2 itself)...

```console
$ git clone https://github.com/leethomason/tinyxml2
$ cmake -B _build_tinyxml2_static -S tinyxml2 -DBUILD_SHARED_LIBS=0 \
  -DCMAKE_INSTALL_PREFIX=./_dist
$ cmake --build _build_tinyxml2_static
$ cmake --install _build_tinyxml2_static

$ cmake -B _build_tinyxml2_shared -S tinyxml2 -DBUILD_SHARED_LIBS=1 \
  -DCMAKE_INSTALL_PREFIX=./_dist
$ cmake --build _build_tinyxml2_shared
$ cmake --install _build_tinyxml2_shared

$ cmake -B _build_xmltest_static -S xmltest -DBUILD_SHARED_LIBS=0 \
  -Dtinyxml2_ROOT=./_dist -DCMAKE_INSTALL_PREFIX=./_dist
$ cmake --build _build_xmltest_static
$ cmake --install _build_xmltest_static

$ cmake -B _build_xmltest_shared -S xmltest -DBUILD_SHARED_LIBS=1 \
  -Dtinyxml2_ROOT=./_dist -DCMAKE_INSTALL_PREFIX=./_dist
$ cmake --build _build_xmltest_shared
$ cmake --install _build_xmltest_shared

$ # At this point, both libtinyxml2 and libxmltest are installed in
$ # both static and shared configurations. Either can be used to
$ # build the 'runtests' binary.

$ cd runtests

$ cmake -B _build_static -S . -DUSE_STATIC=1 \
  -Dtinyxml2_ROOT=../_dist -Dxmltest_ROOT=../_dist
$ cmake --build _build_static
$ ./_build_static/runtests  # run statically-linked tinyxml2 tests

$ cmake -B _build_shared -S . -DUSE_STATIC=0 \
  -Dtinyxml2_ROOT=../_dist -Dxmltest_ROOT=../_dist
$ cmake --build _build_shared
$ ./_build_shared/runtests  # run tinyxml2 tests on libtinyxml2.so.10
```

### License

The code unique to this project is licensed under the Creative Commons CC0-1.0 public domain dedication. See [LICENSES/CC0-1.0.txt][].

[tinyxml2][tinyxml2] is licensed under the terms of the included [LICENSES/tinyxml2.txt][] license.

[tinyxml2]: https://github.com/leethomason/tinyxml2
