#!/bin/bash
declare -a problema=("xor" "cpu" "sin")


for prob in "${problema[@]}"
do
	echo "Estructura, Media, Desv, Media, Desv" > error_$prob.csv
	for capa in 1 2
	do
		for neuronas in 2 5 10 25 50 100
		do
			salida=$(./practica1 -t ../BasesDatos/dat/train_$prob.dat -T ../BasesDatos/dat/test_$prob.dat -i 1000 -l $capa -h $neuronas -e 0.1 -m 0.9 -b | sed -n -e 's/Error de \([^ \t]*\) (Media +- DT):[ \t]*\([^ \t]*\)[ \t]*[^ \t]*[ \t]*\(.*\)/\1 \2, \3/p')
			errorTrain=$(echo $salida | grep "entrenamiento" | sed -n -e 's/entrenamiento[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\).*/\1, \2/p')
			errorTest=$(echo $salida  | grep "test " | sed -n -e 's/.*test[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\)/\1, \2/p')

			if [ $capa -eq 1 ]
			then
				echo "{n : $neuronas : k}, $errorTrain, $errorTest" >> error_$prob.csv
			else
				echo "{n : $neuronas : $neuronas : k}, $errorTrain, $errorTest" >> error_$prob.csv
			fi
		done
	done
done




#| sed -n -e 's/Iteración[ \t]*\([^ \t]*\)[ \t]*Error de entrenamiento:[ \t]*\(.*\)/\1, \2/p')
#echo "$salida" >> convergencia_$prob.csv
