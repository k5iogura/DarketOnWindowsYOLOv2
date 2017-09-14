Binary WeightなDNNを試し、学習推定はほぼ性能が分かってきたかな  
正解率はCifar10で80%以上、MNISTでは99%以上までBinaryWeightでも達成できそう  

Cifar10のデータセットをみる限り、実用的では無いのは明らか;^)  
MNISTでは、、、なににつかうのだろうか？  

[■Darknetでの学習]
DarknetのWebに従い、VOCデータセットでの学習を試す  
Darknetではlinux版が一般的だが、VirtualBoxでは遅い  

[■Windows版Darknetってどうなの]

Windows版のDarknetがころがっているので試す、、、GPUも導入できるかも  

git clone https://github.com/AlexeyAB/darknet.git  

コンパイラはVisual Studio Community  
としたがまるでslnを認識してくれない、、、数時間格闘、、、あきらめたぁ  

src/以下のソースはたっくさん修正が入っているなぁ～Windows向けの修正だろうなぁ  

[■mingwでコンパイル]

cmdのコンソールからchocoでインストール  
choco install mingw  
choco install msys2  

cmderに切り替えて、調べてみると、  

gccとかld、arなんかも  
/c/tools/mingw64/bin/  

include系は  
/c/tools/mingw64/x86_64-w64-mingw32/include/  

一通り入ったのでPATHを通す  
export PATH=/c/tools/mingw64/bin/:$PATH  

コンパイルへ、  

MakefileのCFLAGSに  
CFLAGS+=-IC:/tools/mingw64/x86_64-w64-mingw32/include/  

と修正してからコンパイルしてみる  

[※パス表記の方法　CmderとMakefile]  
Cmder上でのCドライブルートは、/c/なのだが、makeコマンドはこれを理解しない  
Makefileには/c/ではなくC:/で表記するとmakeコマンドは理解できる  
bashのPATH変数は/c/を理解するしC:/も理解する  
ls C:/  
ls /c/  
どちらもルート直下を参照できる  
[※end]  

src/gettimeofday.h  
でtime.hが読めないとかエラー、、、;-<  
#include < time.h >  
↓  
#include <time.h>  
スペースを取ると読み込めるが今度はtimezoneの二重定義でエラー  
struct timezone  
{  
...  
}  
↑をコメントアウト、、、;-<  

これでコンパイル通りました:-)  

[■起動 @Cmder on Windows]  
./darknet.exe  
usage: darknet.exe <function>  

それらしく動いてます:-)  

[■Webに従いいつものdarknetのテスト]  
./darknet imtest data/eagle.jpg  
C1.png  
とかできてる:-)  

[■tiny-yoloを動かす]  
Webに従い  
./darknet detect cfg/yolo.cfg yolo.weights data/dog.jpg  
OpenCVが一緒にコンパイルされていなぁ～い、とか警告されますが、  
いつものように、  
predictions.png  
Windowsフォトビューアーで見てみると、  
bicycle, truck dog,,,出来ましたぁ～OKOK  

Windowsで動いちゃったぁ:-)  

CUDAとか、OPenCVとか、どうなるんだろうかぁ  

[■Windows版Darknetでyolo2.0の学習開始]  

https://pjreddie.com/darknet/yolo/  
↑ここの"Training YOLO On VOC"を頼りにデータをそろえる  

Pre-Trained な重みを使えと出ているが、1から重みを作ってみる  

./darknet yolo train cfg/yolo.2.0.cfg  
/deta/voc/train.txtが見つからないエラー  

src/yolo.cにイメージのパスがハードコーディングされている、、、びっくり  
修正  
make  

動き出したみたい  

Logを眺めるが、QNAN...とか数値になっていない結果が出てる  
150万枚の画像なんで、やってみるだけ無駄だけど、しばらく様子をみる  
