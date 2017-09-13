#pragma once

#include "platform.h"
#include <stdio.h>

#if defined DEBUG
#define UDP_TRACE(fmt, ...) fprintf(stderr, fmt"\n", __VA_ARGS__);
#elif
#define TRACE(CHAN, MESSAGE, ...)
#endif