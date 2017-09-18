#!/usr/bin/env python
import argparse
import os
import sys
import re
import shutil
import fnmatch

parser = argparse.ArgumentParser(description='renumbering and bury label into filename')
parser.add_argument('in_dirs', type=str, nargs='+')
parser.add_argument('--label',    '-l', type=str, nargs=1, required=True ,help='label to be bury into filename')
parser.add_argument('--out_dir',  '-o', type=str, nargs=1, required=True)
parser.add_argument('--print_out','-p', default=False,action='store_true',help='print all files in in_dirs')
parser.add_argument('--copy',     '-c', default=False,action='store_true',help='copy into out_dir')
args = parser.parse_args()
print >> sys.stderr,args.in_dirs,args.out_dir

files=list()
for in_dir in args.in_dirs:
  for root,dirs,names in os.walk(in_dir):
    new_files= [ os.path.join(root,f) for f in names if re.findall('.jpg',f) or re.findall('.png',f)]
    print >>sys.stderr,root,dirs,len(names)
    files.extend(new_files)
print >>sys.stderr,'files=%d %s'%(len(files),files[0])

if not os.path.exists(str(args.out_dir[0])):
  print >>sys.stderr,'os.mkdir',str(args.out_dir[0])
  os.mkdir(str(args.out_dir[0]))

label = '_'+args.label[0]
for n,f in enumerate(files):
  if re.findall('.jpg',f):
    o = os.path.join(args.out_dir[0],str(n)+label+'.jpg')
  else:
    o = os.path.join(args.out_dir[0],str(n)+label+'.png')
  if args.print_out:
    print('cp %s\t%s'%(f,o))
  if args.copy:
    shutil.copy(f,o)

sys.exit(1)
