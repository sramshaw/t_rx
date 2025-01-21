import re

def no_capture(mask):       # for regex
    return mask.replace('\)','close_parenthesis').replace('\(','open_parenthesis').replace(')','').replace('(','').replace('close_parenthesis','\)').replace('open_parenthesis','\(')


def getPatterns():
    return "(.*)(" + "|".join([no_capture(o[1]) if o[1].startswith("from") else "\n[\t ]*"+no_capture(o[1]) for o in getRxGrammar()])  + ")(.*)"

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
    [   "scan0",
        'scan\s+(\w+)->(\w+\s*\[\s*\w+\s*\]\s*)\s*,\s*\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*=>\s*(\{.*\})\s*,\s*(\w+)\s*=>\s*(.*)',
        "",
        "select(op,name,captype,capture,a(4),'{ typedef decltype('+a(1)+'->'+a(2)+') _'+a(3)+'; auto accumulate = [](_'+a(3)+' '+a(3)+',type%(deci)d '+a(4)+')'+a(5)+'; auto project = [](_'+a(3)+' '+a(6)+'){ return '+a(7)+';}; '+a(1)+'->'+a(2)+' = accumulate('+a(1)+'->'+a(2)+','+a(4)+'); return project('+a(1)+'->'+a(2)+'); }')"], 
    [   "scan1",
        'scan\s+\[\s*(\w+)(s*\])\s*,\s*\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*=>\s*(\{.*\})\s*,\s*(\w+)\s*=>\s*(.*)',
        "",
        "scan(op,name,captype,capture,a(4),'{ typedef decltype('+a(1)+'[0]) _'+a(3)+'; auto accumulate = [](_'+a(3)+' '+a(3)+',type%(deci)d '+a(4)+')'+a(5)+'; auto project = [](_'+a(3)+' '+a(6)+'){ return '+a(7)+';}; '+a(1)+'[index] = accumulate('+a(1)+'[index],'+a(4)+'); return project('+a(1)+'[index]); }', a(1))"], 
    [   "scan2",
        'scan\s+(\w+)->(\w+)\s*,\s*\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*=>\s*(\{.*\})\s*,\s*(\w+)\s*=>\s*(.*)',
        "",
        "select(op,name,captype,capture,a(4),'{ typedef decltype('+a(1)+'->'+a(2)+') _'+a(3)+'; auto accumulate = [](_'+a(3)+' '+a(3)+',type%(deci)d '+a(4)+')'+a(5)+'; auto project = [](_'+a(3)+' '+a(6)+'){ return '+a(7)+';}; '+a(1)+'->'+a(2)+' = accumulate('+a(1)+'->'+a(2)+','+a(4)+'); return project('+a(1)+'->'+a(2)+'); }')"], 
    [   "selectmany",
        'selectmany\s+(\w+)\s*,\s*\[\s*(\w*)\]\s*=>',
        "",
        "selectmany(op,name,a(1),a(2))"], 
    [   "merge",
        'merge\s+\[\s*(\w*)\s*\]\s*=>',
        "",
        "merge(op,name,a(1))"], 
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
    [   "fromPublisher0",
        "fromPublisher\<(\w+)\>",
        "\s*\(\s*(\w+)\s*\)\s*",
        "fromStaticPublisher(op,a(1),b(1)+'.subscribe',b(1)+'.dispose',name,captype,capture)"],
    [   "fromStaticPublisher1",
        "fromStaticPublisher\<(\w+)\>",
        "\s*\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*",
        "fromStaticPublisher(op,a(1),b(1),b(2),name,captype,capture)"],
    [   "fromJust",
        "fromJust\<(\w+)\>",
        "\s*\((.*)\)\s*",
        "fromJust(op,a(1),b(1),name,captype,capture)"],
    [   "fromCapture",
        "fromCapture\s*(\s*)\s*",
        "",
        "fromJust(op,captype,capture,name,captype,capture)"],
    [   "fromRange",
        "fromRange\<(\w+)\>",
        "\s*\((.+),(.+)\)\s*",
        "fromRange(op,a(1),b(1),b(2),name,captype,capture)"],
    [   "never",
        "fromNever\<(\w+)\>",
        "\s*\(\s*\)\s*",
        "never(op,a(1),name)"],
    ]



