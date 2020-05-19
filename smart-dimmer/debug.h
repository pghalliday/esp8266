#ifndef debug_h
#define debug_h

#include <Arduino.h>

/*
 * Uncomment the next line to globally enable
 * debug output to serial
 */
#define DEBUG

#ifdef DEBUG

// Internal macros
#define _DEBUG_FILE Serial.print(F(__FILE__))
#define _DEBUG_LINE Serial.print(__LINE__)
#define _DEBUG_FUNC Serial.print(__func__)
#define _DEBUG_SEP Serial.print(F(": "))
#define _DEBUG_START_VAL Serial.print(F("["))
#define _DEBUG_END_VAL Serial.print(F("]"))
#define _DEBUG_NEW_LINE Serial.println(F(""))
#define _DEBUG_PREFIX _DEBUG_FILE; _DEBUG_SEP; _DEBUG_FUNC; _DEBUG_SEP; _DEBUG_LINE
#define _DEBUG_VAL(LABEL, VALUE) _DEBUG_SEP; Serial.print(LABEL); _DEBUG_SEP; _DEBUG_START_VAL; Serial.print(VALUE); _DEBUG_END_VAL
#define _DEBUG_MSG(MSG) _DEBUG_SEP; Serial.print(MSG)

// Public macros
#define DEBUG_MSG(MSG) { _DEBUG_PREFIX; _DEBUG_MSG(MSG); _DEBUG_NEW_LINE; }
#define DEBUG_VAL(MSG, LABEL, VALUE) { _DEBUG_PREFIX; _DEBUG_MSG(MSG); _DEBUG_VAL(LABEL, VALUE); _DEBUG_NEW_LINE; }
#define DEBUG_LIST_START(MSG) { _DEBUG_PREFIX; _DEBUG_MSG(MSG)
#define DEBUG_LIST_VAL(LABEL, VALUE) _DEBUG_VAL(LABEL, VALUE)
#define DEBUG_LIST_END _DEBUG_NEW_LINE; }
#define DEBUG_DO(CODE) { CODE; }

#else

#define DEBUG_START(BAUDRATE)
#define DEBUG_MSG(MSG)
#define DEBUG_VAL(MSG, LABEL, VALUE)
#define DEBUG_LIST(MSG)
#define DEBUG_LIST_VAL(LABEL, VALUE)
#define DEBUG_LIST_END
#define DEBUG_DO

#endif

#endif
