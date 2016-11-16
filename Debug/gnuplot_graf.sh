#!/bin/bash
declare -a problema=("xor" "cpu" "sin")


for prob in "${problema[@]}"
do

cat << _end_ | gnuplot
set terminal postscript eps color
set output "convergencia_$prob_graf.eps"
set key right bottom
set xlabel "Iteración"
set ylabel "Error cometido"
plot 'convergencia_$prob.csv' using 1:6 t "Entrenamiento" w l, 'convergencia_$prob.csv' using 1:7 t "Test" w l

_end_

done




#| sed -n -e 's/Iteración[ \t]*\([^ \t]*\)[ \t]*Error de entrenamiento:[ \t]*\(.*\)/\1, \2/p')
#echo "$salida" >> convergencia_$prob.csv
