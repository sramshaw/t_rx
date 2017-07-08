import re

def getEntity(op,name,captype,capture):
    invoker = op[4][5]
    id = op[0]                 #  rank index
    a = re.search(invoker[1],op[4][3]).group
    b = re.search(invoker[2],op[4][4]).group
    ret = eval(invoker[3])
    return ret

def create_class(name,capture,f_ops):
    s = len(f_ops)
    ret = "\n".join([create_class(name+ "_" + str(i),o.capture,o.ops) for i,o in enumerate(f_ops) if hasattr(o,"ops")])
    ret = ret + ('\ntemplate<typename _c0, ' + ",".join(
                ["typename type" + str(i) for i in range(0,s)]
              + ["typename " + (o.internal_dcl + str(i)) for i,o in enumerate(f_ops) if hasattr(o,'internal_dcl')]
              ) + ">\n")
    ret =(ret + 'class ' + name + ':public observable<type' + str(s-1) + '>{\npublic:\n  _c0 ' + capture +';'
              + '\n  typedef type' + str(s-1) + " output;"
              + "\n  bool enable(_c0 a0) {this->enabled = true; "+capture+"=a0;" 
              + "\n  ".join([ o.internal_enable for i,o in enumerate(f_ops) if hasattr(o,'internal_enable')])
              + "\n   from_connect   (&transform_next0, &transform_complete0, this); return true;}"
              + "\n  void disable()      {"
              + "\n    //std::cout<<\"disable " + name + "\\n\"<<std::flush;"
              + "\n    from_disconnect(&transform_next0, &transform_complete0, this);"
              + "\n    this->enabled = false;"
              + "\n    ".join([o.ondisable for o in f_ops if hasattr(o,'ondisable')])
              + "\n  }"
              + "\n  "+ "\n  ".join([(o.internal_dcl + str(i) + "& " + o.internal_dcl[0] + str(i) + ";") for i,o in enumerate(f_ops) if hasattr(o,'internal_dcl')])
              + "\n  " + name + "("
              + ",".join([  o.internal_dcl + str(i) + "& a" + str(i) for i,o in enumerate(f_ops) if hasattr(o,'internal_dcl')])
              + ") "+ (":" if any([hasattr(o,'internal_dcl') for o in f_ops]) else "")
              + ",".join([  o.internal_dcl[0:1] + str(i) + "(a" + str(i) + ")" for i,o in enumerate(f_ops) if hasattr(o,'internal_dcl')])
              + "{"
              + "\n  ".join([ o.internal_setup for i,o in enumerate(f_ops) if hasattr(o,'internal_setup')])
              + "  }\n"
              + "  ~" + name + "(){disable();}\n"
              + "  union _exchange_type {\n    " + "\n    ".join(["type"+str(i)+" type_"+str(i)+";" for i in range(0,s)]) + "\n  };"
              + "\n  _exchange_type _exchange; \n"
              )

    ret = ret + "\n"+ "\n  ".join([o.trampoline for o in f_ops if hasattr(o,'trampoline')])
    ret = (ret+ "\n" 
  "  inline void transform_next" + str(s) + "    () { this->direct_exit_next(_exchange.type_" + str(s-1) + "); }\n"
  "  inline void transform_complete" + str(s) + "() {"
  "    disable();"
  "    this->direct_exit_complete(); "
  "  }\n};\n")
    return ret

class fromStaticPublisher:
    def __init__(self,op,arg2,f1,f2,seq_name,captype,capture):
        print op[4][2]
        index = op[3]
        i       = index
        inci    = i + 1
        index   = index + 1
        self.external = ''
        self.common= (
  '  inline void from_connect   (void (*n)('+arg2+',void*), void(*c)(void*), void* self){'+f1+'(n,c,self);}\n'
  '  inline void from_disconnect(void (*n)('+arg2+',void*), void(*c)(void*), void* self){'+f2+'(    self);}\n'
  '  static void transform_next0(type%(i)d v%(i)d, void *obj) { '
  '    auto self = static_cast<'+ seq_name + '*>(obj);'
  '    self->_exchange.type_%(i)d = v%(i)d;'
  '    self->transform_next%(inci)d();'
  '  }\n'
  '  static void transform_complete0(void *obj)       {'
  '    auto self = static_cast<'+ seq_name + '*>(obj);'
  '    self->transform_complete%(inci)d();'
  '  }\n') %locals()
        self.straight=''
        self.trampoline = self.common
        self.outputType='typedef ' + arg2 + " " + seq_name + '_type' + str(i) + ';'
        self.index=i

class fromJust:
    def __init__(self, op, arg1, arg2, seq_name, captype, capture):
        print op[4][2]
        index = op[3]
        i       = index
        inci    = i + 1
        index   = index + 1
        self.external = ''
        self.common= (
  '  inline void from_connect   (void (*n)(type0,void*), void(*c)(void*), void* self){_exchange.type_%(i)d = (type0)('+arg2+');transform_next%(inci)d();transform_complete%(inci)d();}\n'
  '  inline void from_disconnect(void (*n)(type0,void*), void(*c)(void*), void* self){}\n'
  '  static void transform_next0(type%(i)d v%(i)d, void *obj) {}\n'
  '  static void transform_complete0(void *obj)       {}\n') %locals()
        self.straight=''
        self.trampoline = self.common
        self.outputType='typedef ' + arg1 + " " + seq_name + '_type' + str(i) + ';'
        self.index=i

class fromRange:
    def __init__(self, op, theType, arg1,arg2, seq_name, captype, capture):
        print op[4][2]
        index = op[3]
        i       = index
        inci    = i + 1
        index   = index + 1
        self.common= (
  '  inline void from_connect   (void (*n)('+theType+',void*), void(*c)(void*), void* self){'
     'for ('+theType+' i = '+arg1+';i<='+arg2+' && this->enabled;i++){'
     '_exchange.type_%(i)d = i;transform_next%(inci)d(); }transform_complete%(inci)d();}\n'
  '  inline void from_disconnect(void (*n)('+theType+',void*), void(*c)(void*), void* self){}\n'
  '  static void transform_next0(type%(i)d v%(i)d, void *obj) {}\n'
  '  static void transform_complete0(void *obj)       {}\n') %locals()
        self.straight=''
        self.trampoline = self.common
        self.outputType='typedef ' + theType + " " + seq_name + '_type' + str(i) + ';'
        self.index=i

def expand_anon_types(s):
    match =re.findall(r"(return)[\s\n\r]*\{[\n\s\r]*(.+)[\s\n\r]*}[/n/s]*;[\n\s\r]*",s,re.DOTALL)
    print match
    if match:
        t = match[0][1]
        u = re.findall(r"\s*(\w+)?\s*=\s*([^,]+)?,?\s*",t,re.DOTALL)
        ret0 = "\n".join(["typedef decltype(" + item[1] + ") anon_type"+ str(i) + ";" for i,item in enumerate(u)])
        ret1 = "struct {\n  "+ ";\n  ".join(["anon_type" + str(i) + " "+ item[0]for i,item in enumerate(u)]) + ";\n} __rx_ret;\n"
        #ret1 = "struct {\n  "+ ";\n  ".join(["decltype(" + item[1] + ") "+ item[0]for i,item in enumerate(u)]) + ";\n} __rx_ret;\n"
        ret2 = "\n".join(["__rx_ret." +item[0] +"="+ item[1]+ ";" for item in u])
        ret3 = "\nreturn __rx_ret;\n"
        ret = s.replace(match[0][0],"").replace(t,ret0+ret1+ret2+ret3)
        return ret
    return s

class scan:
    def __init__(self, op, name, captype,capture, init,acc,n, funcBody):
        index = op[3]
        transformed      = expand_anon_types(funcBody)
        transformed_init = expand_anon_types(init)
        print op[4][2]
        #print 'b4:\n' + funcBody+ '\n  after:\n'+ transformed
        i       = index
        deci    = i+1
        index   = index + 1
        funcName        = "%(name)s_fs%(i)d" %locals() 
        deci            = i-1
        inci            = i+1
        self.external   = ('template<typename _c0> auto %(funcName)s_init (_c0 %(capture)s)%(transformed_init)s\n' 
                           'template<typename _c0, typename type%(deci)d, typename type%(i)d> auto %(funcName)s (_c0 %(capture)s, type%(deci)d %(n)s,type%(i)d %(acc)s)%(transformed)s\n' 
                          )% locals()
        self.straight   =''
        self.trampoline =(
    'inline void transform_next%(i)d()     { %(acc)s = %(funcName)s(%(capture)s,_exchange.type_%(deci)d,%(acc)s); _exchange.type_%(i)d = %(acc)s; transform_next%(inci)d(); }\n'
  '  inline void transform_complete%(i)d() { transform_complete%(inci)d(); }\n'
  '  type%(i)d %(acc)s;\n'
  "  inline void init_scan%(i)d()   { %(acc)s = %(funcName)s_init (%(capture)s);}\n"
            ) %locals()
        self.outputType =('typedef decltype(%(funcName)s_init<%(captype)s>(%(captype)s{})) ' + name + '_type%(i)d;') %locals()
        self.internal_enable = 'init_scan' + str(i) + "();"
        self.index=i

class select:
    def __init__(self, op, name, captype,capture, var, funcBody):
        index = op[3]
        transformed = expand_anon_types(funcBody)
        print op[4][2]
        #print 'b4:\n' + funcBody+ '\n  after:\n'+ transformed
        i       = index
        deci    = i+1
        index   = index + 1
        funcName        = "%(name)s_fs%(i)d" %locals() 
        deci            = i-1
        inci            = i+1
        self.external   = 'template<typename _c0, typename type%(deci)d> auto %(funcName)s (_c0 %(capture)s, type%(deci)d %(var)s)%(transformed)s' % locals()
        self.straight   =''
        self.trampoline =(
    'inline void transform_next%(i)d()     { _exchange.type_%(i)d = %(funcName)s(%(capture)s,_exchange.type_%(deci)d); transform_next%(inci)d(); }\n'
  '  inline void transform_complete%(i)d() { transform_complete%(inci)d(); }'
            ) %locals()
        self.outputType =('typedef decltype(%(funcName)s<%(captype)s,' + name + '_type%(deci)d>(%(captype)s{},'+name+'_type%(deci)d{})) ' + name + '_type%(i)d;') %locals()
        self.index=i

class where:
    def __init__(self, op, name, captype,capture, var, funcBody):
        index = op[3]
        transformed = expand_anon_types(funcBody)
        print op[4][2]
        #print 'b4:\n' + funcBody+ '\n  after:\n'+ transformed
        i       = index
        deci    = i+1
        index   = index + 1
        funcName        = "%(name)s_fs%(i)d" %locals()
        deci            = i-1
        inci            = i+1
        self.external   = 'template<typename _c0, typename type%(deci)d> bool %(funcName)s (_c0 %(capture)s, type%(deci)d %(var)s)%(transformed)s' % locals()
        self.straight   =''
        self.trampoline =(
    'inline void transform_next%(i)d()     { if (%(funcName)s(%(capture)s,_exchange.type_%(deci)d)) transform_next%(inci)d(); }\n'
  '  inline void transform_complete%(i)d() { transform_complete%(inci)d(); }'
            ) %locals()
        self.outputType =('typedef ' + name + '_type%(deci)d ' + name + '_type%(i)d;') %locals()
        self.index=i

class do:
    def __init__(self, op, name, captype,capture, var, funcBody):
        index = op[3]
        transformed = expand_anon_types(funcBody)
        print op[4][2]
        #print 'b4:\n' + funcBody+ '\n  after:\n'+ transformed
        i       = index
        deci    = i+1
        index   = index + 1
        funcName        = "%(name)s_fs%(i)d" %locals()
        deci            = i-1
        inci            = i+1
        self.external   = 'template<typename _c0, typename type%(deci)d> void %(funcName)s (_c0 %(capture)s, type%(deci)d %(var)s)%(transformed)s' % locals()
        self.straight   =''
        self.trampoline =(
    'inline void transform_next%(i)d()     { %(funcName)s(%(capture)s,_exchange.type_%(deci)d); transform_next%(inci)d(); }\n'
  '  inline void transform_complete%(i)d() { transform_complete%(inci)d(); }'
            ) %locals()
        self.outputType =('typedef ' + name + '_type%(deci)d ' + name + '_type%(i)d;') %locals()
        self.index=i

class take:
    def __init__(self, op, name, captype,capture, count):
        index = op[3]
        print op[4][2]
        i       = index
        deci    = i+1
        index   = index + 1
        funcName        = "%(name)s_fs%(i)d" %locals()
        deci            = i-1
        inci            = i+1
        self.straight   =''
        self.trampoline =(
  "  decltype(" + count + ") counter%(i)d = 0;\n"
  'inline void transform_next%(i)d()     { counter%(i)d++; transform_next%(inci)d(); if (counter%(i)d >= ' + count + ') {transform_complete%(inci)d(); std::cout<<"was here"; }}\n'
  '  inline void transform_complete%(i)d() { transform_complete%(inci)d(); }'
            ) %locals()
        self.outputType =('typedef ' + name + '_type%(deci)d ' + name + '_type%(i)d;') %locals()
        self.index=i

class selectmany:
    def __init__(self, op, name, maxConcurrent, var): #, subseq):
        print op[4][2]
        index = op[3]
        i = index
        deci            = i - 1
        index = index +1
        subName         = "%(name)s_%(i)d" %locals() 
        index = 0
        self.ops = [getEntity(o,subName, name+"_type%(deci)d"%locals(),var) for o in op[5]]
        self.class_definition = create_class(name,var,self.ops)
        self.capture = var
        index = i + 1
        inci            = i + 1
        deci            = i - 1
        self.external   = ''
        self.ondisable = "m%(i)d.unbind();" %locals()
        self.trampoline =   (
  "  decltype(" + maxConcurrent + ") counter%(i)d;\n"
  "  bool bool%(i)d;\n"
  "  inline void transform_next%(i)d()     {bool%(i)d=false; bool success = m%(i)d.assign_work(_exchange.type_%(deci)d, this);if (success) counter%(i)d++; /*std::cout<<\""+name+"\"<<\" count up \"<<counter%(i)d<<\"\\n\"<<std::flush;*/}\n"
  "  inline void transform_complete%(i)d_() { counter%(i)d--;  /*std::cout<<\""+name+"\"<<\" count down \"<<counter%(i)d<<\"\\n\"<<std::flush; */ if(!counter%(i)d<=0 || !bool%(i)d) return; /*m%(i)d.unbind();*/   transform_complete%(inci)d();}\n"
  "  inline void transform_complete%(i)d() { bool%(i)d = true;if(!counter%(i)d<=0) return; /*m%(i)d.unbind();*/   transform_complete%(inci)d();}\n"
  "  static void receive_next%(i)d(type%(i)d v%(i)d, void *obj) {"
  "    //std::cout << \"receive next%(i)d \" << v%(i)d << \"\\n\" << std::flush;\n"
  "    auto self = static_cast<%(name)s *>(obj);"
  "    self->_exchange.type_%(i)d = v%(i)d;"
  "    self->transform_next%(inci)d();"
  "  }\n"
  "  static void receive_complete%(i)d(void *obj) {"
       "    auto self = static_cast<%(name)s *>(obj);"
   "self->transform_complete%(i)d_();}\n"
  "  inline void init_higher_order%(i)d()   { counter%(i)d=0; m%(i)d.bind(&receive_next%(i)d, &receive_complete%(i)d, this);}") %locals()

        self.straight   = ''
        self.outputType =('typedef ' + subName + '_sub::output ' + name+ '_type%(i)d;') %locals()
        self.internal_dcl = 'manager'
        self.internal_setup = 'init_higher_order' + str(i) + "();"
        self.index=i
        self.maxConcurrent = maxConcurrent

class endSequence:
    def __init__(self, op,name):
        index = op[3]
        print op[4][2]
        i = index
        deci = i + 1
        index = index + 1
        deci = i - 1
        inci = i + 1
        self.straight = ''
        self.trampoline = (
                              'inline void transform_next%(i)d()     { transform_next%(inci)d(); }\n'
                              '  inline void transform_complete%(i)d() { transform_complete%(inci)d(); }'
                          ) % locals()
        self.outputType = (
                          'typedef ' + name + '_type%(deci)d ' + name + '_type%(i)d;') % locals()
        self.index = i

class endNamedSequence:
    def __init__(self):
        pass

class namedSequence:
    def __init__(self,op,name,capture):
        print op[4][2]
        index = op[3]
        i = index
        self.name     = name
        self.capture  = capture
        self.outputType =('typedef decltype(' + capture + ') ' + name+ '_type%(i)d;') %locals()
        captype = 'decltype(' + capture + ')'
        self.captype  = captype
        self.ops      = [getEntity(o,name,captype,capture) for o in op[5]] 
        self.class_definition = create_class(name, capture, self.ops)

