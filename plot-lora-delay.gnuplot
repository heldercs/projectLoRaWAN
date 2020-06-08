# Reset all previously set options
reset

#set terminal png size 800,500 enhanced font "Helvetica,20"
#set output './TestResult/delay_IIP_Alarms.png'

set term postscript eps color blacktext "Times-Roman-Bold" 20
set output './TestResult/delay_IIP_Alarms.eps'

red = "#FF0000"; green = "#00FF00"; blue = "#0000FF"; skyblue = "#87CEEB";
gray = "#6D666B"; black = "#000000"; aquamarine = "#458b74"; sea = "#3cb371"; pale = "#db7093"; 

set yrange [0:300]
set style line 2 lc rgb 'blue' lt 1 lw 1
set style data histogram
set style histogram cluster gap 1
#set style fill solid
set style fill pattern border -1 
set boxwidth 0.9
set ylabel "Average delay (milliSeconds)" offset 1 font "Times-Roman-Bold,24"
set xlabel "end-nodes" font "Times-Roman-Bold,24"
#set xtics format ''
set grid ytics
set datafile separator ","

set key left top
set key reverse vertical Left

# Filename of the data
filename1='TestResult/result-IIP-Alm-ai.dat'
filename2='TestResult/result-IIP-Alm-aii.dat'
filename3='TestResult/result-IIP-Alm-aiii.dat'
filename4='TestResult/result-IIP-Alm-bi.dat'
filename5='TestResult/result-IIP-Alm-bii.dat'
filename6='TestResult/result-IIP-Alm-biii.dat'
filename7='TestResult/result-IIP-Alm-ci.dat'
filename8='TestResult/result-IIP-Alm-cii.dat'
filename9='TestResult/result-IIP-Alm-ciii.dat'

# set title "A Sample Bar Chart"
plot filename1 using 4:xtic(1) title "Alarm: a-i" ls 2, \
            filename2 using 4 title "Alarm: a-ii" ls 2, \
            filename3 using 4 title "Alarm: a-iii" ls 2, \
		 	filename4 using 4 title "Alarm: b-ii" ls 2, \
 			filename5 using 4 title "Alarm: b-ii" ls 2, \
 			filename6 using 4 title "Alarm: b-ii" ls 2, \
 			filename7 using 4 title "Alarm: c-ii" ls 2, \
 			filename8 using 4 title "Alarm: c-ii" ls 2, \
            filename9 using 4 title "Alarm: c-iii" ls 2
