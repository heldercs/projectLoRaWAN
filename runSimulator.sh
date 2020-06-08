#!/bin/bash

if [ $# -ne 8 ]
then
echo "parameters missing"
exit 1
fi

gwRing=$2
rad=$3
gwRad=$4
simTime=$5
interval=$6
trial=$7
pEDs=$8


echo "##### Simulation Start #####"

if [ ! -d TestResult/ ]
	then
	mkdir TestResult/
fi


#if [ $trial -eq 2 ]
#then
#	gwRing=1
#	gwRad=0
#elif [ $trial -eq 1 ]
#then
#	gwRing=2
#	gwRad=0
#else
#	gwRing=2
#	gwRad=4000
#fi  

#for trial in 0
#do

if [ ! -d TestResult/test$trial/ ]
	then
	mkdir TestResult/test$trial/
fi

if [ ! -d TestResult/test$trial/traffic-$interval/ ]
	then
	mkdir TestResult/test$trial/traffic-$interval/
fi

if [ $1 -eq 1 ]
then
	touch ./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt
	file1="./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt"

	#touch ./TestResult/test$trial/traffic-$interval/result-STAs.dat
	#file2="./TestResult/test$trial/traffic-$interval/result-STAs.dat"
	#echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), ProbInter(%), ProbNoMo(%), ProbUSen(%)" > ./TestResult/test$trial/traffic-$interval/result-STAs.dat 
		
#	touch ./TestResult/test$trial/delay.dat
#	file3="./TestResult/test$trial/delay.dat"
#	echo "number STA: delay (in nanoseconds) " >> ./TestResult/test$trial/delay.dat

	for numSta in 500 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000 
	do
			echo "trial:$trial-numSTA:$numSta #"

			if [ ! -d TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/ ]
			then
				mkdir TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/
			fi

			touch TestResult/test$trial/time-record$numSta.txt

			echo "Time: $(date) $interval $numSta" >> TestResult/test$trial/time-record$numSta.txt

		for numSeed in {1..5}
		do
			echo -ne "$numSeed \r"
  			./waf --run "lorawan-network-class-sim --nSeed=$numSeed --nDevices=$numSta --gatewayRings=$gwRing --radius=$rad --gatewayRadius=$gwRad --simulationTime=$simTime --appPeriod=$interval --printEDs=$pEDs --file1=$file1 --trial=$trial"  > ./TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/record-$numSta.txt 2>&1
		done
	done
	#done
else
	touch ./TestResult/test$trial/traffic-$interval/mac-regSTAs-GW-$gwRing.txt
	file1="./TestResult/test$trial/traffic-$interval/mac-regSTAs-GW-$gwRing.txt"

	touch ./TestResult/test$trial/traffic-$interval/result-regSTAs.dat
	file2="./TestResult/test$trial/traffic-$interval/result-regSTAs.dat"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), ProbInter(%), ProbNoMo(%), ProbUSen(%)" > ./TestResult/test$trial/traffic-$interval/result-regSTAs.dat 
	
	touch ./TestResult/test$trial/traffic-$interval/mac-almSTAs-GW-$gwRing.txt
	file3="./TestResult/test$trial/traffic-$interval/mac-almSTAs-GW-$gwRing.txt"

	touch ./TestResult/test$trial/traffic-$interval/result-almSTAs.dat
	file4="./TestResult/test$trial/traffic-$interval/result-almSTAs.dat"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), ProbInter(%), ProbNoMo(%), ProbUSen(%), avgDelay(ms)" > ./TestResult/test$trial/traffic-$interval/result-almSTAs.dat 
	
	touch ./TestResult/test$trial/traffic-$interval/alarmsRtx.txt
	file5="./TestResult/test$trial/traffic-$interval/alarmsRtx.txt"
	echo "#numSta: vector Rtx" > ./TestResult/test$trial/traffic-$interval/alarmsRtx.txt 
		
#	touch ./TestResult/test$trial/delay.dat
#	file5="./TestResult/test$trial/delay.dat"
#	echo "number STA: delay (in nanoseconds) " >> ./TestResult/test$trial/delay.dat
	for numSta in 500 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000
	do
			echo "trial:$trial-numSTA:$numSta"

			if [ ! -d TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/ ]
			then
				mkdir TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/
			fi

			touch TestResult/test$trial/time-record$numSta.txt

			echo "Time: $(date) $interval $numSta" >> TestResult/test$trial/time-record$numSta.txt

		for numSeed in {1..5}
		do
			echo -ne "$numSeed \r"
  			./waf --run "lorawan-network-wAlm-sim --nSeed=$numSeed --nDevices=$numSta --gatewayRings=$gwRing --radius=$rad --gatewayRadius=$gwRad --simulationTime=$simTime --appPeriod=$interval --printEDs=$pEDs --file1=$file1 --file2=$file2 --file3=$file3 --file4=$file4 --file5=$file5 --trial=$trial"  > ./TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/record-$numSta.txt 2>&1
		done
	done
fi
#done
echo "##### Simulation finish #####"
#echo "seinding email..."
#echo simulation finish | mail -s Simulator helderhdw@gmail.com


