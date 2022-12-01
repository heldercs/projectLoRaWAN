#!/bin/bash

if [ $# -ne 7 ]
then
echo "parameters missing"
exit 1
fi

numSta=$1
rad=$2
interval=$3
simTime=$4
ver=$5
trac=$6
trial=$7

echo "##### Simulation Start #####"

if [ ! -d TestResult/ ]
	then
	mkdir TestResult/
fi


if [ ! -d TestResult/test$trial/ ]
	then
	mkdir TestResult/test$trial/
fi

if [ ! -d TestResult/test$trial/traffic-$interval/ ]
	then
	mkdir TestResult/test$trial/traffic-$interval/
fi


touch ./TestResult/test$trial/traffic-$interval/result-sim-$numSta.dat
file="./TestResult/test$trial/traffic-$interval/result-sim-$numSta.dat"
echo "#nDevice, TX Total, RX Total, Throughput(pps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > ./TestResult/test$trial/traffic-$interval/result-sim-$numSta.dat 
   	

for numNode in {5..45..2}
do
	dev=$(($numSta+$numNode))
	echo "Num Devices: $dev"

    if [ ! -d TestResult/test$trial/traffic-$interval/pcap-sta-$dev/ ]
    then
       mkdir TestResult/test$trial/traffic-$interval/pcap-sta-$dev/
    fi

    touch TestResult/test$trial/time-record$dev.txt
	
	./waf --run "simNetMultUser --nCsma=$numNode --nWifi=$numSta --radius=$rad --simulationTime=$simTime --appInterval=$interval --file=$file --verbose=$ver --tracing=$trac" > ./TestResult/test$trial/traffic-$interval/pcap-sta-$dev/record-$numSta.txt 2>&1
done

#echo "##### Simulation finish #####"
#echo "seinding email..."
#'echo simulation finish | mail -s Simulator helderhdw@gmail.com


