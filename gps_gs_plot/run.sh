#!/bin/bash

echo "开始运行run.sh"
echo "进入csv文件夹"
cd csv/
./csv.sh

echo "进入gs文件夹"
cd ../gs/
./gs.sh

echo "进入plot文件夹"
cd ../plot/
./plot.sh

echo "run.sh运行结束!"

