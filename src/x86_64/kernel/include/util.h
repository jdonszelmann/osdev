
#ifndef UTIL_H
#define UTIL_H

// preprocessor utilities

#define UNUSED(x) ((x)=(x))

#define MACROCAT(a, ...) PRIMITIVE_MACROCAT(a, __VA_ARGS__)
#define PRIMITIVE_MACROCAT(a, ...) a ## __VA_ARGS__

#define DEBUG 1

#endif