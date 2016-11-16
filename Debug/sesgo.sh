#!/bin/bash
declare -a problema=("xor" "cpu" "sin")


for prob in "${problema[@]}"
do
	echo "Sesgo, Media, Desv, Media, Desv" > sesgo_error_$prob.csv
	
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

	salida_ses=$(./practica1 -t ../BasesDatos/dat/train_$prob.dat -T ../BasesDatos/dat/test_$prob.dat -i 1000 -l $capa -h $neuronas -e 0.1 -m 0.9 -b | sed -n -e 's/Error de \([^ \t]*\) (Media +- DT):[ \t]*\([^ \t]*\)[ \t]*[^ \t]*[ \t]*\(.*\)/\1 \2, \3/p')
	
	errorTrain_ses=$(echo $salida_ses | grep "entrenamiento" | sed -n -e 's/entrenamiento[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\).*/\1, \2/p')
	errorTest_ses=$(echo $salida_ses  | grep "test " | sed -n -e 's/.*test[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\)/\1, \2/p')

	salida=$(./practica1 -t ../BasesDatos/dat/train_$prob.dat -T ../BasesDatos/dat/test_$prob.dat -i 1000 -l $capa -h $neuronas -e 0.1 -m 0.9  | sed -n -e 's/Error de \([^ \t]*\) (Media +- DT):[ \t]*\([^ \t]*\)[ \t]*[^ \t]*[ \t]*\(.*\)/\1 \2, \3/p')
	
	errorTrain=$(echo $salida | grep "entrenamiento" | sed -n -e 's/entrenamiento[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\).*/\1, \2/p')
	errorTest=$(echo $salida  | grep "test " | sed -n -e 's/.*test[ \t]*\([^ \t]*\), [ \t]*\([^ \t\n]*\)/\1, \2/p')

	echo "activado, $errorTrain_ses, $errorTest_ses" >> sesgo_error_$prob.csv
	echo "desactivado, $errorTrain, $errorTest" >> sesgo_error_$prob.csv
done




#| sed -n -e 's/Iteración[ \t]*\([^ \t]*\)[ \t]*Error de entrenamiento:[ \t]*\(.*\)/\1, \2/p')
#echo "$salida" >> convergencia_$prob.csv
