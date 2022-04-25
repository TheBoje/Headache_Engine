#ifndef IFT_ASSERTS_H
#define IFT_ASSERTS_H

#include "Logger.h"
#include <stdlib.h>

namespace ift3100 {
#define IFT_ASSERT(expr, message)                                                                                                                    \
	if (!(expr)) {                                                                                                                                   \
		IFT_LOG_FATAL << message;                                                                                                                    \
		exit(EXIT_FAILURE);                                                                                                                          \
	}
} // namespace ift3100

#endif