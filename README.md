# Bitstring
This is a small library for creating and manipulating arbitrarily sized bitstrings.
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
lib_deps = aberratic/Bitstring @ ^0.2.0
```
Furthermore to use menuconfig you need to add the following line to your
project CMakeLists.txt

```CMakeLists.txt
list(APPEND EXTRA_COMPONENT_DIRS ".pio/libdeps/esp32dev/Bitstring")
```

## Configuration
There is one compile time configuration value:

CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS

This setting enable out of bound checks. When a OOB access occurs and this
setting is enabled the app will panic.
If this setting is disabled and a OOB access occurs the behavior is undefined.

I recommend to enable this setting.

## How to use the library
Just look into include/bitstring.h . It is well documented.

## Changelog

| Version | Changes                                    |
|---------|--------------------------------------------|
| 0.2     | Added bstr_ffus(), to find first unset bit |
| 0.1     | Initial Release                            |
