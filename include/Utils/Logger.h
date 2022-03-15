#ifndef LOGGER_H
#define LOGGER_H

#include "ofMain.h"
#include <string.h>

namespace ift3100 {
// Extract raw filename from file path
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define ANSI_RESET "\u001B[0m"
#define ANSI_RED "\u001B[31m"
#define ANSI_YELLOW "\u001B[33m"
#define ANSI_RED_BACKGROUND "\u001B[41m"

// Personal logger using ofLog logger and loglevels
// This allow us to format the logs because ofLog won't.
// format :
// <filename::function_name::line_number> message_passed_as_args

// LOGLEVEL :
// - NOTICE
// - WARNING
// - ERROR
// - FATAL
// Usage :
// IFT_LOG_{LEVEL} << "your message in here" << "concatenation works or " << " text too"

#define FORMAT_STREAM "<" << __FILENAME__ << "::" << __FUNCTION__ << "::" << __LINE__ << ">"

#define IFT_LOG ofLogNotice() << FORMAT_STREAM << " "
#define IFT_LOG_WARNING                                                                                                                              \
	std::cout << ANSI_YELLOW;                                                                                                                        \
	ofLogWarning() << FORMAT_STREAM << ANSI_RESET << " "
#define IFT_LOG_ERROR                                                                                                                                \
	std::cerr << ANSI_RED;                                                                                                                           \
	ofLogError() << FORMAT_STREAM << ANSI_RESET << " "
#define IFT_LOG_FATAL                                                                                                                                \
	std::cerr << ANSI_RED_BACKGROUND;                                                                                                                \
	ofLogFatalError() << FORMAT_STREAM << ANSI_RESET << " "

} // namespace ift3100

#endif