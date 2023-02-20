#include "dispatchinformation.h"
#if (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#include <limits>
#endif

const DispatchInformation::BEGIN_TYPE DispatchInformation::INVALID_BEGIN = std::numeric_limits<DispatchInformation::BEGIN_TYPE>::max();
