#pragma once
#include "fmt/format.h"
#include <iostream>

#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 4
#define ENGINE_VERSION_PATCH 0

#define ENGINE_VERSION_STRING fmt::format("v{}.{}.{}", ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH)