#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Spark/Log.h"
#include "Spark/Core.h"

#ifdef SPARK_PLATFORM_WINDOWS
	#include <Windows.h>
#endif