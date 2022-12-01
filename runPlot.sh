#!/bin/bash

echo "##### Start Plot throughput #####"
gnuplot plot-lora-throughput.gnuplot
echo "##### Finish Plot throughput #####"

echo "##### Start Plot ProbSucc #####"
gnuplot plot-lora-probSucc.gnuplot
echo "##### Finish Plot ProbSucc #####"

echo "##### Start Plot ProbLoss #####"
gnuplot plot-lora-probLoss.gnuplot
echo "##### Finish Plot ProbLoss #####"

echo "##### Start Plot endDevices #####"
# plot endDEvices
for trial in 0
do
	for numSta in 100 
	do
			echo "numSTA:$numSta"
			file1="'./data/endDevices$numSta.png'"
			file2="'./data/endDevices$numSta.dat'"

			gnuplot -e "outFile=$file1; filename=$file2" plot-end-devices.gnuplot	

	done
done	
echo "##### Finish Plot endDevices #####"

