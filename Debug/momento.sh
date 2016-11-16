#!/bin/bash
declare -a problema=("xor" "cpu" "sin")


for prob in "${problema[@]}"
do
	echo "Momento, Media, Desv, Media, Desv" > momento_error_$prob.csv
	
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
	for momento in 0.0 0.1 0.5 0.9
	do
		salida=$(./practica1 -t ../BasesDatos/dat/train_$prob.dat -T ../BasesDatos/dat/test_$prob.dat -i 1000 -l $capa -h $neuronas -e 0.1 -m $momento -b  | sed -n -e 's/Error de \([^ \t]*\) (Media +- DT):[ \t]*\([^ \t]*\)[ \t]*[^ \t]*[ \t]*\(.*\)/\1 \2, \3/p')
		
		errorTrain=$(echo $salida | grep "entrenamiento" | sed -n -e 's/entrenamiento[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\).*/\1, \2/p')
		errorTest=$(echo $salida  | grep "test " | sed -n -e 's/.*test[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\)/\1, \2/p')

		echo "$momento, $errorTrain, $errorTest" >> momento_error_$prob.csv
	done
done




#| sed -n -e 's/Iteración[ \t]*\([^ \t]*\)[ \t]*Error de entrenamiento:[ \t]*\(.*\)/\1, \2/p')
#echo "$salida" >> convergencia_$prob.csv
