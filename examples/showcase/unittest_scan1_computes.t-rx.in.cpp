extern "C" {
    #include<stdio.h>
}
#include <iostream>
#include "../../include/trx.hpp"

static void n_trace(long n, void* obj) {std::cout << "[TRACE] " << n << "\n" << std::flush;}
static void c_trace(void* obj)         {std::cout << "[TRACE] seq > completed\n" << std::flush;  }


int main(){
    struct { long result;} c;
    std::array<long,1> totals2 ={0};
    auto c0= &c;
#define SEQUENCE
    sequence seq1 = [c0] => fromRange<unsigned> (1,2)
        do     x => std::cout << "_x = " << x               << std::flush
        scan [totals2], (acc,n) => { acc += n;  return acc;  }, acc => acc
        do     y => std::cout << "_y = " << y       << "\n" << std::flush
        do     y => c0->result =y
        endObservable
        ;
#undef SEQUENCE
    seq1.subscribe(&n_trace, c_trace, nullptr);
    seq1.enable(c0);
    std::cout << "[RESULT] >  " << c.result <<"\n" << std::flush;
    SMART_ASSERT(c.result == 3, "this is basic scan computation to accumulate 1 and 2, =3");
}