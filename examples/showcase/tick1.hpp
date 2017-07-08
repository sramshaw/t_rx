#include <iostream>
#include <array>
#include "../../include/trx.hpp"

//////////////////////// SOURCE OF TICKS

// observable to use in the sequence
static publishedObservable<unsigned> tick1;

class interface_tick1 {
// API to stimulate
public:
    static void onnext(unsigned v) {
        tick1.direct_exit_next(v);
    }
    static void oncomplete() {
        tick1.direct_exit_complete();
    }
};



