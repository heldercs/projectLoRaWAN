# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps color blacktext "Times-Roman-Bold" 16
set grid
#set key box lt -1 lw 2
set xrange [0:2400]
set xtics 300 font "Times-Roman-Bold,18"
set yrange [0.01:1]
set ytics font "times-Roman-bold,18"
set ylabel "Packet Success Probability (%)" font "Times-Roman-Bold,20"
set xlabel "# endNodes" font "times-Roman-Bold,20"
set output './TestResult/probSuccess_multClass-EAB.eps'
#set key bottom
set key reverse vertical right
set format y "10^{%L}"
set grid ytics
set logscale y 10

# line styles
set style line 1 lc rgb '#0000ff' lw 3 # blue
set style line 2 lc rgb '#00ff00' lw 3 # green
set style line 3 lc rgb '#ff0000' lw 3 # red
set style line 4 lc rgb '#000000' lw 3 # black
set style line 5 dt 2 lc rgb '#000000' lw 3 # black
set style line 6 dt 3 lc rgb '#7f7f7f' lw 3 # gray
set style line 7 lt 2 lc rgb '#0000ff' lw 3 # blue
set style line 8 lt 2 lc rgb '#00ff00' lw 3 # green
set style line 9 lt 2 lc rgb '#ff0000' lw 3 # red
set style line 10 lt 2 lc rgb '#000000' lw 3 # black


#set multiplot

# Filename of the data
#filename='./TestResult/result-math-SF789.dat'
#filename1='./TestResult/result-sim-SF789.dat'
#filename2='./TestResult/result-math-SF789_EAB.dat'
#filename3='./TestResult/result-sim-SF789_EAB.dat'
#filename4='./TestResult/result-math-SF7.dat'
#filename3='./TestResult/result-sim-SF7.dat'
#filename4='./TestResult/result-multClass_2_2-SF8.dat'
#filename5='./TestResult/result-multClass_2_2-SF9.dat'
#filename='~/Doutorado/projectLoRa/result-math-SF789.dat'
#filename1='~/Doutorado/projectLoRa/result-sim-SF789.dat'
filename='~/Doutorado/projectLoRa/result-math-SF789_EAB.dat'
filename1='~/Doutorado/projectLoRa/result-sim-SF789_EAB.dat'

# Plot the data
plot filename using 1:2 w l ls 1 t 'SF = 7', filename1 using 1:2 ls 7 notitle, filename using 1:3 w l ls 2 t 'SF = 8', filename1 using 1:3 ls 8 notitle, filename using 1:4 w l ls 3 t 'SF = 9', filename1 using 1:4 ls 9 notitle, filename using 1:5 w l ls 5 t 'average', filename1 using 1:5 ls 10 notitle 

#plot filename using 1:2 w l ls 1 t 'SF = 7', filename using 1:3 w l ls 2 t 'SF = 8', filename using 1:4 w l ls 3 t 'SF = 9', filename using 1:5 w l ls 4 t 'average', filename3 using 1:2 w l ls 5 t 'only SF = 7'
#plot filename using 1:5 w l ls 1 t 'avg SF789', filename1 using 1:5 w l ls 2 t 'avg SF789-Size', filename2 using 1:5 w l ls 3 t 'avg SF789-EAB', filename3 using 1:5 w l ls 4 t 'avg SF789-EAB-Size', filename4 using 1:2 w l ls 6 t 'only SF = 7'



#reset 
#set grid
#set origin 0.425,0.4
#set size 0.4,0.45
#set title 'zoom'
#set xlabel "End-Nodes"
#set ylabel ""
#set xrange [1150:1200]
#set yrange [98.4:98.9]

#plot filename using 1:3 w lp lw 4 notitle, filename1 using 1:3 w lp lw 4 notitle, filename2 using 1:3 w lp lw 4 notitle, filename3 using 1:3 w lp lw 4 notitle, filename4 using 1:3 w lp lw 4 notitle, filename5 using 1:3 w lp lw 4 notitle, filename6 using 1:3 w lp lw 4 notitle, filename7 using 1:3 w lp lw 4 notitle, filename8 using 1:3 w lp lw 4 notitle, filename9 using 1:3 w lp lw 4 notitle
