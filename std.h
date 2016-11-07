#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>

#include <errno.h>

#include <assert.h>

#define PI (M_PI)
#define PI2 (2*M_PI)
#define LN2 (M_LN2)

#if defined SIMPLE_LOGGING
#include "log.h"
#endif
