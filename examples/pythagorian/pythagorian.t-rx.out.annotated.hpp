#include <functional>
#include <array>

// START lambdas zone
// from interior to exterior
template<typename _c0, typename type0> bool seq14_2_2_fs1 (_c0 azcx, type0 y){ (*azcx.c)++; return azcx.x*azcx.x + y*y == azcx.z*azcx.z; }
template<typename _c0, typename type1> auto seq14_2_2_fs2 (_c0 azcx, type1 y){ {typedef decltype(azcx.x) anon_type0;
typedef decltype(y ) anon_type1;
typedef decltype(azcx.z) anon_type2;struct {
  anon_type0 x;
  anon_type1 y;
  anon_type2 z;
} __rx_ret;
__rx_ret.x=azcx.x;
__rx_ret.y=y ;
__rx_ret.z=azcx.z;
return __rx_ret;
};}
template<typename _c0, typename type2> void seq14_2_2_fs3 (_c0 azcx, type2 a){ std::cout << " values: xx: " << a.x << ", yy:" <<a.y << ", zz:" << a.z <<'\n';}
template<typename _c0, typename type0> auto seq14_2_fs1 (_c0 azc, type0 x){ {typedef decltype(azc.z) anon_type0;
typedef decltype(x) anon_type1;
typedef decltype(azc.c) anon_type2;struct {
  anon_type0 z;
  anon_type1 x;
  anon_type2 c;
} __rx_ret;
__rx_ret.z=azc.z;
__rx_ret.x=x;
__rx_ret.c=azc.c;
return __rx_ret;
};}

template<typename _c0, typename type0> auto seq14_fs1 (_c0 c0, type0 z1){ {typedef decltype(z1) anon_type0;
typedef decltype(c0) anon_type1;struct {
  anon_type0 z;
  anon_type1 c;
} __rx_ret;
__rx_ret.z=z1;
__rx_ret.c=c0;
return __rx_ret;
};}

template<typename _c0, typename type2> auto seq14_fs3 (_c0 c0, type2 a){return a.z;}
// END lambdas zone

// START observable zone
// from interior to exterior
template<typename _c0, typename type0,typename type1,typename type2,typename type3,typename type4>
class seq14_2_2:public observable<type4>{
public:
  _c0 azcx;
  typedef type4 output;
  bool enable(_c0 a0) {this->enabled = true; azcx=a0;
   from_connect   (&transform_next0, &transform_complete0, this); return true;}
  void disable()      {
    //std::cout<<"disable seq14_2_2\n"<<std::flush;
    from_disconnect(&transform_next0, &transform_complete0, this);
    this->enabled = false;
  }
  
  seq14_2_2() {  }
  ~seq14_2_2(){disable();}
  union _exchange_type {
    type0 type_0;
    type1 type_1;
    type2 type_2;
    type3 type_3;
    type4 type_4;
  };
  _exchange_type _exchange; 

  inline void from_connect   (void (*n)(unsigned,void*), void(*c)(void*), void* self){for (unsigned i = azcx.x;i<= azcx.z && this->enabled;i++){_exchange.type_0 = i;transform_next1(); }transform_complete1();}
  inline void from_disconnect(void (*n)(unsigned,void*), void(*c)(void*), void* self){}
  static void transform_next0(type0 v0, void *obj) {}
  static void transform_complete0(void *obj)       {}

  inline void transform_next1()     { if (seq14_2_2_fs1(azcx,_exchange.type_0)) transform_next2(); }
  inline void transform_complete1() { transform_complete2(); }
  inline void transform_next2()     { _exchange.type_2 = seq14_2_2_fs2(azcx,_exchange.type_1); transform_next3(); }
  inline void transform_complete2() { transform_complete3(); }
  inline void transform_next3()     { seq14_2_2_fs3(azcx,_exchange.type_2); transform_next4(); }
  inline void transform_complete3() { transform_complete4(); }
  inline void transform_next4()     { transform_next5(); }
  inline void transform_complete4() { transform_complete5(); }
  inline void transform_next5    () { this->direct_exit_next(_exchange.type_4); }
  inline void transform_complete5() {    disable();    this->direct_exit_complete();   }
};

template<typename _c0, typename type0,typename type1,typename type2,typename type3,typename manager2>
class seq14_2:public observable<type3>{
public:
  _c0 azc;
  typedef type3 output;
  bool enable(_c0 a0) {this->enabled = true; azc=a0;
   from_connect   (&transform_next0, &transform_complete0, this); return true;}
  void disable()      {
    //std::cout<<"disable seq14_2\n"<<std::flush;
    from_disconnect(&transform_next0, &transform_complete0, this);
    this->enabled = false;m2.unbind();
  }
  manager2& m2;
  seq14_2(manager2& a2) :m2(a2){init_higher_order2();  }
  ~seq14_2(){disable();}
  union _exchange_type {
    type0 type_0;
    type1 type_1;
    type2 type_2;
    type3 type_3;
  };
  _exchange_type _exchange; 

  inline void from_connect   (void (*n)(unsigned,void*), void(*c)(void*), void* self){for (unsigned i = 1;i<= azc.z && this->enabled;i++){_exchange.type_0 = i;transform_next1(); }transform_complete1();}
  inline void from_disconnect(void (*n)(unsigned,void*), void(*c)(void*), void* self){}
  static void transform_next0(type0 v0, void *obj) {}
  static void transform_complete0(void *obj)       {}

  inline void transform_next1()     { _exchange.type_1 = seq14_2_fs1(azc,_exchange.type_0); transform_next2(); }
  inline void transform_complete1() { transform_complete2(); }
    decltype(1) counter2;
  bool bool2;
  inline void transform_next2()     {bool2=false; bool success = m2.assign_work(_exchange.type_1, this);if (success) counter2++; /*std::cout<<"seq14_2"<<" count up "<<counter2<<"\n"<<std::flush;*/}
  inline void transform_complete2_() { counter2--;  /*std::cout<<"seq14_2"<<" count down "<<counter2<<"\n"<<std::flush; */ if(!counter2<=0 || !bool2) return; /*m2.unbind();*/   transform_complete3();}
  inline void transform_complete2() { bool2 = true;if(!counter2<=0) return; /*m2.unbind();*/   transform_complete3();}
  static void receive_next2(type2 v2, void *obj) {    //std::cout << "receive next2 " << v2 << "\n" << std::flush;
    auto self = static_cast<seq14_2 *>(obj);    self->_exchange.type_2 = v2;    self->transform_next3();  }
  static void receive_complete2(void *obj) {    auto self = static_cast<seq14_2 *>(obj);self->transform_complete2_();}
  inline void init_higher_order2()   { counter2=0; m2.bind(&receive_next2, &receive_complete2, this);}
  inline void transform_next3()     { transform_next4(); }
  inline void transform_complete3() { transform_complete4(); }
  inline void transform_next4    () { this->direct_exit_next(_exchange.type_3); }
  inline void transform_complete4() {    disable();    this->direct_exit_complete();   }
};

template<typename _c0, typename type0,typename type1,typename type2,typename type3,typename type4,typename manager2>
class seq14:public observable<type4>{
public:
  _c0 c0;
  typedef type4 output;
  bool enable(_c0 a0) {this->enabled = true; c0=a0;
   from_connect   (&transform_next0, &transform_complete0, this); return true;}
  void disable()      {
    //std::cout<<"disable seq14\n"<<std::flush;
    from_disconnect(&transform_next0, &transform_complete0, this);
    this->enabled = false;m2.unbind();
  }
  manager2& m2;
  seq14(manager2& a2) :m2(a2){init_higher_order2();  }
  ~seq14(){disable();}
  union _exchange_type {
    type0 type_0;
    type1 type_1;
    type2 type_2;
    type3 type_3;
    type4 type_4;
  };
  _exchange_type _exchange; 

  inline void from_connect   (void (*n)(unsigned,void*), void(*c)(void*), void* self){for (unsigned i = 1;i<=999 && this->enabled;i++){_exchange.type_0 = i;transform_next1(); }transform_complete1();}
  inline void from_disconnect(void (*n)(unsigned,void*), void(*c)(void*), void* self){}
  static void transform_next0(type0 v0, void *obj) {}
  static void transform_complete0(void *obj)       {}

  inline void transform_next1()     { _exchange.type_1 = seq14_fs1(c0,_exchange.type_0); transform_next2(); }
  inline void transform_complete1() { transform_complete2(); }
    decltype(1) counter2;
  bool bool2;
  inline void transform_next2()     {bool2=false; bool success = m2.assign_work(_exchange.type_1, this);if (success) counter2++; /*std::cout<<"seq14"<<" count up "<<counter2<<"\n"<<std::flush;*/}
  inline void transform_complete2_() { counter2--;  /*std::cout<<"seq14"<<" count down "<<counter2<<"\n"<<std::flush; */ if(!counter2<=0 || !bool2) return; /*m2.unbind();*/   transform_complete3();}
  inline void transform_complete2() { bool2 = true;if(!counter2<=0) return; /*m2.unbind();*/   transform_complete3();}
  static void receive_next2(type2 v2, void *obj) {    //std::cout << "receive next2 " << v2 << "\n" << std::flush;
    auto self = static_cast<seq14 *>(obj);    self->_exchange.type_2 = v2;    self->transform_next3();  }
  static void receive_complete2(void *obj) {    auto self = static_cast<seq14 *>(obj);self->transform_complete2_();}
  inline void init_higher_order2()   { counter2=0; m2.bind(&receive_next2, &receive_complete2, this);}
  inline void transform_next3()     { _exchange.type_3 = seq14_fs3(c0,_exchange.type_2); transform_next4(); }
  inline void transform_complete3() { transform_complete4(); }
    decltype( 100) counter4 = 0;
inline void transform_next4()     { counter4++; transform_next5(); if (counter4 >=  100) {transform_complete5(); std::cout<<"was here"; }}
  inline void transform_complete4() { transform_complete5(); }
  inline void transform_next5    () { this->direct_exit_next(_exchange.type_4); }
  inline void transform_complete5() {    disable();    this->direct_exit_complete();   }
};
// END observable zone

// START stack declations
// from interior to exterior
 #define T_RX_CREATE_seq14() \
\
/* seq14_2_sequence2_array */ \
typedef unsigned seq14_type0;\
typedef decltype(seq14_fs1<decltype(c0),seq14_type0>(decltype(c0){},seq14_type0{})) seq14_type1;\
typedef unsigned seq14_2_type0;\
typedef decltype(seq14_2_fs1<seq14_type1,seq14_2_type0>(seq14_type1{},seq14_2_type0{})) seq14_2_type1;\
typedef unsigned seq14_2_2_type0;\
typedef seq14_2_2_type0 seq14_2_2_type1;\
typedef decltype(seq14_2_2_fs2<seq14_2_type1,seq14_2_2_type1>(seq14_2_type1{},seq14_2_2_type1{})) seq14_2_2_type2;\
typedef seq14_2_2_type2 seq14_2_2_type3;\
typedef seq14_2_2_type3 seq14_2_2_type4;\
typedef seq14_2_2<seq14_2_type1,seq14_2_2_type0,seq14_2_2_type1,seq14_2_2_type2,seq14_2_2_type3,seq14_2_2_type4> seq14_2_2_sub;\
std::array<seq14_2_2_sub,1> seq14_2_sequence2_array={seq14_2_2_sub()};\
/* seq14_2_manager2 */ \
typedef high_order_manager<decltype(seq14_2_sequence2_array)&,seq14_2_type1,seq14_2_2_sub::output> seq14_2_manager2_type;\
seq14_2_manager2_type seq14_2_manager2(seq14_2_sequence2_array);\
/* seq14_sequence2_array */ \
typedef seq14_2_2_sub::output seq14_2_type2;\
typedef seq14_2_type2 seq14_2_type3;\
typedef seq14_2<seq14_type1,seq14_2_type0,seq14_2_type1,seq14_2_type2,seq14_2_type3,seq14_2_manager2_type> seq14_2_sub;\
std::array<seq14_2_sub,1> seq14_sequence2_array={seq14_2_sub(seq14_2_manager2)};\
/* seq14_manager2 */ \
typedef high_order_manager<decltype(seq14_sequence2_array)&,seq14_type1,seq14_2_sub::output> seq14_manager2_type;\
seq14_manager2_type seq14_manager2(seq14_sequence2_array);\
/* seq14_sub */ \
typedef seq14_2_sub::output seq14_type2;\
typedef decltype(seq14_fs3<decltype(c0),seq14_type2>(decltype(c0){},seq14_type2{})) seq14_type3;\
typedef seq14_type3 seq14_type4;\
typedef seq14<decltype(c0),seq14_type0,seq14_type1,seq14_type2,seq14_type3,seq14_type4,seq14_manager2_type> seq14_sub;\
  auto seq14 = seq14_sub(seq14_manager2);\
// END stack declations
 
