#!/bin/bash

echo "---------------启动 gs.sh--------------"

echo "---------------删除 output/2.csv-------"

rm output/2.csv

echo "---------------开始运行 gs.cpp!--------"

echo "请稍等..."

./gs

echo "---------------运行结束!---------------"

echo "---------------将生成的2.csv复制到plot文件夹里------"

cp output/2.csv ../plot/
