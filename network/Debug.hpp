#ifndef debug_hpp
#define debug_hpp

/*
 * Uncomment the next line to globally enable
 * debug output to serial
 */
#define DEBUG

#ifdef DEBUG

#define DEBUG_PRINT(...) { Serial.printf("%s: %s: %d: ", __FILE__, __func__, __LINE__); Serial.printf(__VA_ARGS__); Serial.print(F("\n")); }
#define DEBUG_DO(CODE) { CODE; }
#define DEBUG_STR(VAL) (VAL.toString().c_str())

#else

#define DEBUG_PRINT(...)
#define DEBUG_DO

#endif

#endif
