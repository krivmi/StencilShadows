// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string>
#include <algorithm>
#include <memory>
#include <limits>
#include <array>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <optional>
#include <math.h>
#include <float.h>
#include <assert.h>

#ifdef _WIN32
#include "winerror.h"
#else
#define S_OK 0
#endif

#endif //PCH_H
