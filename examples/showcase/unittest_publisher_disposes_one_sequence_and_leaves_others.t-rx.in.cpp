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
    sequence seq1 = [c0] => fromPublisher<unsigned> (tick1)
        where  x => x%2 == 0 && x <= 2
        do     x => std::cout << "_x = " << x     << "\n" << std::flush
        selectmany 2,  [c1] => fromPublisher<unsigned> (tick1)
            where  x => x%2 == 1 || c1 ==2
            do     y => std::cout <<  " c1 = " << c1 << "_y = " << y << "\n" << std::flush
            take   4
            endObservable
        endObservable
        ;
#undef SEQUENCE

    seq1.subscribe(n_trace, &c_trace, nullptr);
    seq1.enable(c0);

    auto client_count_at4 =0;
    for(int i=0; i<=6; i++){
        interface_tick1::onnext(i);
        // after 4  items, there should be 3 clients active for the publisher
        if (i == 3)
            client_count_at4 = tick1.activeClientsCount();
    }
    auto client_count_at7 = tick1.activeClientsCount();
    
    std::cout << "\n";
    std::cout << "[client_count_at4] :  " << client_count_at4 <<"\n";
    std::cout << "[client_count_at7] :  " << client_count_at7 <<"\n" << std::flush;
    SMART_ASSERT(client_count_at7, == (client_count_at4 - 1), "one publisher is stopped, and available for reuse ");
}