# Bitstring
This is a small library for creating and manipulating arbitrarily sized bitstrings. There is support for static sized (and stack allocated) aswell
as heap allocated (and resizable) bitstrings.
It is published at [platformio](https://platformio.org) and has all batteries
included to work with ESP-IDF.

## License
This code is licensed under the MIT license.

## Installation
There are two ways of adding this library to your project. You could either:

### Add as submodule
Just run the following line to add this library to your components directory.
```bash
git submodule add https://github.com/aberratic/bitstring.git components
```

If you are not using ESP IDF and its configuration system take care of setting
the configuration below.

### Add as platformio dependency
Add to your platformio.ini the following line:
```ini
lib_deps = aberratic/Bitstring @ ^2.0.3
```
Furthermore to use menuconfig you need to add the following line to your
project CMakeLists.txt

```CMakeLists.txt
list(APPEND EXTRA_COMPONENT_DIRS ".pio/libdeps/esp32dev/Bitstring")
```

## Configuration
There is one compile time configuration value:

CONFIG_BITSTRING_ENABLE_BOUND_CHECKS

This setting enable out of bound checks. When a OOB access occurs and this
setting is enabled the app will panic.
If this setting is disabled and a OOB access occurs the behavior is undefined.

I recommend to enable this setting.

## How to use the library
Just look into include/bitstring.h or bitstring/bitstring_static.h. It is well documented.
There are also examples in the examples directory.

## Changelog

| Version | Changes                                                            |
|---------|--------------------------------------------------------------------|
| 2.0.3   | Fixed a bug with false strncat string sizes.                       |
| 2.0.2   |                                                                    |
| 2.0.1   |                                                                    |
| 2.0     | Renamed CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS to                    |
|         | CONFIG_BITSTRING_ENABLE_BOUND_CHECKS                               |
|         | Added support for stack allocated / static sized bitstrings        |
|         | Added examples                                                     |
| 1.0     | Renamed bstr_err_t variants                                        |
|         | Added function attributes                                          |
|         | Added two unit tests for bstr_resize()                             |
| 0.2     | Added bstr_ffus(), to find first unset bit                         |
| 0.1     | Initial Release                                                    |
