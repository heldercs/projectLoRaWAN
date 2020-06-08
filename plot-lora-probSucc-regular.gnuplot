# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps color blacktext "FreeSans-Bold" 10
set grid
set key box lt -1 lw 2
set xrange [500:2000]
set xtics font ",10"
set yrange [0:100]
set ytics font ",10"
set ylabel "Prob. Success (%)" font "Times-Roman-Bold,16"
set xlabel "End-Nodes" font "times-Roman-Bold,16"
set output './TestResult/probSuccess_RegularOpenField_2GW.eps'
set key bottom

set multiplot

# Filename of the data
filename='~/Doutorado/projectLoRa/result_OpenField_2GW/test30/traffic-600/result-STAs.dat'
filename1='~/Doutorado/projectLoRa/result_OpenField_2GW/test31/traffic-600/result-regSTAs.dat'
filename2='~/Doutorado/projectLoRa/result_OpenField_2GW/test32/traffic-600/result-regSTAs.dat'
filename3='~/Doutorado/projectLoRa/result_OpenField_2GW/test33/traffic-600/result-regSTAs.dat'
filename4='~/Doutorado/projectLoRa/result_OpenField_2GW/test34/traffic-600/result-regSTAs.dat'
filename5='~/Doutorado/projectLoRa/result_OpenField_2GW/test35/traffic-600/result-regSTAs.dat'
filename6='~/Doutorado/projectLoRa/result_OpenField_2GW/test36/traffic-600/result-regSTAs.dat'
filename7='~/Doutorado/projectLoRa/result_OpenField_2GW/test37/traffic-600/result-regSTAs.dat'
filename8='~/Doutorado/projectLoRa/result_OpenField_2GW/test38/traffic-600/result-regSTAs.dat'
filename9='~/Doutorado/projectLoRa/result_OpenField_2GW/test39/traffic-600/result-regSTAs.dat'

# Plot the data
plot filename using 1:3 w lp lw 4 t 'Regular without alarm', filename1 using 1:3 w lp lw 4 t 'Regular with alarm: a-i', filename2 using 1:3 w lp lw 4 t 'Regular with alarm: a-ii', filename3 using 1:3 w lp lw 4 t 'Regular with alarm a-iii', filename4 using 1:3 w lp lw 4 t 'Regular with alarm: b-i', filename5 using 1:3 w lp lw 4 t 'Regular with alarm: b-ii', filename6 using 1:3 w lp lw 4 t 'Regular with alarm: b-iii', filename7 using 1:3 w lp lw 4 t 'Regular with alarm: c-i',filename8 using 1:3 w lp lw 4 t 'Regular with alarm: c-ii', filename9 using 1:3 w lp lw 4 t 'Regular with alarm: c-iii'
#plot filename using 1:3 w lp lw 2 t 'Regular w/o alarm', filename1 using 1:3 w lp lw 2 t 'Regular w alarm: a-i', filename2 using 1:3 w lp lw 2 t 'Regular w alarm: a-ii', filename4 using 1:3 w lp lw 2 t 'Regular w alarm: b-i', filename5 using 1:3 w lp lw 2 t 'Regular w alarm: b-ii', filename7 using 1:3 w lp lw 2 t 'Regular w alarm: c-i',filename8 using 1:3 w lp lw 2 t 'Regular w alarm: c-ii'

reset 
set grid
set origin 0.425,0.4
set size 0.4,0.45
set title 'zoom'
set xlabel "End-Nodes"
set ylabel ""
set xrange [1150:1200]
set yrange [98.4:98.9]

plot filename using 1:3 w lp lw 4 notitle, filename1 using 1:3 w lp lw 4 notitle, filename2 using 1:3 w lp lw 4 notitle, filename3 using 1:3 w lp lw 4 notitle, filename4 using 1:3 w lp lw 4 notitle, filename5 using 1:3 w lp lw 4 notitle, filename6 using 1:3 w lp lw 4 notitle, filename7 using 1:3 w lp lw 4 notitle, filename8 using 1:3 w lp lw 4 notitle, filename9 using 1:3 w lp lw 4 notitle
