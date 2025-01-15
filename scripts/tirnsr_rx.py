import os,sys
from t_rx_file import *

for root,dirs,files in os.walk(sys.argv[1]):
    for f in files:
        if f.endswith('.t-rx.in.cpp'):
            print(f)
            generateCodeForOneFile(os.path.join(root,f))

print('######-- DONE --####')
print('######-- DONE --####')


