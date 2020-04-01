#!/bin/bash

echo "开始运行run.sh"
#===========================
echo "进入csv文件夹"
cd csv/
./csv.sh
cd ..
echo "将生成的文件拷贝到gs文件夹"
cp csv/output/1.csv gs/
#===========================
echo "进入gs文件夹"
cd gs/
./gs.sh
cd ..
#===========================
echo "1:opencv画图，目前只能画二维图像"
echo "2：python画图，可以画2维和3维图像，速度较慢"
echo "请选择画图工具"
read SW

case $SW in
1)
echo "将生成的文件拷贝到opencv_plot文件夹" 
cp gs/output/2.csv opencv_plot/
echo "进入opencv_plot文件夹"
cd opencv_plot/
./opencv_plot.sh
cd ..
;;
2)
echo "将生成的文件拷贝到python_plot文件夹" 
cp gs/output/2.csv python_plot/
echo "进入python_plot文件夹"
cd python_plot/
./python_plot.sh
cd ..
;;
esac

echo "run.sh运行结束!"

