# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps color blacktext "FreeSans-Bold" 10
set grid
set xrange [500:2000]
set xtics font ",10"
set yrange [0:110]
set ytics font ",10"
set ylabel "Prob. Success (%)" font "Times-Roman-Bold,16"
set xlabel "No. of EndDevices" font "times-Roman-Bold,16"
set output './TestResult/probSuccess_AlarmOpenField_2GW.eps'
set key bottom

set multiplot

# Filename of the data
filename1='~/Doutorado/projectLoRa/result_OpenField_2GW/test31/traffic-600/result-almSTAs.dat'
filename2='~/Doutorado/projectLoRa/result_OpenField_2GW/test32/traffic-600/result-almSTAs.dat'
filename3='~/Doutorado/projectLoRa/result_OpenField_2GW/test33/traffic-600/result-almSTAs.dat'
filename4='~/Doutorado/projectLoRa/result_OpenField_2GW/test34/traffic-600/result-almSTAs.dat'
filename5='~/Doutorado/projectLoRa/result_OpenField_2GW/test35/traffic-600/result-almSTAs.dat'
filename6='~/Doutorado/projectLoRa/result_OpenField_2GW/test36/traffic-600/result-almSTAs.dat'
filename7='~/Doutorado/projectLoRa/result_OpenField_2GW/test37/traffic-600/result-almSTAs.dat'
filename8='~/Doutorado/projectLoRa/result_OpenField_2GW/test38/traffic-600/result-almSTAs.dat'
filename9='~/Doutorado/projectLoRa/result_OpenField_2GW/test39/traffic-600/result-almSTAs.dat'

# Plot the data
plot filename1 using 1:3 w lp lw 4 t 'Alarm: a-i', filename2 using 1:3 w lp lw 4 t 'Alarm: a-ii', filename3 using 1:3 w lp lw 4 t 'Alarm: a-iii', filename4 using 1:3 w lp lw 4 t 'Alarm: b-i', filename5 using 1:3 w lp lw 4 t 'Alarm: b-ii', filename6 using 1:3 w lp lw 4 t 'Alarm: b-iii', filename7 using 1:3 w lp lw 4 t 'Alarm: c-i', filename8 using 1:3 w lp lw 4 t 'Alarm: c-ii', filename9 using 1:3 w lp lw 4 t 'Alarm: c-iii'
#plot filename1 using 1:3 w lp lw 4 t 'Alarm: a-i', filename2 using 1:3 w lp lw 4 t 'Alarm: a-ii', filename4 using 1:3 w lp lw 4 t 'Alarm: b-i', filename5 using 1:3 w lp lw 4 t 'Alarm: b-ii', filename7 using 1:3 w lp lw 4 t 'Alarm: c-i', filename8 using 1:3 w lp lw 4 t 'Alarm: c-ii'


#reset 
#set grid
#set origin 0.425,0.34
#set size 0.4,0.45
#set title 'zoom'
#set xlabel "End-Nodes"
#set ylabel ""
#set xrange [1150:1200]
#set yrange [99.9:100.1]

#plot filename1 using 1:3 w lp lw 4 notitle, filename2 using 1:3 w lp lw 4 notitle, filename3 using 1:3 w lp lw 4 notitle, filename4 using 1:3 w lp lw 4 notitle, filename5 using 1:3 w lp lw 4 notitle, filename6 using 1:3 w lp lw 4 notitle, filename7 using 1:3 w lp lw 4 notitle, filename8 using 1:3 w lp lw 4 notitle, filename9 using 1:3 w lp lw 4 notitle
