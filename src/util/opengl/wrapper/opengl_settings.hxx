#pragma once

#define OPENGL_CURRENT_MAJOR_VERSION 3

#define OPENGL_CURRENT_MINOR_VERSION 0

#define OPENGL_INFO_LOG_SIZE 512

#ifdef __ANDROID__

#if OPENGL_CURRENT_MAJOR_VERSION == 3 && OPENGL_CURRENT_MINOR_VERSION == 2
#include <GLES3/gl32.h>
#endif

#if OPENGL_CURRENT_MAJOR_VERSION == 3 && OPENGL_CURRENT_MINOR_VERSION == 1
#include <GLES3/gl31.h>
#endif

#if OPENGL_CURRENT_MAJOR_VERSION == 3 && OPENGL_CURRENT_MINOR_VERSION == 0
#include <GLES3/gl3.h>
#endif

#endif // __ANDROID__

#ifndef __ANDROID__

#include <glad_utils.hxx>

#endif
