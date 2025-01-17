#include <string>
#include <cassert>

#define SMART_ASSERT(expr, message) \
    do { \
        if (!(expr)) { \
            std::cerr << "Assertion failed: " << #expr << std::endl; \
            std::cerr << "Message: " << message << std::endl; \
            std::cerr << "File: " << __FILE__ << std::endl; \
            std::cerr << "Line: " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while (false)

//     SMART_ASSERT(x > 0, "x should have been positive"); // This will pass
