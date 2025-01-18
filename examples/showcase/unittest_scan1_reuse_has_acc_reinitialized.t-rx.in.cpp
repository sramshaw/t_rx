extern "C" {
    #include<stdio.h>
}
#include <iostream>
#include "../../include/trx.hpp"

static void n_trace(long n, void* obj) {std::cout << "[TRACE] " << n << "\n" << std::flush;}
static void c_trace(void* obj)         {std::cout << "[TRACE] seq > completed\n" << std::flush;  }
int main(){
    struct { long v;} c;
    std::array<long,1> totals2 ={0};
    auto c0= &c;
#define SEQUENCE
    sequence seq1 = [c0] => fromRange<unsigned> (1,2)
        select     x => struct { a = x, c0 = c0}
        do     w => std::cout << "_w = " << w.a       << "\n" << std::flush
        selectmany 1, [c1] => fromRange<unsigned>(1, 2)
            do     x => std::cout << "_x = " << x       << "\n" << std::flush
            selectmany 1, [c2] => fromRange<unsigned>(1, 2)
                do     y => std::cout << "_y = " << y                 << std::flush
                scan [totals2], (acc,n) => { acc += n;  return acc;  }, acc => acc
                do     acc => std::cout << ", accs = " << acc << "\n" << std::flush
                endObservable
            endObservable
        endObservable
        ;
#undef SEQUENCE

    seq1.subscribe(&n_trace, &c_trace, nullptr);
    seq1.enable(c0);

}