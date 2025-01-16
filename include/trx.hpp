#pragma once

#include <iostream>


class rx_usable {
public: //observable
    bool enabled;
    rx_usable(): enabled(false){}    
};

template<typename typeN>
class observable: public rx_usable {  
    void (*exit_next)(typeN,void*);
    void (*exit_complete)(void*);
    public:
    void* exit_obj;
    void subscribe(void(*n)(typeN,void*),void(*c)(void*),void* o) {
        exit_next     = n;
        exit_complete = c;
        exit_obj      = o;  
//        if (DEBUG) std::cout << "subscribe!!\n" << std::flush;
    }
    inline void direct_exit_next(typeN d) {
        (*exit_next)(d,exit_obj);    
    }
    inline void direct_exit_complete() {
        (*exit_complete)(exit_obj);    
    }
};
template<typename repo_t, typename origin,typename witness>
class high_order_manager {
private:
    repo_t   repo;
public:
    bool assign_work(origin x, void* caller) {
        auto i =0;
        for(auto& element: repo) {
            if (!element.enabled) {
                element.exit_obj=caller;
                //std::cout << "assign work @"<<i<<" \n" << std::flush;
                return element.enable(x);
            }
            i++;
        }
        //if (DEBUG) std::cout << "FAIL STARTING subseq for value of x: " /*<< x*/ << " \n" << std::flush;
        return false;
    };
    void disableFrom(void* caller) {
        for(auto& element: repo) {
            if (element.exit_obj==caller) {
                return element.disable();
            }
        }
    };
    void unbind () {
        for(auto& element: repo)
            if (element.enabled)   element.disable();
    };
    void bind(void (*n)(witness,void*), void(*c)(void*), void* self) {
        //if (DEBUG) std::cout << "binding seqs!! \n" << std::flush;
        for(auto& element: repo)
            element.subscribe(n,c,self);
    };
    high_order_manager(repo_t r): repo(r){} 
};


const int publish_max = 5; // 10;

#define DEBUG               false
template<typename typeN>
class publishedObservable : public rx_usable {
 struct { 
    void(*next      [publish_max])(typeN,void*);
    void(*complete  [publish_max])(void*);
    void* obj       [publish_max];
    bool enabled    [publish_max];
    const int size =publish_max;
} _clients;

public:
//clients_t<typeN> _clients;

    inline void direct_exit_next(typeN val) {
       for(int i =0; i<_clients.size; i++){
          if (_clients.enabled[i]) {
              (*_clients.next[i])(val,_clients.obj[i]);
          }
        }
        if (DEBUG) std::cout << "[SOURCE] 0 > next val " << (val) << "\n" << std::flush;
    }
    inline void direct_exit_complete() {
     for(int i =0; i<_clients.size; i++){
        if (_clients.enabled[i]) {
            (*_clients.complete[i])(_clients.obj[i]);
        }
      }
    }
    
    void subscribe(void(*next)(typeN,void*),void(*complete)(void*), void* obj) {
      for(int i =0; i<_clients.size; i++){
        if (!_clients.enabled[i]) {
            _clients.next[i]      = next;
            _clients.complete[i]  = complete;
            _clients.obj[i]       = obj;
            _clients.enabled[i]   = true;
            //this->exit_object   = this;
            //_source0_subscribe( &onnext, &oncomplete,this);
            if (DEBUG) std::cout << "[SOURCE] 0 > registered client " << (i+1) << "\n" << std::flush;
            return;
        }
      }
      if (DEBUG) std::cout << "[SOURCE] 0 > source0 could not register a client\n" << std::flush;
    } 

    void dispose(void* obj){
      for(int i =0; i<_clients.size; i++){
        if (_clients.obj[i] ==obj) {
            _clients.enabled[i] = false;
            if (DEBUG) std::cout << "[SOURCE] 0 > source0 unregistered client " <<  (i+1) << "\n" << std::flush;
        }
      }
    } 
};


