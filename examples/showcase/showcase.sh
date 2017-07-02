rm showcase.exe
echo '\n\n\n\n////////////////////  START GENERATION showcase //////////////////' 
g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections showcase.t-rx.out.cpp -Wunused-result -o showcase.exe -Wl,--gc-sections 2>&1 | grep error 
size showcase.exe && ./showcase.exe | c++filt -t
    
