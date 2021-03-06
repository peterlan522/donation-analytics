#!/bin/bash
#
# This is the shell script to compile the code and then execute it.
#

cd ./src
g++ -std=c++11 donation_analytics_main.cpp -o donation_analytics_main
cd ..

./src/donation_analytics_main ./input/itcont.txt ./input/percentile.txt ./output/repeat_donors.txt

