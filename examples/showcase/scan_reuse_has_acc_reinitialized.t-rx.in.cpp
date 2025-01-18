extern "C" {
    #include<stdio.h>
}

#include <fstream>
#include <iostream>
#include <chrono>
#include "tick1.hpp"
#include "../../include/trx.hpp"


#define DEBUG               true
#define COMPLETED           true
#define PROOF_BY_MESSAGE    true

static void nothing(){}
void tests();

int main(){
    tests();
}

///////////////////////// GENERATED CODE UTILITIES
// utils for visualizing the code generated
static void n_trace(unsigned n, void* obj) {if (DEBUG) std::cout << "[TRACE] " << n << "\n" << std::flush;}
static void c_trace(void* obj)      {if (COMPLETED) std::cout << "[TRACE] seq > completed\n" << std::flush;  }

///////////////////////// TESTS
void test_generated_main_loop();
void tests(){
    if (DEBUG) std::cout << "\n\n//// START OF TESTS /////\n";
    {
    	test_generated_main_loop();
        if (DEBUG) std::cout << "************* end test ************\n" << std::flush;
    }
}


void test_generated_main_loop(){
  struct { long v;} c;
std::array<long,1> totals2 ={0};
auto c0= &c;
#define SEQUENCE
    sequence seq1 = [c0] => fromRange<unsigned> (1,2)
        select     x => struct { a = x, c0 = c0}
        selectmany 1, [c1] => fromRange<unsigned>(1, 2)
            scan [totals2], (acc,n) => { acc += n;  return acc;  }, acc => acc
            do     acc => std::cout << ", accs = " << acc << "\n" << std::flush
            endObservable
        endObservable
        ;
#undef SEQUENCE;

      seq1.subscribe(&n_trace, &c_trace, nullptr);
      if (DEBUG) std::cout << "\nready to go, enable: \n" << std::flush;
      seq1.enable(c0);

}