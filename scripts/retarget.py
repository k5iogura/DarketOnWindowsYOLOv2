import os
import sys
import re
import shutil

path = 'data/cifar/test'
path = 'data/cifar/train'
files = os.listdir(path)
print('{} files'.format(len(files)))

ptn = re.compile('^([0-9\-]+)_(\w+)\.png')

if not os.path.exists(path+'3'):
  os.mkdir(path+'3')

for f in files[:]:
  parts = ptn.findall(f)
  if len(parts[0]) == 2:
    number = parts[0][0]
    classn = parts[0][1]
    org_file = path+'/'+f
    if classn == 'automobile' or classn == 'truck':
      new_file = path+'3/'+number+'_'+'automobile'+'.png'
      print(" cp {} {}".format(org_file,new_file))
      shutil.copy(org_file,new_file)
    else:
      new_file = path+'3/'+number+'_'+'background'+'.png'
  else:
    print("unsupport filename",f)
