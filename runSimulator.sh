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
pEDs=$7
trial=$8

echo "##### Simulation Start #####"

if [ ! -d TestResult/ ]
then
	mkdir TestResult/
fi

RANDOM=$$

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

if [ ! -d TestResult/test$trial/ ]
	then
	mkdir TestResult/test$trial/
fi

if [ ! -d TestResult/test$trial/traffic-$interval/ ]
	then
	mkdir TestResult/test$trial/traffic-$interval/
fi

if [ $1 -eq 0 ]
then

	touch ./TestResult/test$trial/traffic-$interval/result-STAs.dat
	file1="./TestResult/test$trial/traffic-$interval/result-STAs.dat"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > ./TestResult/test$trial/traffic-$interval/result-STAs.dat 
   	
	touch ./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt
	file2="./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt"

	for numSta in {0..10..2}
	do
			echo -ne "trial:$trial-numSTA:$numSta #"

			if [ ! -d TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/ ]
			then
				mkdir TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/
			fi

			touch TestResult/test$trial/time-record$numSta.txt

			echo "Time: $(date) $interval $numSta" >> TestResult/test$trial/time-record$numSta.txt

		for numSeed in {1..5}
		do
			echo -ne "$numSeed \r"
  			./waf --run "lorawan-network-sim --nSeed=$RANDOM --nDevices=$numSta --nGateways=$gwRing --radius=$rad --gatewayRadius=$gwRad --simulationTime=$simTime --appPeriod=$interval --file1=$file1 --file2=$file2 --printEDs=$pEDs --trial=$trial"  > ./TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/record-$numSta.txt 2>&1
		done
	echo ""
	done
elif [ $1 -eq 1 ]
then

	file1="./TestResult/test$trial/traffic-$interval/result-STAs"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > ./TestResult/test$trial/traffic-$interval/result-STAs-SF7.dat 
		
	touch ./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt
	file2="./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt"


	for numSta in {1..2..1}
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
 			./waf --run "lorawan-network-mClass-sim --nSeed=$RANDOM --nDevices=$numSta --nGateways=$gwRing --radius=$rad --gatewayRadius=$gwRad --simulationTime=$simTime --appPeriod=$interval --file1=$file1 --file2=$file2 --printEDs=$pEDs --trial=$trial" > ./TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/record-$numSta.txt 2>&1
		done
	done
elif [ $1 -eq 2 ]
then

	file1="./TestResult/test$trial/traffic-$interval/result-regSTAs"
	#echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > ./TestResult/test$trial/traffic-$interval/result-regSTAs.dat 
	
	touch ./TestResult/test$trial/traffic-$interval/mac-almSTAs-GW-$gwRing.txt
	file4="./TestResult/test$trial/traffic-$interval/mac-almSTAs-GW-$gwRing.txt"

	for numSta in {100..4000..100}
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
  			./waf --run "lorawan-network-wAlm-sim --nSeed=$RANDOM --nDevices=$numSta --nGateways=$gwRing --radius=$rad --gatewayRadius=$gwRad --simulationTime=$simTime --appPeriod=$interval --file1=$file1 --file2=$file2 --printEDs=$pEDs --trial=$trial"  > ./TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/record-$numSta.txt 2>&1
		done
	done
else

	touch ./TestResult/test$trial/traffic-$interval/result-STAs-SF7.dat
	file1="./TestResult/test$trial/traffic-$interval/result-STAs-SF7.dat"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > ./TestResult/test$trial/traffic-$interval/result-STAs-SF7.dat 
		
	touch ./TestResult/test$trial/traffic-$interval/result-STAs-SF8.dat
	file2="./TestResult/test$trial/traffic-$interval/result-STAs-SF8.dat"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > ./TestResult/test$trial/traffic-$interval/result-STAs-SF8.dat 
	
	touch ./TestResult/test$trial/traffic-$interval/result-STAs-SF9.dat
	file3="./TestResult/test$trial/traffic-$interval/result-STAs-SF9.dat"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > ./TestResult/test$trial/traffic-$interval/result-STAs-SF9.dat 	

	touch ./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt
	file4="./TestResult/test$trial/traffic-$interval/mac-STAs-GW-$gwRing.txt"


	for numSta in {100..4000..100}
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
  			./waf --run "lorawan-network-wAlm-mClass-sim --nSeed=$RANDOM --nDevices=$numSta --nGateways=$gwRing --radius=$rad --gatewayRadius=$gwRad --simulationTime=$simTime --appPeriod=$interval --file1=$file1 --file2=$file2 --file3=$file3 --file4=$file4 --printEDs=$pEDs --trial=$trial" > ./TestResult/test$trial/traffic-$interval/pcap-sta-$numSta/record-$numSta.txt 2>&1
		done
	done


fi
echo "##### Simulation finish #####"
#echo "seinding email..."
#echo simulation finish | mail -s Simulator helderhdw@gmail.com


