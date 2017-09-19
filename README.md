# Darknet on Windows and YOLO  

From git clone https://github.com/AlexeyAB/darknet.git and modified.  

not only Visual C++(VS2013 community) but also mingw-gcc,  
sorry unsupporting GPU.  

additional support network,  
*cfg/cifar_minin.cfg training*
  this is minmum NiN as 1 3x3 conv + 3 1x1 layer  

./darknet classifier train data/cifar.data5 cfg/cifar_minin.cfg  

*testing*
./darknet classifier test data/cifar.data5 cfg/cifar_minin.cfg backup5/cifar_minin.weights  

*demo*
./darknet classifier demo data/cifar.data5 cfg/cifar_minin.cfg backup5/cifar_minin.weights -c 0  

