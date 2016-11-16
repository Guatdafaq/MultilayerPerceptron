#!/bin/bash
declare -a problema=("xor" "cpu" "sin")


for prob in "${problema[@]}"
do
	echo "Iteración, ErrorTrain, ErrorTest" > convergencia_error_$prob.csv
	
	if [ $prob = "xor" ]
	then
		capa=2
		neuronas=100
	elif [ $prob = "cpu" ]
	then	
		capa=1
		neuronas=10
	elif [ $prob = "sin" ]
	then
		capa=1
		neuronas=50
	fi
		salida=$(./practica1 -t ../BasesDatos/dat/train_$prob.dat -T ../BasesDatos/dat/test_$prob.dat -i 1000 -l $capa -h $neuronas -e 0.1 -m 0.9 -b | sed -n -e 's/Iteración[ \t]*\([^ \t]*\)[ \t]*Error de entrenamiento:[ \t]*\([^ \t]*\)[ \t]*Error de test:[ \t]*\([^ \t]*\)/\1, \2, \3 /p')
		

		echo "$salida" >> convergencia_error_$prob.csv
done




#| sed -n -e 's/Iteración[ \t]*\([^ \t]*\)[ \t]*Error de entrenamiento:[ \t]*\(.*\)/\1, \2/p')
#echo "$salida" >> convergencia_$prob.csv
