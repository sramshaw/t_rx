import os
from t_rx_file import *

for root,dirs,files in os.walk('./'):
    for file in files:
        if file.endswith('.t-rx.in.cpp'):
            print file
            generateCodeForOneFile(file)

print('######-- DONE --####')
print('######-- DONE --####')


