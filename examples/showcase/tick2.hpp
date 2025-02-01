#include <iostream>
#include <array>
#include "../../include/trx.hpp"

//////////////////////// SOURCE OF TICKS

// observable to use in the sequence
static publishedObservable<unsigned> tick2;

class interface_tick2 {
// API to stimulate
public:
    static void onnext(unsigned v) {
        tick2.direct_exit_next(v);
    }
    static void oncomplete() {
        tick2.direct_exit_complete();
    }
};



