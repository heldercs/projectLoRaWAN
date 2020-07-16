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
set yrange [0:6]
set ytics font "Times-Roman-Bold,18"
set ylabel "Throughput Kbps" font "Times-Roman-Bold,20"
set xlabel "# endNodes" font "Times-Roman-Bold,20"
set output './TestResult/throughput_multClass_2.eps'
#set key left top
#set encoding utf8
set key reverse vertical right

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


set multiplot

# Filename of the data
filename='./TestResult/result-math-SF78_PLB.dat'
filename1='./TestResult/result-math-SF78_EAB.dat'
filename2='./TestResult/result-math-SF78_NIB.dat'
filename3='./TestResult/result-math-SF78_FIB.dat'


# Plot the data
#plot filename using 1:9 w l ls 1 t 'SF789', filename5 using 1:9 ls 7 notitle, filename1 using 1:9 w l ls 2 t 'SF789-Size', filename6 using 1:9 ls 8 notitle, filename2 using 1:9 w l ls 3 t 'SF789-EAB', filename7 using 1:9 ls 9 notitle, filename3 using 1:9 w l ls 4 t 'avg SF789-EAB-Size', filename8 using 1:9 ls 10 notitle, filename4 using 1:3 w l ls 6 t 'only SF = 7', filename9 using 1:3 ls 6 notitle

plot filename using 1:7 w l ls 1 t 'PLB', filename1 using 1:7 w l ls 2 t 'EAB', filename2 using 1:7 w l ls 3 t 'NIB', filename3 using 1:7 w l ls 4 t 'FIB'

