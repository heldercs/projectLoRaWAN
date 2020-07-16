# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps color blacktext "Times-Roman-Bold" 16
set grid
#set key box lt -1 lw 2
set xrange [0:2000]
set xtics 400 font "Times-Roman-Bold,18"
set yrange [0.1:1]
set ytics font "times-Roman-bold,18"
set ylabel "Packet Success Probability (%)" font "Times-Roman-Bold,20"
set xlabel "# endNodes" font "times-Roman-Bold,20"
set output './TestResult/probSuccess_multClass_3.eps'
#set key bottom
set key reverse vertical right
set format y "10^{%L}"
set grid ytics mytics
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
filename='./TestResult/result-math-SF789_PLB.dat'
filename1='./TestResult/result-math-SF789_EAB.dat'
filename2='./TestResult/result-math-SF789_NIB.dat'
filename3='./TestResult/result-math-SF789_FIB.dat'


# Plot the data
#plot filename using 1:5 w l ls 1 t 'avg SF789', filename5 using 1:5 ls 7 notitle, filename1 using 1:5 w l ls 2 t 'avg SF789-Size', filename6 using 1:5 ls 8 notitle, filename2 using 1:5 w l ls 3 t 'avg SF789-EAB', filename7 using 1:5 ls 9 notitle, filename3 using 1:5 w l ls 4 t 'avg SF789-EAB-Size', filename8 using 1:5 ls 10 notitle, filename4 using 1:2 w l ls 6 t 'only SF = 7', filename9 using 1:2 ls 6 notitle

plot filename using 1:5 w l ls 1 t 'avg PLB', filename1 using 1:5 w l ls 2 t 'svg EAB', filename2 using 1:5 w l ls 3 t 'avg NIB', filename3 using 1:5 w l ls 4 t 'avg FIB'



#, filename2 using 1:5 w l ls 3 t 'avg SF789-080', filename3 using 1:5 w l ls 5 t 'avg SF789-MIX'




