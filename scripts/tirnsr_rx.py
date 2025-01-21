import os,sys
from t_rx_file import *

if (sys.argv[1] == "-f"):
    generateCodeForOneFile(sys.argv[2])
else:
    for root,dirs,files in os.walk(sys.argv[1]):
        for f in files:
            if f.endswith('.t-rx.in.cpp'):
                print(f)
                generateCodeForOneFile(os.path.join(root,f))

print('######-- DONE --####')
print('######-- DONE --####')


