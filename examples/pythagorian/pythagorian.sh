#rm main1
#echo '\n\n\n\n////////////////////  START GENERATION main1 //////////////////'
# wasteful version, to compare to RxCpp
#g++ -std=c++14  -O2 -s -DNDEBUG pythagorian.cpp -o main1  && size main1 && ./main1
# efficient one to stay below 32KB
#g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections pythagorian.cpp -Wunused-result -o main1 -Wl,--gc-sections && size main1 && ./main1
#g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections pythagorian.gen.py.cpp -Wunused-result -o main1 -Wl,--gc-sections 2>&1 | grep error 
#g++ -std=c++14 -I . -O0 -fno-inline -fdata-sections -ffunction-sections -g pythagorian.cpp -Wunused-result -o main1 -Wl,--gc-sections 2>&1 | grep error 
#size main1 && ./main1 | c++filt -t

rm pythagorian.exe
echo '\n\n\n\n////////////////////  START GENERATION pythagorian //////////////////' 
#g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections pythagorian.t-rx.out.cpp -Wunused-result -o pythagorian -Wl,--gc-sections 2>&1 | grep error 
g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections pythagorian.t-rx.out.cpp -Wunused-result -o pythagorian.exe -Wl,--gc-sections 2>&1 | grep error 
#g++ -S -masm=intel -std=c++14 -I . -Os -fdata-sections -ffunction-sections pythagorian.t-rx.out.cpp -Wunused-result -o pythagorian.s -Wl,--gc-sections 2>&1 | grep error 
size pythagorian.exe && ./pythagorian.exe | c++filt -t

#watch -n 1 "g++ -std=c++14 -O3 -fdata-sections -ffunction-sections pythagorian.cpp -o main1 -Wl,--gc-sections && size main1 && ./main1"
#watch -n 1 "g++ -std=c++14 -Os pythagorian.cpp -o main1 -Wl,--gc-sections && size main1 && ./main1"


