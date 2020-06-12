# Reset all previously set options
reset

# Set terminal up
set term pngcairo font "FreeSans, 10" size 1024, 768
#set term postscript eps color blacktext "FreeSans-Bold" 10
set grid
set key box lt -1 lw 2
set xtics 100
set xrange [500:2000]
set xtics font ",10"
set yrange [20:35]
set ytics font ",10"
set ylabel "Throughput (pcts/s)" font "Times-Roman-Bold,16"
set xlabel "# endNodes" font "times-Roman-Bold,16"
set output './TestResult/throughput_1GW.png'
set key left top
#set encoding utf8

#set multiplot

# Filename of the data
filename='TestResult/result-STAs.dat'
filename1='TestResult/result-regSTAs.dat'
#filename2='TestResult/result-almSTAs.dat'
#filename3='~/Doutorado/projectLoRa/result_OpenField_1GW/test23/traffic-600/result-regSTAs.dat'
#filename4='~/Doutorado/projectLoRa/result_OpenField_1GW/test24/traffic-600/result-regSTAs.dat'
#filename5='~/Doutorado/projectLoRa/result_OpenField_1GW/test25/traffic-600/result-regSTAs.dat'
#filename6='~/Doutorado/projectLoRa/result_OpenField_1GW/test26/traffic-600/result-regSTAs.dat'
#filename7='~/Doutorado/projectLoRa/result_OpenField_1GW/test27/traffic-600/result-regSTAs.dat'
#filename8='~/Doutorado/projectLoRa/result_OpenField_1GW/test28/traffic-600/result-regSTAs.dat'
#filename9='~/Doutorado/projectLoRa/result_OpenField_1GW/test29/traffic-600/result-regSTAs.dat'




# Plot the data
#plot filename using 1:2 w lp lw 4 t 'Regular whitout alarm', filename1 using 1:2 w lp lw 4 t 'Regular with alarm: a-i', filename2 using 1:2 w lp lw 4 t 'Regular with alarm: a-ii', filename3 using 1:2 w lp lw 4 t 'Regular with alarm: a-iii', filename4 using 1:2 w lp lw 4 t 'Regular with alarm: b-i', filename5 using 1:2 w lp lw 4 t 'Regular with alarm: b-ii', filename6 using 1:2 w lp lw 4 t 'Regular with alarm: b-iii', filename7 using 1:2 w lp lw 4 t 'Regular with alarm: c-i', filename8 using 1:2 w lp lw 4 t 'Regular with alarm: c-ii', filename9 using 1:2 w lp lw 4 t 'Regular with alarm: c-iii'

plot filename using 1:2 w lp lw 4 t 'Regular whitout alarm', filename1 using 1:2 w lp lw 4 t 'Regular with alarm'


#plot filename using 1:2 w lp lw 4 t 'systemic-Throughput', filename using 1:3 w lp lw 4 t 'effective-Throughput', filename using 1:4 w lp lw 4 t 'mean-Throughput'

#reset 
#set grid
#set origin 0.55,0.21
#set size 0.4,0.45
#set title 'zoom'
#set xlabel "# endNodes"
#set ylabel ""
#set xrange [1000:1500]
#set yrange [0.0001:0.0003]

#plot filename using 1:2 w lp lw 4 notitle, filename1 using 1:2 w lp lw 4 notitle, filename2 using 1:2 w lp lw 4 notitle, filename3 using 1:2 w lp lw 4 notitle, filename4 using 1:2 w lp lw 4 notitle, filename5 using 1:2 w lp lw 4 notitle, filename6 using 1:2 w lp lw 4 notitle, filename7 using 1:2 w lp lw 4 notitle, filename8 using 1:2 w lp lw 4 notitle, filename9 using 1:2 w lp lw 4 notitle

#plot filename using 1:2 w lp lw 4 notitle, filename using 1:3 w lp lw 4 notitle, filename using 1:4 w lp lw 4 notitle

