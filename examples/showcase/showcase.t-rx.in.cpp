extern "C" {
    #include<stdio.h>
}

#include <fstream>
#include <iostream>
#include <chrono>
#include "source0.hpp"
#include "../../include/trx.hpp"

#define DEBUG               true
#define USAGE_TESTS         true
#undef COMPILE_GENERATED
#define PRINT_GENERATED_CODE false

static void nothing(){}
void tests();

int main(){
    tests();
}

///////////////////////// GENERATED CODE UTILITIES
// utils for visualizing the code generated
template<typename t>
static void n_trace(t n, void* obj) {if (DEBUG) std::cout << "[TRACE] " << n << "\n" << std::flush;}
static void c_trace(void* obj)      {if (DEBUG) std::cout << "[TRACE] seq > completed\n" << std::flush;  }

class observer { // not sure this is necessary yet
public:
    static void n_trace(short n)  {if (DEBUG) std::cout << "seq2 output " << n << "\n" << std::flush;}
    static void c_trace()         {if (DEBUG) std::cout << "seq2 completed\n" << std::flush;  }
};

///////////////////////// TESTS
void test_generated();
void tests(){
    if (DEBUG) std::cout << "\n\n//// START OF TESTS /////\n";
    if (USAGE_TESTS == true)
    {
    	test_generated();
        if (DEBUG) std::cout << "************* end test ************\n" << std::flush;
    }
    else
    {
        source0_sub(n_trace,c_trace,nullptr);
        (*source0_callback)(1);
        source0_stop(nullptr);
    }
}

void test_generated(){
  unsigned long c = 0;
  if (DEBUG) std::cout  << "\nlibrary warm up, to make it as fair as possible: "
                        << c;

  auto start_time = std::chrono::high_resolution_clock::now();
  {
      auto c0= &c;

      sequence seq16 = [c0] => fromStaticPublisher<unsigned> (source0_sub, source0_disp)
          select x => { return (short) x *2 ;}
          select x => { return { c = (short) x *2}; }
          select x => x.c
          select x => (short)x +2
          select x => struct { a = x, b = 1}
          selectmany 3, [x] => fromCapture()
                select y => y.a+2
                select y => y*x.b
                endObservable
          select x => x-1
          endObservable
          ;

      seq16.subscribe(&n_trace, &c_trace, nullptr);
      if (DEBUG) std::cout << "\nready to go, enable: \n" << std::flush;
      seq16.enable(c0);

      for(int j=0; j<1; j++){
        for(int i=0; i<5; i++){
            if (DEBUG) std::cout << "********** new value incoming\n" << std::flush;
            (*source0_callback)(1+i);
        }
        if (DEBUG) std::cout << "last element was sent by source 0, now complete\n" << std::flush;
        (*source0_done)();
      }
      
      seq16.disable();
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto time = end_time - start_time;


  if (true) std::cout   << "\ndone\n"
                        << std::flush;
}
