#ifndef LOGGER_H
#define LOGGER_H

#include <string.h>

namespace ift3100 {
// Extract raw filename from file path
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

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

#define FORMAT_STREAM "<" << __FILENAME__ << "::" << __FUNCTION__ << "::" << __LINE__ << "> "

#define IFT_LOG ofLogNotice() << FORMAT_STREAM
#define IFT_LOG_WARNING ofLogWarning() << FORMAT_STREAM
#define IFT_LOG_ERROR ofLogError() << FORMAT_STREAM
#define IFT_LOG_FATAL ofLogFatalError() << FORMAT_STREAM

} // namespace ift3100

#endif