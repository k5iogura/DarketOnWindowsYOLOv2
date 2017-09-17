#! /usr/bin/env python
import sys
import re
import os

if len(sys.argv) < 3:
  print('usage: %s labels list'%(sys.argv[0]))
  sys.exit(1)
print('*label = %s\n*list  = %s'%(sys.argv[1],sys.argv[2]))

labels=open(sys.argv[1]).read().strip().split()
lists =open(sys.argv[2]).read().strip().split()

print('labels=%d\tlists=%d'%(len(labels),len(lists)))

all_items=0
for l in labels:
  notfounds = [re.findall(l,lists[i]) for i in range(0,len(lists))].count([])
  founds    = len(lists) - notfounds
  all_items+= founds
  print('%20s items / total =%6d / %8d'%(l,founds,len(lists)))
print('                       items total =%6d'%(all_items))

P={}	# empty dictionary
for i in lists:
  P[os.path.dirname(i)]=0
for i in lists:
  P[os.path.dirname(i)]+=1
print(' items path')
for p,n in P.items():
  print('%6d %s'%(n,p))
