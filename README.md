## [ti-re:ks] The tiny C/C++ Rx implementation (hobby prototype, not for prod)
### Foreword
T_rx or tirnsr_rx is a tiny version of [ReactiveX [Rx]](http://reactivex.io/intro.html). It is a small DSL that offers flatmap capabilities and a fluent way to combine C# inspired lambdas. All memory is on the stack, no use of new (so far). Beware, made for fun by a C# programmer enthusiastic about Rx.Net, ex C programmer not C++.

This is aiming at programs that need to fit on 32kB (Ex: single [Parallela](https://www.adapteva.com/parallella/) core).
 
ReactiveX [Rx] is a powerful language that can change, where applicable, the way we code event based systems. It can and has been used in place of what would have been accomplished via small interpreters and/or state machines.


- [\[ti-re:ks\] The tiny C/C++ Rx implementation (hobby prototype, not for prod)](#ti-reks-the-tiny-cc-rx-implementation-hobby-prototype-not-for-prod)
  - [Foreword](#foreword)
  - [Goal](#goal)
  - [Targetted systems](#targetted-systems)
  - [Implementation](#implementation)
  - [Example](#example)
  - [Design decisions](#design-decisions)
  - [Demo](#demo)
  - [Development](#development)
  - [Status : in construction](#status--in-construction)
  - [Notes](#notes)
    - [uml diagram using plantuml plugin](#uml-diagram-using-plantuml-plugin)
    - [tooling versions](#tooling-versions)
  - [contribute](#contribute)
  - [release notes](#release-notes)
  - [coming next](#coming-next)
  - [design](#design)
    - [pythagorian example, inspired by RxCpp's example, which this design needs to beat by a lot to be worth using](#pythagorian-example-inspired-by-rxcpps-example-which-this-design-needs-to-beat-by-a-lot-to-be-worth-using)
  - [references](#references)


### Goal
Having focused on small C systems for the better part of a decade, then having played with Rx.Net, I expect the following positive impact to small platform: 
1. [G1] Ease of reasoning offered by [Rx](http://reactivex.io/intro.html)
2. [G2] Freedom to compose however many operators, no price incurred by using 2 do(...) operators instead of 1 
3. [G3] Preserved performance via limited abstraction and sequence simplification
4. [G4] Light weight implementation of operators, and where not possible: it will not be implemented (go to RxCpp for that)

### Targetted systems
The project aims at small programs running on micro processors, say for instance a 16bit processor with 64KB of program space. The main behavior will remain single core and single threaded.

### Implementation
The main contribution required to make event handling palatable to small systems is a brutal shrinking of the executable. RxCpp will take most likely a minimum of 300KB for a basic implementation, for instance look at the [pythagorian example](https://github.com/ReactiveX/RxCpp/blob/main/Rx/v2/examples/pythagorian/main.cpp). 
Here we achieve shrinking by simplifying the sequences and observables at compile time. Type deduction is used to give it a C# feel, and no use of parenthesis since it is a DSL already, let's make it lighter. Use of `=>` to distinguish from C++ lambdas.
In  `selectmany` aka flatmap, the upstream value is recaptured (here as azc , then again azcx) to be usable in the sub-sequence.  

### Example
Here is how the Pythagorian example is written using t_rx, with the new special keywords {`sequence`, `select`, `do`, `selectmany`, `where`, `endObservable`, `fromRange`} :
```C
  { // 
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
```
Also there are more operators showcased in unit tests: {`never`, `take`, `scan`, `fromPublisher`}.


### Design decisions
1. minimal program size via inline, compile time simplifications
2. simplified sequences/observables are implemented in a class each, the core concept is the 'simple sequence' that is represeted by a single class. 
3. lean on the compiler and inlining to simplify successions of operators, for [G2]
2. memory management via stack and object pools, no new operator unless tied to mem pools
3. time sources handled as observables
4. disposal by getting out of scope

### Demo

Running this demo now essentially requires to use vscode to use the devcontainer environment. Once you open this repo in vscode, reopen it in the devcontainer. It should build itself as needed.  
The demos and unit tests will all run via `./run.sh` that will run 2 builds. Because the code generation output is hidden by 'clear' during (successful) preprocessing , only the last build will be visible in the console output in the end. This leaves the pythagorian example be the last one showing ouput. Consider running `build.sh` for each example independently.


### Development

If you are modifying files and wanting to see in vscode terminal the results of your changes live,
run `./dev.sh` in your terminal, it will monitor changes to folders listed, and run the ```./run.sh``` script which rebuilds all, applying the preprocessing to the files ending in ```t-rx.in.cpp``` .

### Status : in construction
The pythagorian example works, the showcase does too. This is LINQ type behaviors with sources being immediate. There is no stack limitations at this point.
The capture of lambdas is already done mostly for select and do operators.
Work needed on the push story with buffering to allow backpressure at the sources and temporary buffering by a subscriber.

### Notes

#### uml diagram using plantuml plugin
it did not work for me until I installed on WSL: ```sudo apt install openjdk-21-jre-headless```
extensions used: [listed here](./.vscode/extensions.json)


#### tooling versions
by running ```./versions.sh``` in the devcontainer, here is the snapshot of the versions seen during a successful execution: 

```shell
$ g++ --version
g++ (Debian 12.2.0-14) 12.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


$ python3 --version
Python 3.11.2

$ cat /etc/lsb-release
cat: /etc/lsb-release: No such file or directory

$ clang-format --version
Debian clang-format version 14.0.6

```

### contribute

So far this is a solo POC, but I am open to suggestions. I believe the parsing approach with regex is flawed and constraining, but workable. For now learning is the main objective, with great results and coverage.

Here is an interesting website for working on the parser with regex, and expecially captures: [https://regex101.com/](https://regex101.com/)


### release notes
- v0.1 - the original LINQ like design has the following decomposition of features:
  - sequence class that
    - concatenates most of the operators using a union variable to hold the transformed values over time
    - branches out values to sub sequences as needed for selectmany (aka flatmap)
  - foreach selectmany operator, a manager class that handles a pool (std::array) of sub-sequences
    - binds the sub- sequences to the main sequence, and also
    - initiates immediate inits with assign_work
    - ultimately disposes (aka disables in the case of the pool approach) the sub
- v0.2 - optimization , this version now merges the managers within the sequence owning the corresponding selectmany logic
  - this reduces the number of indirections
    - seen by smaller binary:
    >-rwxr-xr-x 1 root   root   18840 Jan 16 23:28 pythagorian.exe.original  

    >-rwxr-xr-x 1 root   root   18592 Jan 16 23:27 pythagorian.exe.merge_binding_to_sequence  
    - we can see that ~250 bytes are spared by this, explained by more inlining
  - note that it is possible to abstract more parts of the system to have a manager that is more generic, and therefore reusable when the templates are redundant
    - this is no likely though, and still requires more execution
-  v0.3
   -  scan0, sacn2: operator using simple ints on capture no issue, but limited use when nested in selectmany
   -  scan1: use explicitly captured accumulator pool, (had issue passing the pool via top of sequence capture like for scan0 and scan2)
      -  the anonymous struct approach leads to union issue due to non-trivial constructors
   -  fixes, now captured in unit tests

### coming next
- [ON HOLD]: gcc13.1+ for C++20 support
  -  I have a gcc rebuild branch, but will stop users given the time it takes
  - nice to have for std support of [Designated Initializers](https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP20.md#designated-initializers) etc..
  - any use of [templates for lambdas](https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP20.md#template-syntax-for-lambdas) ? 
  - string [starts_with and ends_with operators](https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP20.md#starts_with-and-ends_with-on-strings), for rx/linq concise filtering in where operator etc...
  - easier creation of arrays as auto vars initialized with [to_array](https://en.cppreference.com/w/cpp/container/array/to_array)
  - maybe use of [generators (C++23)](https://en.cppreference.com/w/cpp/header/generator) eventually ? though it is not useful for linq like behavior here it seems
- [READY FOR IMPLEMENTATION] ReactiveX push behavior, using a 
  - forever loop as background task to advance
  - sources push to buffers (optimal circular buffers)
  - operator fromCircular(get function) registers to the advance mechanism
  - a new operator withLogic put anywhere allows other logic to be serviced (used here to fake data)
- [RESEARCH] user ease of adoption, more C# like features
  - attempt to map debugging experience running on the transformed files back to displaying the original file
    - most promising: https://stackoverflow.com/questions/42234485/showing-original-source-when-debugging-generated-code-in-gdb
  - color coding of sequence
    - put the sequence in a comment to avoid intellisense ?
      - example off [better-comments](https://github.com/aaron-bond/better-comments/blob/master/src/extension.ts) extension plugin for vscode 
      - use cpp related syntac from [better-cpp](https://github.com/jeff-hykin/better-cpp-syntax/blob/master/autogenerated/cpp.tmLanguage.json) ?
      - use an anchors like markdown's \`\`\`C  .... \`\`\`
      - new anchor \`\`\`seq
  - more operators: buffer
  - back pressure support via circular buffer, whichever [most standard one c++ provides (is libc++ too much?)](https://stackoverflow.com/a/79102831/19037406) on top of std array would be best
- [NEXT] use google test for tests, and [google benchmark](https://github.com/google/benchmark)
  - use test maste
  - generate benchmarks for alternatives of implementation
  - review [asymptotic scaling](https://github.com/NAThompson/using_googlebenchmark)?
- [OBJECTIVES] long term
  - minimal code for embedded or containers
    - chaining various wasm modules together using C++ t_rx based plumbing
      - need support for compilation to wasm 
    - combine with modules in [embedded Rust](https://docs.rust-embedded.org/book/intro/index.html)
  - contrast the present rx implementation with what could be a [coroutine (c++20)](https://en.wikipedia.org/wiki/Coroutine) equivalent
  

### design

Note that there is a force to try and move the declaration of std:arrays for selectmany and scan from top level into the owning sequence, as the sequence could have enough info to contain the array, however this is not done at the moment.
For selectmany, the issue comes with what could lead to an automatic allocation of a pool bigger than strictly necessary: first selectmany announces 3 subsequences, second selectmany announces 2subsequences for each of its sequences, auto assignement would be of 3*2 subsequences, maybe an asymetry could lead to 4 or 5 to be enough.
Instead today we would announce the total instances needed for the second selectmany, for instance: 5.
In the case of scan, the issue has to do with how to infer the type of accumulator and also initialize the member variable from a lambda in place. I am not sure if that is possible. Instead, a direct capture is done. Direct captures could be made available in other operators as they are easy to handle, but not recommended for unknown uses. not yet time to decide.

#### pythagorian example, inspired by [RxCpp's example](https://github.com/ReactiveX/RxCpp/blob/main/Rx/v2/examples/pythagorian/main.cpp), which this design needs to beat by a lot to be worth using

```C
  { // 
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
```
Below is a graph generated via plantuml, for now built manually, could be autogenerated I suppose. Install the recommended vscode extensions to see the graph in the readme preview. Only for illustration to help navigate the preprocessed code at the moment.

```plantuml
@startuml lifecycle of captured variables
' START stack items definitions, from pythagorian.t-rx.out.hpp
' from interior to exterior
participant stack

participant seq14_2_sequence2_array.n [
  = seq14_2_sequence2_array.n
  ----
  is std::array<subsequence>
  ----
  __is rx_usable()__
  + bool enabled
  + constr() => enabled =false;
  ----
  __is observable(seq14_sub)__
  - exit_next
  - exit complete
  + void* exit_obj
  + void subscribe(exit_next,exit_complete,exit_obj)
  + void direct_exit_next( d)
  ----
  - auto capture _c0 
  - auto union _exchange
  - receive_next
  - count
  generate:
  (...)
  + bool enable(c0)
  + void disable
  + constructor => func_init_higher_order 
  + destructor => disable
]
participant seq14_2_sequence2_array [
  = seq14_2_sequence2_array
  ----
  is std::array<subsequence>
]

participant seq14_sequence2_array.n [
  = seq14_sequence2_array.n
  ----
  is high_order_manager(sequence_array)
  ----
  + constructor
  + bind
  + unbind
  + assign_work
]
participant seq14_sequence2_array.n [
  = seq14_sequence2_array.n
  ----
  is std::array<subsequence>
  ----
  __is rx_usable()__
  + bool enabled
  + constr() => enabled =false;
  ----
  __is observable(seq14_sub)__
  - exit_next
  - exit complete
  + void* exit_obj
  + void subscribe(exit_next,exit_complete,exit_obj)
  + void direct_exit_next( d)
  ----
  - auto capture _c0 
  - auto union _exchange
  - receive_next
  - count
  generate:
  (...)
  + bool enable(c0)
  + void disable
  + constructor => func_init_higher_order 
  + destructor => disable
]
participant seq14_sequence2_array [
  = seq14_sequence2_array
  ----
  is std::array<subsequence>
]
participant seq14_sub [
  = seq14_sub
  ----
  + bind 
  + unbind
  + assign_work
  ----
  __is rx_usable()__
  + bool enabled
  + constr() => enabled =false;
  ----
  __is observable(seq14_sub)__
  - exit_next
  - exit complete
  + void* exit_obj
  + void subscribe(exit_next,exit_complete,exit_obj)
  + void direct_exit_next( d)
  ----
  - auto capture _c0 
  - auto union _exchange
  - receive_next
  - count
  generate:
  - transform_next0 ()
  - transform_complete0()
  - transform_next1 ()
  - transform_complete1 = push next
  for selectmany:
  - counter2 (not init!?!?)
   

  + bool enable(c0)
  + void disable
  + constructor => func_init_higher_order 
  + destructor => disable
]
' END stack items definitions

' START stack usage section, from pythagorian.t-rx.out.hpp
' from interior to exterior
stack->stack: stack reservations \nat declaration

stack -> seq14_2_sequence2_array.n : create (seq14_sequence2_array.n)
activate stack #AAAAAA
activate seq14_2_sequence2_array.n #FFBBBB
seq14_2_sequence2_array.n -> seq14_2_sequence2_array.n : __func_init_higher_order__:\n    count =0 
activate seq14_2_sequence2_array.n #FF4444
seq14_2_sequence2_array.n -> seq14_2_sequence2_array.n : NO bind
deactivate seq14_2_sequence2_array.n
seq14_2_sequence2_array.n -> stack
deactivate seq14_2_sequence2_array.n

stack -> seq14_2_sequence2_array : create (seq14_2_sequence2_array.n)\noutside sequence\nso that is on the stack at top level\nof declaring function
activate stack #AAAAAA

stack -> seq14_sequence2_array.n : create (seq14_sequence2_array.n)
activate stack #AAAAAA
activate seq14_sequence2_array.n #FFBBBB
seq14_sequence2_array.n -> seq14_sequence2_array.n : __func_init_higher_order__:\n    count =0 
activate seq14_sequence2_array.n #FF4444
seq14_sequence2_array.n -> seq14_sequence2_array.n : bind (this.receive_next, this.receive_complete, this)
seq14_sequence2_array.n -> seq14_sequence2_array.n
deactivate seq14_sequence2_array.n
seq14_sequence2_array.n -> stack
deactivate seq14_sequence2_array.n

stack -> seq14_sequence2_array : create (seq14_sequence2_array.n)\noutside sequence\nso that is on the stack at top level\nof declaring function
activate stack #AAAAAA


stack -> seq14_sub : create (seq14_sequence2_array.n)
activate stack #AAAAAA
activate seq14_sub #FFBBBB
seq14_sub -> seq14_sub : __func_init_higher_order__:\n    count =0 
activate seq14_sub #FF4444
seq14_sub -> seq14_sub : bind (this.receive_next, this.receive_complete, this)
seq14_sub -> seq14_sub
deactivate seq14_sub
seq14_sub -> stack
deactivate seq14_sub
' END stack usage section

' START usage section, from pythagorian.t-rx.out.cpp
' from interior to exterior staying within for immediate action
activate stack #00FF00
stack -> seq14_sub : (&n_trace, &c_trace, nullptr)

stack -> seq14_sub : enable(c0)
seq14_sub -> seq14_sub : set enabled = true
seq14_sub -> seq14_sub : set capture value passed as arg
seq14_sub -> seq14_sub : try an external provider \n(nothing in fromconnect would pull \nfrom a buffer)
' note that for background task to try later, would be best to hit fromconnect as long as enabled, and avoid set enabled and set capture.
seq14_sub -> seq14_sub : for loop logic starts\npushing integers to transform_next1
seq14_sub -> seq14_sub : select logic\n transform_next1 applies seq14_fs1\n z1 => struct {z = z1, c = c0}
activate seq14_sub
seq14_sub -> seq14_sub : selectmany logic\n try to assign work to subsequence
seq14_sub -> seq14_sub : success =assignwork(for_capture, this)

seq14_sub -> seq14_sequence2_array : get iterator => foreach element
seq14_sub -> seq14_sub : if element.enabled==false, then
seq14_sub -> seq14_sequence2_array.n : element.exit_obj=caller
'note: should above be passed in enabler?
seq14_sub -> seq14_sequence2_array.n : ret = enable(captured)
seq14_sequence2_array.n -> seq14_sequence2_array.n : same logic
'same logic within for seq14_sequence2_array.n.enable,
' and again within 
seq14_sequence2_array.n -> seq14_sub : static receive_next(v2, self) { self->state=v2;}
seq14_sub -> seq14_sub : select logic\n transform_next3 applies seq14_fs3\n a => a.z
seq14_sub -> seq14_sub : take logic\n x=> { counter4++; &n_trace(x); if (counter4 >=  100) {stop()}
seq14_sub -> seq14_sub : stop: oncomplete => dispose
activate seq14_sub
seq14_sub -> seq14_sub : disable();
activate seq14_sub
seq14_sub -> seq14_sub : enable = false;
seq14_sub -> seq14_sub : enable = false;
seq14_sub -> seq14_sub : disableFrom(this)
seq14_sub -> seq14_sequence2_array : get iterator\nforeach element
seq14_sub -> seq14_sequence2_array.n : if (element.exitObject == caller)
seq14_sub -> seq14_sequence2_array.n : element.disable()
seq14_sub -> seq14_sub
deactivate seq14_sub
deactivate seq14_sub

seq14_sub -> seq14_sub
seq14_sub -> seq14_sequence2_array.n
seq14_sequence2_array.n -> seq14_sub: return true
' need o revisit the dispose() story. is the completed from top observable propagating down: .
' transform_next2 probably needs to call transform_complete2() if !this->enabled,
'  transform_complete2 should call manager.unbind ()
' transform_next4 should have a bool to prevent a double transform_complete4
' transform_complete4() to call transform_complete5()


seq14_sub -> seq14_sub : return ret or 'no element => false'

' seems below is for tracing?
seq14_sub -> seq14_sub : if success counter2++
seq14_sub -> stack 
deactivate seq14_sub

stack -> seq14_sub : disable() // already done - not drawing again

stack -> stack : out of scope => stack cleanup
deactivate stack
deactivate stack
deactivate stack
deactivate stack
deactivate stack
deactivate stack
deactivate stack
deactivate stack
@enduml

```

### references
other than known Rx.Net and RxCpp
https://github.com/Cysharp/R3 (improved over https://github.com/neuecc/UniRx)
https://github.com/victimsnino/ReactivePlusPlus and https://victimsnino.github.io/ReactivePlusPlus/benchmark

the old https://github.com/ReactiveX/RxCpp/tree/main/Rx/v2/examples/pythagorian

compiler online: https://godbolt.org/

