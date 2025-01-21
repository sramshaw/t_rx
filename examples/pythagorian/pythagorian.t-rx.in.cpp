extern "C" {
    #include<stdio.h>
}

#include <fstream>
#include <iostream>
#include <chrono>
#include <tuple>
#include "../../include/trx.hpp"

#define DEBUG               false 
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
void test_classic(long &d, std::chrono::_V2::system_clock::time_point &start_time);
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

long test_classic( )
{
    auto d = 0L;
    auto d0 = &d;
    { // same code direct with for loops
        auto count = 0L;
        auto tracer = [](unsigned x)
        { n_trace(x, nullptr); };
        auto ender = []()
        { c_trace(nullptr); };
        bool continued = true;
        for (unsigned z = 1; z < 1000 && continued; z++)
            for (unsigned x = 1; x <= z && continued; x++)
                for (unsigned y = x; y <= z && continued; y++)
                {
                    (*d0)++;
                    if (x * x + y * y == z * z)
                    {
                        std::cout   << " values: xx: " << x << ", yy:" << y << ", zz:" << z << '\n'
                                     ;
                        count++;
                        if (count >= 100)
                            continued = false;
                        (*tracer)(z);
                    }
                }
        (*ender)();
    }
    std::cout<<"";
    return d;
}

unsigned long test_rx();

auto run_test_classic()
{
  auto start_time = std::chrono::high_resolution_clock::now();
  unsigned long d ;
  for (auto i =0; i < 100 && !DEBUG; i++)
  {
      d = test_classic();
      std::cout<<"\n" << std::flush;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration2 = end_time - start_time;
  std::cout<<"\n\n" << std::flush;
  return std::make_tuple(duration2, d);
}

auto run_test_rx()
{
  auto start_time = std::chrono::high_resolution_clock::now();
  unsigned long d ;
  for (auto i =0; i < 100 && !DEBUG; i++)
  {
      d = test_rx();
      std::cout<<"\n" << std::flush;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration2 = end_time - start_time;
  return std::make_tuple(duration2, d);
}

// placed here due to #define DEBUG conflict between generated code and the definition above. There is an issue using defines in the code below at the moment (breaks the build).
unsigned long test_rx()
{
    unsigned long c = 0;
    auto c0= &c;
    sequence seq14 = [c0] => fromRange<unsigned>(1,999)
        select z1 => struct {z = z1, c = c0}
        selectmany 1, [azc] => fromRange<unsigned>(1, azc.z)
                select x => struct {z = azc.z, x = x, c = azc.c}
                selectmany 1, [azcx] => fromRange<unsigned>(azcx.x, azcx.z)
                    where  y => { (*azcx.c)++; return azcx.x*azcx.x + y*y == azcx.z*azcx.z; }
                    select y => struct {x = azcx.x, y =y , z= azcx.z}
                    do     a =>  std::cout << " values: xx: " << a.x << ", yy:" <<a.y << ", zz:" << a.z <<'\n'
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
    return c;
}

void test_generated(){
  if (DEBUG) std::cout  << "\nlibrary warm up, to make it as fair as possible: "
                        << 123;
  std::cout  << std::flush;
  auto result_classic = run_test_classic();
  std::cout  << std::flush;
  auto result_rx      = run_test_rx();
  if (true) std::cout  << "\nTRx results"
                        << "\n------------"
                        << "\nnumber of elements traversed: "
                        << std::get<1>(result_rx)
                        <<" in "
                        << std::chrono::duration_cast<std::chrono::microseconds>(std::get<0>(result_rx)).count() << " microsecs\n\n"
                        <<"\nstandard coding approach results:"
                        <<"\n---------------------------------"
                        << "\nnumber of elements traversed: "
                        << std::get<1>(result_classic)
                        <<" in "
                        << std::chrono::duration_cast<std::chrono::microseconds>(std::get<0>(result_classic)).count() << " microsecs\n\n"
                        <<"\nRxCpp approach results:"
                        <<"\n---------------------------------"
                        << "\nnumber of elements traversed: "
                        << "?"
                        <<" in 3071946 microsecs\n\n"
                        << std::flush;
}