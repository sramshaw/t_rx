#include <iostream>
#include <array>
#include "trx.hpp"
//#define DEBUG true
//////////////////////// TEST's SOURCE 0
// 
// friendly source allowing to register and unregister
//

// API to stimulate
static void (* source0_callback)(unsigned);
static void (* source0_done)();


/**/
///   THIS IS THE MOST COMPACT CODE FOR g++, 
const int client_max = 5; // 10;

// internals
static struct { 
    void(*next      [client_max])(unsigned,void*);
    void(*complete  [client_max])(void*);
    void* obj       [client_max];
    bool  enabled   [client_max];
    const int size = client_max;
} source0_clients;

static void source0_propagate_next(unsigned val){
    for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.enabled[i]) {
            //std::cout << "push one\n" << std::flush;
            (*source0_clients.next[i])(val,source0_clients.obj[i]);
        }
    }
}
static void source0_propagate_complete(){
    for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.enabled[i]) {
            (*source0_clients.complete[i])(source0_clients.obj[i]);
        }
    }
}

static void source0_subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
     for(int i =0; i<source0_clients.size; i++){
        if (!source0_clients.enabled[i]) {
            source0_clients.next[i]      = next;
            source0_clients.complete[i]  = complete;
            source0_clients.obj[i]       = obj;
            source0_clients.enabled[i]   = true;
            source0_callback= &source0_propagate_next;
            source0_done    = &source0_propagate_complete;
//            if (DEBUG) std::cout << "[SOURCE] 0 > registered client " << (i+1) << "\n" << std::flush;
            return;
        }
    }
//    if (DEBUG) std::cout << "[SOURCE] 0 > source0 could not register a client\n" << std::flush;
}
static void source0_stop(void* obj){
     for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.obj[i] ==obj) {
            source0_clients.enabled[i] = false;
            //if (DEBUG) std::cout << "[SOURCE] 0 > source0 unregistered client " <<  (i+1) << "\n" << std::flush;
        }
    }
}
static void source0_dispose(void* obj){
     for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.obj[i] ==obj) {
            source0_clients.enabled[i] = false;
            //if (DEBUG) std::cout << "[SOURCE] 0 > source0 unregistered client " <<  (i+1) << "\n" << std::flush;
        }
    }
}


/**/

/*
////   PROBABLY SECOND PLACE
const int client_max = 2; // 10;

static struct { 
    void(*next      [client_max])(unsigned,void*);
    void(*complete  [client_max])(void*);
    void* obj       [client_max];
    bool enabled    [client_max];
    const int size =client_max;
} source0_clients;


class source0 {
    public:
// internals
static void source0_propagate_next(unsigned val){
    for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.enabled[i]) {
            (*source0_clients.next[i])(val,source0_clients.obj[i]);
        }
    }
}
static void source0_propagate_complete(){
    for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.enabled[i]) {
            (*source0_clients.complete[i])(source0_clients.obj[i]);
        }
    }
}

static void subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
     for(int i =0; i<source0_clients.size; i++){
        if (!source0_clients.enabled[i]) {
            source0_clients.next[i]      = next;
            source0_clients.complete[i]  = complete;
            source0_clients.obj[i]       = obj;
            source0_clients.enabled[i]   = true;
            source0_callback= &source0_propagate_next;
            source0_done    = &source0_propagate_complete;
            if (DEBUG) std::cout << "[SOURCE] 0 > registered client " << (i+1) << "\n" << std::flush;
            return;
        }
    }
    if (DEBUG) std::cout << "[SOURCE] 0 > source0 could not register a client\n" << std::flush;
}
static void stop(void* obj){
     for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.obj[i] ==obj) {
            source0_clients.enabled[i] = false;
            if (DEBUG) std::cout << "[SOURCE] 0 > source0 unregistered client " <<  (i+1) << "\n" << std::flush;
        }
    }
}
};


/*
*/
/*

/// THIRD PLACE
const int client_max = 2; // 10;

inline static void _source0_subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) ;

class source0 {
 struct { 
    void(*next      [client_max])(unsigned,void*);
    void(*complete  [client_max])(void*);
    void* obj       [client_max];
    bool enabled    [client_max];
    const int size =client_max;
} source0_clients;

public:
    static void onnext(unsigned val,void* me){
        auto self = static_cast<source0*> (me);
     for(int i =0; i<self->source0_clients.size; i++){
        if (self->source0_clients.enabled[i]) {
            (*self->source0_clients.next[i])(val,self->source0_clients.obj[i]);
        }
    }
}

    static void oncomplete(void* me){
        auto self = static_cast<source0*> (me);
     for(int i =0; i<self->source0_clients.size; i++){
        if (self->source0_clients.enabled[i]) {
            (*self->source0_clients.complete[i])(self->source0_clients.obj[i]);
        }
    }
}
    
    void subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
      for(int i =0; i<source0_clients.size; i++){
        if (!source0_clients.enabled[i]) {
            source0_clients.next[i]      = next;
            source0_clients.complete[i]  = complete;
            source0_clients.obj[i]       = obj;
            source0_clients.enabled[i]   = true;
            _source0_subscribe( &onnext, &oncomplete,this);
            if (DEBUG) std::cout << "[SOURCE] 0 > registered client " << (i+1) << "\n" << std::flush;
            return;
        }
    }
    if (DEBUG) std::cout << "[SOURCE] 0 > source0 could not register a client\n" << std::flush;
} 

    void dispose(void* obj){
      for(int i =0; i<source0_clients.size; i++){
        if (source0_clients.obj[i] ==obj) {
            source0_clients.enabled[i] = false;
            if (DEBUG) std::cout << "[SOURCE] 0 > source0 unregistered client " <<  (i+1) << "\n" << std::flush;
        }
    }
} 
};

static source0 s0;
typedef source0 s0_t;
inline static void _s0_next(unsigned val){
    s0_t::onnext(val,&s0);  
}
inline static void _s0_complete(){
    s0_t::oncomplete(&s0);  
}
inline static void _source0_subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
            source0_callback= &_s0_next;
            source0_done    = &_s0_complete;
}

inline static void source0_subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
    s0.subscribe(next,complete,obj);
}
inline static void source0_stop(void* obj){
    s0.dispose(obj);
}

/*
 
/// FOURTH PLACE, SURPRISING: USE OF struct MAKES DATA BIGGER AS OPPOSED TO ARRAYS OF INTRINSICS
struct client {
    void(*next      )(unsigned,void*);
    void(*complete  )(void*);
    void* obj       ;
    bool enabled    ;
//    client(): enabled(false){}
};

template<typename repo_t>
class source0 {
    repo_t repo;

public:
    source0(repo_t o): repo(o) {}

    static void onnext(unsigned val,void* me){
        auto self = static_cast<source0*> (me);
        for(auto& element: self->repo){
            if (element.enabled) {
                (*element.next)(val,element.obj);
            }
        }
    }

    static void oncomplete(void* me){
        auto self = static_cast<source0*> (me);
        for(auto& element: self->repo){
            if (element.enabled) {
                (*element.complete)(element.obj);
            }
        }
    }
    
    void subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
        auto i =0;
         for(auto& element: repo){
            if (!element.enabled) {
                element.next      = next;
                element.complete  = complete;
                element.obj       = obj;
                element.enabled   = true;
                _source0_subscribe( &onnext, &oncomplete,this);
                if (DEBUG) std::cout << "[SOURCE] 0 > registered client " << (i+1) << "\n" << std::flush;
                return;
            }
            i++;
        }
        if (DEBUG) std::cout << "[SOURCE] 0 > source0 could not register a client\n" << std::flush;
    }

    void dispose(void* obj){
        auto i= 0;
         for(auto& element: repo){
            if (element.obj ==obj ){//&& element.enabled) {
                element.enabled = false;
                if (DEBUG) std::cout << "[SOURCE] 0 > source0 unregistered client " <<  (i+1) << "\n" << std::flush;
            }
            i++;
        }
    }
};

const int client_max = 2; // 10;
static std::array<client,client_max> s0_repo;

static source0<decltype(s0_repo)&> s0(s0_repo);
typedef decltype(s0) s0_t;

inline static void _s0_next(unsigned val){
    s0_t::onnext(val,&s0);  
}
inline static void _s0_complete(){
    s0_t::oncomplete(&s0);  
}
inline static void _source0_subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
            source0_callback= &_s0_next;
            source0_done    = &_s0_complete;
}

inline static void source0_subscribe(void(*next)(unsigned,void*),void(*complete)(void*), void* obj) {
    s0.subscribe(next,complete,obj);
}
inline static void source0_stop(void* obj){
    s0.dispose(obj);
}
*/
