#!/bin/bash

if [ $# -ne 2 ]
then
	echo "parameters missing"
	exit 1
fi

trial=$1
interval=$2

if [ ! -d TestResult/ ]
then
	echo "No such directory"
	exit 1
fi

count=0;
total=0;
val=0;


for f in {7..12..1}
do
	touch ./TestResult/test$trial/traffic-$interval/result-SF$f.dat
	output="./TestResult/test$trial/traffic-$interval/result-SF$f.dat"
	echo "#numSta, Throughput(Kbps), ProbSucc(%), ProbLoss(%), avgDelay(Seconds)" > $output
	
	file="./TestResult/test$trial/traffic-$interval/result-STAs-SF$f.dat"

	if [ ! -f $file ]
	then 
		echo "no such file $file"
		exit 1
	fi

	for j in {300..2000..100}
	do
		printf "$j, " >> $output
		for k in {2..5..1}
		do
			for i in $( cat $file | grep -w $j | awk -v l="$k" '{ print $l; }' | sed 's/,//g' )
   			do 
	   	 		total=$( echo $total+$i | bc )
				((count++))
   			done
			val=$( echo "scale=5; $total/$count" | bc)
			if [ $k -eq 5 ]
			then
				printf  "$val" >> $output
			else
				printf "$val, " >> $output
			fi
			total=0
			count=0
		done
		printf "\n" >> $output
	done
done
