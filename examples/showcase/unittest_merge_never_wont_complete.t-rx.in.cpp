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
    struct { int val;} c = {};
    auto c0= &c;
#define SEQUENCE
    sequence seq1 = [c0] => fromJust<unsigned> (1)
        merge  [c1] => fromNever<unsigned> ()
            endObservable
        do     x => std::cout <<  "_x = " << x << "\n" << std::flush
        endObservable
        ;
#undef SEQUENCE

    seq1.subscribe(n_trace, &c_trace, nullptr);
    seq1.enable(c0);

    bool completed = !seq1.enabled;

    std::cout << "\n";
    std::cout << "completed :  " << (completed ? "true" : "false") << "\n" << std::flush;
    SMART_ASSERT(completed, == false, "merge logic with a never source should prevent sequence completion ");
}