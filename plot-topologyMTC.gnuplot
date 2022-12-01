# Reset all previously set options
reset

# Set terminal up
#set term pngcairo font "FreeSans, 10" size 1024, 768
set term postscript eps enhanced color blacktext "Times-New-Roman" 18
set output outFile

set style line 3 lc rgb 'black' pt 9 ps 6 

set boxwidth 1 relative

set key spacing 2.2

#set xlabel "Distance (m)" font "FreeSans,16"
#set ylabel "Distance (m)" font "FreeSans,16"

#set style fill solid border 0.2
#set key box width 2 height 2 opaque
set key outside 
set key center top
set key horizontal Right

#set key samplen 2

# Use a good looking palette
#set palette maxcolors 6
#set palette defined (0.0 "#ff0000",\
# 0.5 "#ff0000",\
# 0.5001 "#ff7f00",\
# 1.5 "#ff7f00",\
# 1.5001 "#ffff00",\
# 2.5 "#ffff00",\
# 2.5001 "#00ff00",\
# 3.5 "#00ff00",\
# 3.5001 "#0000ff",\
# 4.5 "#0000ff",\
# 4.5001 "#4b0082",\
# 5.0 "#4b0082")

# Use a good looking palette
set palette maxcolors 6
set palette defined (1 "#e40303",\
2 "#ff8c00",\
3 "#ffed00",\
4 "#008026",\
5 "#004dff",\
6 "#750787")


#set cblab 'Spreading Factor' 
#font "freeSans,16"

#set xtics offset -1.2,0

set cbrange [6.5:12.5]

# Set up style for buildings
set style rect fc lt -1 fs solid 0.15 noborder

# Filename of the data
#filename='endDevices.dat'

# load the building locations
#load 'buildings.txt'

# Plot the data
#plot filename_edR using 1:2:3 with points ps 2 pt 7 lc 'black' t 'ED Regular', filename_edR using 1:2:3 with points ps 2 pt 7 palette notitle, filename_edA using 1:2:3 with points ps 4 pt 11 palette notitle, filename_edA using 1:2:3 with points ps 4 pt 10 lw 5 lc 'black' t 'ED Alarm', filename_gw using 1:2 with points ls 3 t 'Gateway'
#plot filename_edR using 1:2:3 with points ps 2 pt 7 lc 'black' t 'ED Regular', filename_edR using 1:2:3 with points ps 2 pt 7 palette notitle, filename_edA using 1:2:3 with points ps 4 pt 11 palette notitle, filename_edA using 1:2:3 with points ps 4 pt 10 lw 5 lc 'black' t 'ED Alarm', filename_gw using 1:2 with points ls 3 t 'Gateway', filename_gw with boxes fs solid 1 lc rgb 'grey90' lw 16 t 'Obstacles'
plot filename_ed using ($1/1000):($2/1000):3 with points lw 2 ps 2 pt 6 lc 'black' t 'Nodes', filename_ed using ($1/1000):($2/1000):3 with points ps 1.2 pt 7 palette notitle, filename_gw using 1:2 with points ls 3 t 'Gateway'
#plot filename_edR using ($1/1000):($2/1000):3 with points ps 2 pt 6 lc 'black' t 'Regular', filename_edR using ($1/1000):($2/1000):3 with points pt 7 palette notitle, filename_edA using ($1/1000):($2/1000):3 with points ps 4 pt 11 palette notitle, filename_edA using ($1/1000):($2/1000):3 with points ps 4 pt 10 lw 5 lc 'black' t 'Alarme', filename_gw using ($1/1000):($2/1000) with points ls 3 t 'Gateway'
