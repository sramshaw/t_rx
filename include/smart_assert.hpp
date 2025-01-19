#include <string>
#include <cassert>

#define SMART_ASSERT(val, expr, message ) \
    do { \
        if (!( val expr)) { \
            std::cerr << "Assertion failed: " << #val " " #expr << std::endl; \
            std::cerr << "Message: " << message << ", value: " << val << std::endl; \
            std::cerr << "File: " << __FILE__ << std::endl; \
            std::cerr << "Line: " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while (false)
