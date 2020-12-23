# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps enhanced color blacktext "Times-Roman-Bold" 16
set grid
#set key box lt -1 lw 2
set xrange [0:1]
set xtics 0.2 font "Times-Roman-Bold,18"
set yrange [0:5]
set ytics font "times-Roman-bold,18"
set ylabel "Throughtput (packet/s)" font "Times-Roman-Bold,20"
set xlabel "n_1/(n_1+n_2)" font "times-Roman-Bold,20"
set output './TestResult/strategiesSF_multClass.eps'
#set key bottom
set key vertical left

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

set style line 11 dt 4 lc rgb '#7f7f7f' lw 3 # gray

#set multiplot

# Filename of the data
filename='./TestResult/result-math-2SF-str.dat'



# Plot the data

plot filename using 1:2 w l ls 1 t 'n = 1000', filename using 1:3 w l ls 6 t 'n = 1000', filename using 1:4 w l ls 11 t 'n = 1000', filename using 1:5 w l ls 5 t 'n = 1000'

#, filename4 using 1:2 w l ls 6 t 'only SF = 7'




