from subprocess import call
from t_rx_operators import *
from t_rx_parser import *
import os, re

# use of a tree for representation of nesting of elements in a separate list, nesting given by a rank
# each leaf is an array such that : 
#  [ 0 => index in list, 
#    1 => parent index in list, 
#    2 => rank, 
#    3 => index in rank, 
#    4 => factory/invoker info ( array as defined for each operator in t_rx_parser.py )
#    5 => dependent leaves
#  ]
# for instance [0,-1, 0, 0, [[1,0,1,[]]]] represents  a tree of 2 leaves 


def getContents(filename):
    with open(filename,"r") as f:
        return f.read()

rank = 0
id =-1
def reset():
    global rank
    global id
    rank=0
    id =-1
def countRank(op):
    global rank
    i = rank
    if op == 'namedSequence' or op == 'selectmany':
        rank = rank +1
    if op == 'endSequence':
        rank = rank -1
    print(rank,op)
    return i
def increment():
    global id
    id = id +1
    return id

def codeFragments(operatorsFragments,original):
    a= operatorsFragments
    originalSequenceCode = "".join([o[1]+o[2] for o in a[:-1]]) + a[len(a)-1][1]
    codeBefore = a[0][0]
    codeAfter = a[len(a)-1][2]
    whole  = codeBefore + originalSequenceCode + codeAfter
    print(originalSequenceCode)

    print("are all parts equal to the whole?  " + str(whole == original))
    return [codeBefore,originalSequenceCode,codeAfter]

def createFlatElement(parsedElement,rx_grammar):
    o = parsedElement
    return [increment(), 
            countRank(identified(o[1],rx_grammar)), 
            identified(o[1],rx_grammar), 
            o[1], 
            o[2],
            identifiedFull(o[1],rx_grammar)]
def getFlatElementRank(element):
    return element[1]

def getId(o):
    return o[0]
def findById(tree,id):
    for o in tree:
        if getId(o) == id:
            #print "found in "+  str(o)
            return o
        sub = findById(getSubbranches(o),id)
        if not sub is None:
            return sub
    return None 
def createTreeBranch(flatIndex,parentFlatIndex,rank,indexInRank,flatElement):
    return [flatIndex,parentFlatIndex,rank,indexInRank,flatElement,[]] 
def getBranchRank(branch):
    return branch[2]
def getSubbranches(branch):
    return branch[5]
def addSubbranch(branch,subbranch):
    branch[5] = branch[5] +[subbranch]
def getBranchRankIndex(branch):
    return branch[3]
def createTree(flat_data):
    noId  = -1
    rank0 = 0
    tree  = [createTreeBranch(getId(o), noId, getFlatElementRank(o), rank0, o) for o in flat_data if getFlatElementRank(o) == rank0] 
    c     = [o for o in flat_data if getFlatElementRank(o) > rank0]
    print(tree)
    for o in c:
        previous  = findById(tree, getId(o)-1)
        parent    = previous             # default case: previous is the parent
        rankIndex = 0
        if getBranchRank(previous) == getFlatElementRank(o)+1:       # if previous is nephew, get the nephew's parent
            previous = findById(tree,previous[1])
        if getBranchRank(previous) == getFlatElementRank(o):         # if previous in list is sibling
            rankIndex = getBranchRankIndex(previous)+1      # get index in the rank relative to the sibling
            parent = findById(tree,previous[1])
        addSubbranch(parent,createTreeBranch(getId(o),getId(parent),getFlatElementRank(o),rankIndex,o))
    return tree


def getStandaloneDeclarations(operators):
    s   = len(operators)
    ret = "\n".join([getStandaloneDeclarations(o.ops) for i,o in enumerate(operators) if hasattr(o,"ops")])
    ret = ret + "\n" + "\n".join([o.external for o in operators if hasattr(o,"external")])
    return ret


def generateTypes_i_o(i,o,name):
    ret = ""
    if hasattr(o,"ops"):
        sub = generateTypes(name+ "_" + str(i), o.ops, name+"_type"+str(i-1))
        ret = ret + sub[0]
        array = "\nstd::array<"+name+ "_" + str(i)+"_sub,"+o.maxConcurrent+"> "+name+"_sequence"+str(i)+"_array={"+",".join(["[_creator_]" for j in range(0, int(o.maxConcurrent))])+"};"
        ret = ret + array.replace('[_creator_]',sub[1])
        ret = ret + "\ntypedef high_order_manager<decltype("+name+"_sequence"+str(i)+"_array)&,"+name+"_type"+str(i-1)+","+name+"_"+str(i)+"_sub::output> "+name+"_manager"+str(i)+"_type;"
        ret = ret + "\n"+name+"_manager"+str(i)+"_type "+name+"_manager"+str(i)+"("+name+"_sequence"+str(i)+"_array);"
    #print o
    ret = ret + "\n" + (o.outputType if hasattr(o,'outputType') else '')
    return ret
def generateTypes(name, operators, parent_type):
    ret = "".join([generateTypes_i_o(i,o,name) for i,o in enumerate(operators)])
    types_list = ( [parent_type]
           + [name+"_type" + str(i) for i,o in enumerate(operators)] 
           + [name+"_manager"+str(i)+"_type" for i,o in enumerate(operators) if hasattr(o,"ops")])
    ret = ret + "\ntypedef "+name+"<" + ",".join(types_list) +"> "+name+"_sub;"
    return [ret,name+"_sub("+",".join([name+"_manager"+str(i) for i,o in enumerate(operators) if hasattr(o,"ops")])+")"]

def clang_save(filename,txt):
    with open(filename,"w") as f:
        f.write(txt)
    call (["clang-format", filename])    
    
def getFinalOperators(tree):
    operators = [getEntity(o,"","","") for o in tree]
    return operators

def getHpp(operators):
    rootOperator = operators[0]
    print('\n\n\n\n\n')
    print('####################')
    print(rootOperator)
    print('####----------------')
    name = rootOperator.name
    ret = "#include <functional>\n#include <array>"
    ret = ret + getStandaloneDeclarations(rootOperator.ops)
    ret = ret + rootOperator.class_definition
    ret = ret + "\n\n #define T_RX_CREATE_" + name + "() \\\n"
    sub = generateTypes(name, rootOperator.ops,rootOperator.captype)
    ret = (ret + sub[0].replace("\n","\\\n")
               + "\\\n  auto " + name + " = "+sub[1]+ ";\\\n " 
           "\n"#    + name +" .enable(" + rootOperator.capture + "); \n\n"
           )
    print('########------------')
    return ret

def getCpp(operators,codeBefore,codeAfter,originalSequenceCode,hppname):
    rootOperator = operators[0]
    name = rootOperator.name
    #cpp = '#include "../../include/trx.hpp"\n#include "' + hppname + '"\n\n'
    cpp = codeBefore + "/*" + originalSequenceCode + "*/\n  T_RX_CREATE_" + name + "()\n" + codeAfter
    return cpp

def generateCodeForOneFile(fullname):
    filename = fullname.replace(".t-rx.in.cpp","")
    hppname = filename + ".t-rx.out.hpp"
    cppname = filename + ".t-rx.out.cpp"
 
    original = getContents(fullname)
    includePattern = "(.*)([\/\\\\\\\"]trx\.hpp\")(.*)"
    r = re.search(includePattern,original,re.DOTALL)
    if not r:
        print("[" + fullname + "]----------------------- does not contain any Rx")    
        return
    a = r.group(1)
    b = r.group(2)
    c = r.group(3)
    original = a + b + "\n#include \"" + os.path.basename(hppname) + "\""+c
    
   
    print("[" + fullname + "]----------------------- PARSE")    
    patterns = getPatterns()
    operatorsFragments = getall(original,patterns)
    print("[" + fullname + "]----------------------- GROUP")    
    [codeBefore,originalSequenceCode,codeAfter] = codeFragments(operatorsFragments,original)
    print("[" + fullname + "]----------------------- ENRICH")    
    reset() # for increment and countRank used for createFlatElement 
    rx_grammar = getRxGrammar()
    flat_data = [createFlatElement(o,rx_grammar) for o in operatorsFragments if len(identifiedFull(o[1],rx_grammar))>2]
    print(flat_data)
    print("[" + fullname + "]----------------------- ORGANIZE AS TREE")    
    tree = createTree(flat_data)
    print("[" + fullname + "]----------------------- OUTPUT")    

    operators = getFinalOperators(tree)
    hpp = getHpp(operators)
    cpp = getCpp(operators, codeBefore,codeAfter,originalSequenceCode, os.path.basename(hppname))
    clang_save(hppname, hpp)
    clang_save(cppname, cpp)


