# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps color blacktext "Times-Roman-Bold" 20
set grid
set key box lt -1 lw 2
#set key center top

set xrange [0:3000]
set xtics 300 font "Times-Roman-Bold,20"
set yrange [0:40]
set ytics font "Times-Roman-Bold,20"
set ylabel "Throughput [packets/s]" font "Times-Roman-Bold,24"
set xlabel "# endNodes" font "Times-Roman-Bold,24"
set output './TestResult/throughput_multClass.eps'
#set key left top
#set encoding utf8
set key reverse vertical left

set multiplot

# Filename of the data
filename='TestResult/result-math.dat'
filename1='TestResult/result-sim.dat'
#filename2='TestResult/result-multClass_2-SF9.dat'
#filename3='TestResult/result-multClass_2_2-SF7.dat'
#filename4='TestResult/result-multClass_2_2-SF8.dat'
#filename5='TestResult/result-multClass_2_2-SF9.dat'
#filename6='~/Doutorado/projectLoRa/result_OpenField_1GW/test26/traffic-600/result-regSTAs.dat'
#filename7='~/Doutorado/projectLoRa/result_OpenField_1GW/test27/traffic-600/result-regSTAs.dat'
#filename8='~/Doutorado/projectLoRa/result_OpenField_1GW/test28/traffic-600/result-regSTAs.dat'
#filename9='~/Doutorado/projectLoRa/result_OpenField_1GW/test29/traffic-600/result-regSTAs.dat'




# Plot the data
#plot filename using 1:2 w lp lw 4 t 'Regular whitout alarm', filename1 using 1:2 w lp lw 4 t 'Regular with alarm: a-i', filename2 using 1:2 w lp lw 4 t 'Regular with alarm: a-ii', filename3 using 1:2 w lp lw 4 t 'Regular with alarm: a-iii', filename4 using 1:2 w lp lw 4 t 'Regular with alarm: b-i', filename5 using 1:2 w lp lw 4 t 'Regular with alarm: b-ii', filename6 using 1:2 w lp lw 4 t 'Regular with alarm: b-iii', filename7 using 1:2 w lp lw 4 t 'Regular with alarm: c-i', filename8 using 1:2 w lp lw 4 t 'Regular with alarm: c-ii', filename9 using 1:2 w lp lw 4 t 'Regular with alarm: c-iii'

plot filename using 1:5 w l lc 6 lw 4 t 'sf-7', filename using 1:5 lt 2 lc 6 lw 4 notitle, filename using 1:6 w l lc 2 lw 4 t 'sf-8', filename1 using 1:6 lt 2 lc 2 lw 4 notitle, filename using 1:7 w l lc 7 lw 4 t 'sf-9', filename1 using 1:7 lt 2 lc 7 lw 4 notitle
#plot filename using 1:2 w lp lw 4 t 'm = 0', filename1 using 1:2 w lp lw 4 t 'm = 1'



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

