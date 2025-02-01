extern "C" {
    #include<stdio.h>
}
#include <iostream>
#include <array>
#include "tick1.hpp"
#include "tick2.hpp"
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
    sequence seq1 = [c0] => fromPublisher<unsigned> (tick1)
        where  x => x%2 == 0
        select x => struct { x=x, sub_seq = &c0 }
        selectmany 2,  [c1] => fromPublisher<unsigned> (tick2)
            where  x => x%2 == 1
            endObservable
        endObservable
        ;
#undef SEQUENCE

    seq1.subscribe(n_trace, &c_trace, nullptr);
    seq1.enable(c0);

    for(int i=0; i<=3; i++){
        std::cout << "********** new value incoming\n" << std::flush;
        interface_tick1::onnext(i);
        interface_tick2::onnext(i);
    }
    interface_tick1::oncomplete();
    
    int enabled_count=0;
    for(auto& e: seq1_sequence3_array) // uses knowledge of the internal wiring
    {
        if (e.enabled) enabled_count++;
    }

    std::cout << "[RESULT] :  " << enabled_count <<"\n" << std::flush;
    SMART_ASSERT(enabled_count, == 0, "the completion of the publisher used in the main sequence should disable all sub sequences");
}