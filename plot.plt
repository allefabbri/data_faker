#!/usr/bin/gnuplot
reset
set terminal pngcairo dashed size 900,700 enhanced font 'Verdana,10'
set output 'test.png'
set size ratio 1
set multiplot layout 2, 2 title "Data 3d dispersion" font ",14"
# Styles
linew = 1.2
points = 0.3
set style line  11 lc rgb '#0072bd' linetype  1 linewidth linew   # blue
set style line  12 lc rgb '#d95319' linetype  1 linewidth linew   # orange
set style line  15 lc rgb '#77ac30' linetype  1 linewidth linew   # green
set style line  21 lc rgb '#0072bd' pointtype 7 pointsize points  # blue
set style line  22 lc rgb '#d95319' pointtype 7 pointsize points  # orange
set style line  23 lc rgb '#77ac30' pointtype 7 pointsize points  # green
set style line 101 lc rgb '#000000' lt 1 lw 1    # black
set style line 102 lc rgb '#d6d7d9' lt 1 lw 1    # gray
# Axes label 
range = 1.2
set xrange [-range:range]
set yrange [-range:range]
# Border xy 
set border 3 front ls 101
set tics nomirror out scale 0.75
set format '%g'
set border linewidth 1.5
# Grid 
set x2tics -2, 0.25, 2 scale 0 format " "
set y2tics -2, 0.25, 2 scale 0 format " "
set grid x2tics y2tics back ls 102
# Legend
set key top
# Plot
input = "fake_data.txt"
set size 0.5, 0.5
set xlabel 'x'
set ylabel 'y'
plot input using 2:3 notitle with points ls 21
set xlabel 'z'
set ylabel 'y'
plot input using 4:3 notitle with points ls 22
set xlabel 'x'
set ylabel 'z'
plot input using 2:4 notitle with points ls 23
