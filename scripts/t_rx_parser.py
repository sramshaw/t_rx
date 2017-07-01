import re

def no_capture(mask):       # for regex
    return mask.replace(')','').replace('(','')

def getPatterns():
    return "(.*)(" + "|".join([no_capture(o[1]) for o in getRxGrammar()])  + ")(.*)"

def getall(txt,capture):
    r = re.search(capture,txt,re.DOTALL)
    if r:
        a = r.group(1)
        b = r.group(2)
        c = r.group(3)
        arr = getall(a,capture)
        #print 'bbbbbbbbbbbbb\n' +b
        #print 'ccccccccccccc\n' +c
        return arr + [[a,b,c]] 
    return [] 

def identified(v,detection):
    b = [o[0] for o in detection if re.search(no_capture(o[1]),v)]
    return b[0]

def identifiedFull(v,detection):
    b = [o for o in detection if re.search(o[1],v)]
    return b[0]

def getRxGrammar():
    return [
    # partial definitions for operators used in parsing only, not used in code generation
    # note that the size of the array (here: 2) is used to distinguish these from the above
    [   "comment",
        '//!.*[\n\r]'],
    [   "endNamedSequence",
        "endObservable\s*\;"],
    [   "endSequence",
        "endObservable",
        "",
        "endSequence(op,name)"],
    # fully qualified receipe for operators used in the code generation
    [   "namedSequence",
        "sequence\s*(\w+)\s*=\s*\[(\w*)\]\s*=>",
        "",
        "namedSequence(op,a(1),a(2))"],
    [   "selectmany",
        'selectmany\s+(\w+)\s*,\s*\[\s*(\w*)\]\s*=>',
        "",
        "selectmany(op,name,a(1),a(2))"], 
    [   "where0",
        "where\s+(\w+)\s*=>\s*\{",
        "(.*)\}\s*",
        "where(op,name,captype,capture,a(1),'{' + b(1) +'}')"],
    [   "where2",
        "where\s+(\w+)\s*=>\s*",
        "(.*)\s*",
        "where(op,name,captype,capture,a(1),'{return ' + b(1) +';}')"],
    [   "take0",
        "take(.*)",
        "",
        "take(op,name,captype,capture,a(1))"],
    [   "do0",
        "do\s+(\w+)\s*=>\s*\{",
        "(.*)\}\s*",
        "do(op,name,captype,capture,a(1),'{' + b(1) +'}')"],
    [   "do2",
        "do\s+(\w+)\s*=>\s*",
        "(.*)\s*",
        "do(op,name,captype,capture,a(1),'{ ' + b(1) +';}')"],
    [   "select0",
        "select\s+(\w+)\s*=>\s*\{",
        "(.*)\}\s*",
        "select(op,name,captype,capture,a(1),'{' + b(1) +'}')"],
    [   "select1",
        "select\s+(\w+)\s*=>\s*struct\s*\{",
        "(.*)\}\s*",
        "select(op,name,captype,capture,a(1),'{return {' + b(1) +'};}')"],
    [   "select2",
        "select\s+(\w+)\s*=>\s*",
        "(.*)\s*",
        "select(op,name,captype,capture,a(1),'{return ' + b(1) +';}')"], 
    [   "select3",
        "select\s+(\w+)\s*",
        "",
        "select(op,name,captype,capture,'rx_'+name,'{return ' + a(1) +'(rx_'+ name + ');}')"],
    [   "fromStaticPublisher",
        "fromStaticPublisher\<(\w+)\>",
        "\s*\(\s*[\*\&]\s*(\w+)\s*,\s*[\*\&]\s*(\w+)\s*\)\s*",
        "fromStaticPublisher(op,a(1),b(1),b(2),name,captype,capture)"],
    [   "fromJust",
        "fromJust\<(\w+)\>",
        "\s*\((.*)\)\s*",
        "fromJust(op,a(1),b(1),name,captype,capture)"],
    [   "fromRange",
        "fromRange\<(\w+)\>",
        "\s*\((.*),(.*)\)\s*",
        "fromRange(op,a(1),b(1),b(2),name,captype,capture)"],
    ]



