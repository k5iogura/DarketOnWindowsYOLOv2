#!/usr/bin/env python
import sys
import os

import shutil
import re
from pdb import *

if len(sys.argv)<=1:
  sys.exit(1)

fp= open(sys.argv[1])
f = fp.readlines()
fp.close()
op= open(sys.argv[1],'w')

##print(sys.argv[1],len(f))
##print(f[0])

pat1=re.compile('\n')
pat2=re.compile('\t')
for i in f:
#  set_trace()
#  o = re.sub(pat1,'' ,i)
  o = re.sub(pat2,',',i)
  op.write(o)
op.close()
