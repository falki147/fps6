#pragma once

#ifdef __EMSCRIPTEN__
// For emscripten, instead of using glad we use its built-in support for OpenGL:
#include <GLES3/gl3.h>
#include "emscripten.h"
#else
#include "glad/glad.h"
#endif
