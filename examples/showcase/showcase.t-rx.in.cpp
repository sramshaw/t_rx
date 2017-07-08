extern "C" {
    #include<stdio.h>
}

#include <fstream>
#include <iostream>
#include <chrono>
#include "tick1.hpp"
#include "../../include/trx.hpp"


#define DEBUG               false

static void nothing(){}
void tests();

int main(){
    tests();
}

///////////////////////// GENERATED CODE UTILITIES
// utils for visualizing the code generated
static void n_trace(long n, void* obj) {if (DEBUG) std::cout << "[TRACE] " << n << "\n" << std::flush;}
static void c_trace(void* obj)      {if (DEBUG) std::cout << "[TRACE] seq > completed\n" << std::flush;  }

///////////////////////// TESTS
void test_generated();
void tests(){
    if (DEBUG) std::cout << "\n\n//// START OF TESTS /////\n";
    {
    	test_generated();
        if (DEBUG) std::cout << "************* end test ************\n" << std::flush;
    }
}


void test_generated(){
  struct { long v; long total; } c;
  c.v= 0;
  c.total = 0;

  auto start_time = std::chrono::high_resolution_clock::now();
  {
      auto c0= c;

      std::cout << "value in sequence c0: " << c0.total << "\n" << std::flush;
      sequence seq16 = [c0] => fromPublisher<unsigned> (tick1)
          scan c0, (acc,n) => { acc.total++;  return acc;  }
          select x => x.total
          select x => { return x *1 ;}
          select x => { return { c =  x +0}; }
          select x => x.c
          select x => (long)x + 0
          select x => struct { a = x, b = 1}
          selectmany 3, [x] => fromCapture()
                select y => y.a + 0
                select y => y*x.b
                endObservable
          select x => (x + 0)
          endObservable
          ;

      seq16.subscribe(&n_trace, &c_trace, nullptr);
      if (DEBUG) std::cout << "\nready to go, enable: \n" << std::flush;
      seq16.enable(c0);

      for(int j=0; j<1; j++){
        for(int i=0; i<10; i++){
            if (DEBUG) std::cout << "********** new value incoming\n" << std::flush;
            interface_tick1::onnext(1+i);
        }
        if (DEBUG) std::cout << "last element was sent by source 0, now complete\n" << std::flush;
        interface_tick1::oncomplete();
      }
      
      seq16.disable();
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto time = end_time - start_time;


  if (true) std::cout   << "\ndone in time: \n"
                        << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " microsecs\n\n"
                        << std::flush;
}
