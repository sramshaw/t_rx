extern "C" {
    #include<stdio.h>
}
#include <iostream>
//leave an empty line under the include below
#include "../../include/trx.hpp"


static void n_trace(unsigned n, void* obj) {std::cout << "[TRACE] " << n << "\n" << std::flush;}
static void c_trace(void* obj)         {std::cout << "[TRACE] seq > completed\n" << std::flush;  }

int main(){
    struct { long result; long count;} c = {0,0};
    auto c0= &c;
#define SEQUENCE
    sequence seq1 = [c0] => fromRange<unsigned> (10,12)
        do     x => std::cout << "_x = " << x               << std::flush
        select x => {c0->count++; c0->result = x; return x;}
        endObservable
        ;
#undef SEQUENCE
    seq1.subscribe(&n_trace, c_trace, nullptr);
    seq1.enable(c0);
    std::cout << "[COUNT] >  "  << c.count <<"\n" << std::flush;
    std::cout << "[RESULT] >  " << c.result <<"\n" << std::flush;
    SMART_ASSERT(c.result , == 12, "the last value expected is 12");
    SMART_ASSERT(c.count  , == 3, "the count of emitted values from 10 to 12 is 3");
}