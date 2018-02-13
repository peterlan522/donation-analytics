#!/bin/bash
#
# Use this shell script to compile (if necessary) your code and then execute it. Below is an example of what might be found in this file if your program was written in Python
#

cd ./src
g++ -std=c++11 donation_analytics_main.cpp -o donation_analytics_main
cd ..

./src/donation_analytics_main ./input/itcont.txt ./input/percentile.txt ./output/repeat_donors.txt

