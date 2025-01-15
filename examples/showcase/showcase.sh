rm showcase.exe
echo '\n\n\n\n////////////////////  COMPILING showcase //////////////////' 
# from http://ptspts.blogspot.com/2013/12/how-to-make-smaller-c-and-c-binaries.html
g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections showcase.t-rx.out.cpp -Wunused-result -o showcase.exe -Wl,--gc-sections 2>&1 | grep error 
# g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections -ftree-coalesce-vars -ftree-loop-im -m32 -fno-rtti -fno-stack-protector -fmerge-all-constants  -fno-math-errno -fno-exceptions -fvtable-gc showcase.t-rx.out.cpp -Wunused-result -o showcase.exe -Wl,--gc-sections 2>&1 | grep error 
size showcase.exe && ./showcase.exe | c++filt -t
    
