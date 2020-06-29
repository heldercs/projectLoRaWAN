# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps color blacktext "Times-Roman-Bold" 16
set grid
#set key box lt -1 lw 2
#set key center top

set xrange [0:2000]
set xtics 200 font "Times-Roman-Bold,18"
set yrange [0:10]
set ytics font "Times-Roman-Bold,18"
set ylabel "Throughput Kbps" font "Times-Roman-Bold,20"
set xlabel "# endNodes" font "Times-Roman-Bold,20"
set output './TestResult/throughput_multClass.eps'
#set key left top
#set encoding utf8
set key reverse vertical right

# line styles
set style line 1 lc rgb '#0000ff' lw 3 # blue
set style line 2 lc rgb '#00ff00' lw 3 # green
set style line 3 lc rgb '#ff0000' lw 3 # red
set style line 4 dt 2 lc rgb '#000000' lw 3 # black
set style line 5 dt 3 lc rgb '#7f7f7f' lw 3 # red
set style line 6 lt 2 lc rgb '#0000ff' lw 3 # blue
set style line 7 lt 2 lc rgb '#00ff00' lw 3 # green
set style line 8 lt 2 lc rgb '#ff0000' lw 3 # red
set style line 9 lt 2 lc rgb '#00000' lw 3 # red


set multiplot

# Filename of the data
filename='TestResult/result-math-SF789.dat'
filename1='TestResult/result-math-SF789_EAB.dat'
filename2='TestResult/result-math-SF789_Size.dat'
filename3='TestResult/result-math-SF7.dat'
#filename4='TestResult/result-multClass_2_2-SF8.dat'
#filename5='TestResult/result-multClass_2_2-SF9.dat'
#filename6='~/Doutorado/projectLoRa/result_OpenField_1GW/test26/traffic-600/result-regSTAs.dat'
#filename7='~/Doutorado/projectLoRa/result_OpenField_1GW/test27/traffic-600/result-regSTAs.dat'
#filename8='~/Doutorado/projectLoRa/result_OpenField_1GW/test28/traffic-600/result-regSTAs.dat'
#filename9='~/Doutorado/projectLoRa/result_OpenField_1GW/test29/traffic-600/result-regSTAs.dat'


# Plot the data
#plot filename using 1:2 w lp lw 4 t 'Regular whitout alarm', filename1 using 1:2 w lp lw 4 t 'Regular with alarm: a-i', filename2 using 1:2 w lp lw 4 t 'Regular with alarm: a-ii', filename3 using 1:2 w lp lw 4 t 'Regular with alarm: a-iii', filename4 using 1:2 w lp lw 4 t 'Regular with alarm: b-i', filename5 using 1:2 w lp lw 4 t 'Regular with alarm: b-ii', filename6 using 1:2 w lp lw 4 t 'Regular with alarm: b-iii', filename7 using 1:2 w lp lw 4 t 'Regular with alarm: c-i', filename8 using 1:2 w lp lw 4 t 'Regular with alarm: c-ii', filename9 using 1:2 w lp lw 4 t 'Regular with alarm: c-iii'

#plot filename using 1:6 w l ls 1 t 'SF = 7', filename using 1:7 w l ls 2 t 'SF = 8', filename using 1:8 w l ls 3 t 'SF = 9', filename using 1:9 w l ls 4 t 'average', filename3 using 1:3 w l ls 5 t 'only SF = 7' 
plot filename using 1:9 w l ls 1 t 'avg SF789', filename1 using 1:9 w l ls 2 t 'avg SF789-EAB', filename2 using 1:9 w l ls 3 t 'avg SF789-Size', filename3 using 1:3 w l ls 5 t 'only SF = 7' 




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

