extern "C" {
    #include<stdio.h>
}
#include <iostream>
#include <array>
#include "tick1.hpp"
//leave an empty line under the include below
#include "../../include/trx.hpp"


using namespace std;

static std::array<unsigned,10> _results = {};
static int _index =0;
static void n_trace(unsigned n, void* obj) {std::cout << "[TRACE] " << n << "\n" << std::flush; _results[_index++]=n;}
static void c_trace(void* obj)         {std::cout << "[TRACE] seq > completed\n" << std::flush;  }


int main(){
    struct { int count ; std::array<unsigned,5> store;} c = {};
    auto c0= &c;
#define SEQUENCE
    sequence seq1 = [c0] => fromJust<unsigned> (1)
        merge  [c1] => fromJust<unsigned> (2)
            endObservable
        do     x => std::cout <<  "_x = " << x << "\n" << std::flush
        do     x => c0->store[c0->count++] =x
        endObservable
        ;
#undef SEQUENCE

    seq1.subscribe(n_trace, &c_trace, nullptr);
    seq1.enable(c0);

    std::cout << "\n";
    std::cout << "count :  " << c.count <<"\n";
    SMART_ASSERT(c.count, == 2, " 2 sources of one element should push a merged total of 2 elements ");
}