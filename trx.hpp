#pragma once

#include <iostream>


class rx_usable {
public: //observable
    bool enabled;
    rx_usable(): enabled(false){}    
};

template<typename typeN>
class observable: public rx_usable {  
    public: //observable
//    bool enabled;
    void (*exit_next)(typeN,void*);
    void (*exit_complete)(void*);
    void* exit_obj;
    void subscribe(void(*n)(typeN,void*),void(*c)(void*),void* o) {
        exit_next     = n;
        exit_complete = c;
        exit_obj      = o;  
//        if (DEBUG) std::cout << "subscribe!!\n" << std::flush;
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
