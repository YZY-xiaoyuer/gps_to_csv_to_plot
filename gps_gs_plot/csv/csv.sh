#!/bin/bash

echo "---------------启动 csv.sh--------------"

echo "---------------删除 output/1.csv-------"

rm output/1.csv

echo "---------------开始运行 csv.cpp!--------"

echo "请稍等..."

./csv
echo "---------------运行结束!---------------"

echo "---------------将生成的1.csv复制到gs文件夹里------"

cp output/1.csv ../gs/



