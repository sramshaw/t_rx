extern "C" {
    #include<stdio.h>
}

#include <fstream>
#include <iostream>
#include <chrono>
#include "tick1.hpp"
//leave an empty line under the include below
#include "../../include/trx.hpp"


#define DEBUG               true
#define COMPLETED           true
#define PROOF_BY_MESSAGE    true

static void n_trace(long n, void* obj) {if (DEBUG) std::cout << "[TRACE] " << n << "\n" << std::flush;}
static void c_trace(void* obj)      {if (COMPLETED) std::cout << "[TRACE] seq > completed\n" << std::flush;  }

int main(){
  struct { long v; long total; std::array<long,1> totals ={0};} c;
  c.v= 0;
  c.total = 0;
  c.totals[0] = 0;
  std::array<long,1> totals2 ={0};

  auto start_time = std::chrono::high_resolution_clock::now();
  {
      auto c0= &c;

#define SEQUENCE
    sequence seq16 = [c0] => fromPublisher<unsigned> (tick1)
          do     x => std::cout << "_n = " << x                 << std::flush
          scan c0->total, (acc,n) => { acc += n;  return acc;  }, acc => acc
          do     acc => std::cout << ", _acc = " << acc         << std::flush
          scan c0->totals[0], (acc,n) => { acc += n;  return acc;  }, acc => acc
          do     acc => std::cout << ", _acc2 = " << acc         << std::flush
          scan [totals2], (acc,n) => { acc += n;  return acc;  }, acc => acc
          do     acc => std::cout << ", accs = " << acc << "\n" << std::flush
          select x => x
          select x => { return x *2 ;}
          select x => { return { c =  x +2}; }
          select x => x.c
          select x => (long)x + 2
          select x => struct { a = x, b = 1}
          selectmany 3, [x] => fromCapture()
                select y => y.a + 3
                select y => y*x.b
                endObservable
          select x => (x + 2)
          endObservable
          ;
#undef SEQUENCE

      seq16.subscribe(&n_trace, &c_trace, nullptr);
      if (DEBUG) std::cout << "\nready to go, enable: \n" << std::flush;
      seq16.enable(c0);

      for(int j=0; j<1; j++){
        for(int i=0; i<100; i++){
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


  if (PROOF_BY_MESSAGE) std::cout   << "\ndone in time: \n"
                        << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " microsecs\n\n"
                        << std::flush;
}
