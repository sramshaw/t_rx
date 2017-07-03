extern "C" {
    #include<stdio.h>
}

#include <fstream>
#include <iostream>
#include <chrono>
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
static void n_trace(t n, void* obj) {/*if (DEBUG) std::cout << "[TRACE] " << n << "\n" << std::flush;*/}
static void c_trace(void* obj)          {if (DEBUG) std::cout << "[TRACE] seq > completed\n" << std::flush;  }

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
        //source0_subscribe(n_trace,c_trace,nullptr);
        //(*source0_callback)(1);
        //source0_stop(nullptr);
    }
}

void test_generated(){
  unsigned long c = 0;
  if (DEBUG) std::cout  << "\nlibrary warm up, to make it as fair as possible: "
                        << c;

  auto start_time = std::chrono::high_resolution_clock::now();
  {
      auto c0= &c;
      sequence seq14 = [c0] => fromRange<unsigned>(1,999)
          select z1 => struct {z = z1, c = c0}
          selectmany 1, [azc] => fromRange<unsigned>(1, azc.z)
                  select x => struct {z = azc.z, x = x, c = azc.c}
                  selectmany 1, [azcx] => fromRange<unsigned>(azcx.x, azcx.z)
                        where  y => { (*azcx.c)++; return azcx.x*azcx.x + y*y == azcx.z*azcx.z; }
                        select y => struct {x = azcx.x, y =y , z= azcx.z}
                        do     a => std::cout << " values: xx: " << a.x << ", yy:" <<a.y << ", zz:" << a.z <<'\n'
                        endObservable
                  endObservable
          select a => a.z
          take 100
          endObservable
          ;

      seq14.subscribe(&n_trace, &c_trace, nullptr);
      if (DEBUG) std::cout << "\nready to go, enable: \n" << std::flush;
      seq14.enable(c0);
      seq14.disable();
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto time = end_time - start_time;

  auto d  = 0L;
  auto d0 = &d;
  {  // same code direct with for loops
      start_time = std::chrono::high_resolution_clock::now();
      auto count=0L;
      auto tracer = [](unsigned x){n_trace(x,nullptr);};
      auto ender  = [](){c_trace(nullptr);};
      bool continued = true;
      for (unsigned z=1; z<1000 && continued; z++)
        for (unsigned x = 1; x <= z && continued; x++)
            for (unsigned y = x; y <= z && continued; y++) {
                (*d0)++;
                if (x*x+y*y==z*z){
                    std::cout << " values: xx: " << x << ", yy:" <<y << ", zz:" << z <<'\n';
                    count++;
                    if (count >=100)
                        continued = false;
                    (*tracer)(z);
                }
            }
      (*ender)();
  }
  end_time = std::chrono::high_resolution_clock::now();
  auto time2 = end_time - start_time;


  if (true) std::cout  << "\nTRx results"
                        << "\n------------"
                        << "\nnumber of elements traversed: "
                        << c
                        <<" in "
                        << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " microsecs\n\n"
                        <<"\nstandard coding approach results:"
                        <<"\n---------------------------------"
                        << "\nnumber of elements traversed: "
                        << d
                        <<" in "
                        << std::chrono::duration_cast<std::chrono::microseconds>(time2).count() << " microsecs\n\n"
                        <<"\nRxCpp approach results:"
                        <<"\n---------------------------------"
                        << "\nnumber of elements traversed: "
                        << d
                        <<" in 3071946 microsecs\n\n"
                        << std::flush;
}
