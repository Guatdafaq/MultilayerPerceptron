//============================================================================
// Introducción a los Modelos Computacionales
// Name        : practica1.cpp
// Author      : Pedro A. Gutiérrez
// Version     :
// Copyright   : Universidad de Córdoba
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <ctime>    // Para cojer la hora time()
#include <cstdlib>  // Para establecer la semilla srand() y generar números aleatorios rand()
#include <string.h>
#include <math.h>
#include "imc/PerceptronMulticapa.h"

using namespace std;
using namespace imc;

int main(int argc, char **argv) {

	// Procesar la línea de comandos
	bool tflag = 0, Tflag=0, bflag=0;
	char *tvalue = NULL, *Tvalue = NULL;
	int iteraciones=1000, hvalue=5, numCapas=1;
	float eta=0.1, mu=0.9;

	int opcion;
	while ((opcion = getopt (argc, argv, "t:T:i:l:h:e:m:b")) != -1){
		switch (opcion){

		// Leer fichero de train y de test desde la línea de comandos
		case 't':
			tvalue = optarg;
			tflag = 1;
			break;

		case 'T':
			Tvalue = optarg;
			Tflag = 1;
			break;
		// Leer iteraciones, capas y neuronas desde la línea de comandos
		case 'i':
			iteraciones = atoi(optarg);
			break;

		case 'l':
			numCapas = atoi(optarg);
			break;

		case 'h':
			hvalue = atoi(optarg);
			break;

		// Leer sesgo, eta y mu de la línea desde comandos
		case 'e':
			eta = atof(optarg);
			break;

		case 'm':
			mu = atof(optarg);
			break;

		case 'b':
			bflag = 1;
			break;


		if (isprint (optopt))
			cerr << "Error: Opción desconocida \'" << optopt
			<< "\'" << std::endl;
		else
			cerr << "Error: Caracter de opcion desconocido \'x" << std::hex << optopt
			<< "\'" << std::endl;
		exit (EXIT_FAILURE);

		// en cualquier otro caso lo consideramos error grave y salimos
		default:
			cerr << "Error: línea de comandos errónea." << std::endl;
		exit(EXIT_FAILURE);
		}  // case

	}// while

	if(tflag==0){
		cout << "Error. Falta el archivo de entrada de datos." << endl;
		exit(-1);
	}
	if(Tflag==0){
		Tvalue=tvalue;
	}
	
	PerceptronMulticapa mlp;
	
	Datos * pDatosTrain = mlp.leerDatos(tvalue);

	Datos * pDatosTest = mlp.leerDatos(Tvalue);

	// Inicializar el vector "topología"
	int *topologia = new int [numCapas+2];
	topologia[0]=pDatosTrain->nNumEntradas;
	for (int i=1; i<numCapas+1; i++){
		topologia[i]=hvalue;
	}
	topologia[numCapas+1]=pDatosTrain->nNumSalidas;
	// (número de neuronas por cada capa, incluyendo la de entrada
	//  y la de salida)
	// ...

	// Sesgo
	mlp.bSesgo = bflag;

	// Eta
	mlp.dEta = eta;

	// Mu
	mlp.dMu = mu;

	// Inicialización propiamente dicha
	mlp.inicializar(numCapas+2,topologia);

	// Semilla de los números aleatorios
	int semillas[] = {10,20,30,40,50};
	double *erroresTest = new double[5];
	double *erroresTrain = new double[5];
	for(int i=0; i<5; i++){
		srand(semillas[i]);
		cout << "**********" << endl;
		cout << "SEMILLA " << semillas[i] << endl;
		cout << "**********" << endl;
		mlp.ejecutarAlgoritmoOnline(pDatosTrain,pDatosTest,iteraciones,&(erroresTrain[i]),&(erroresTest[i]));
		cout << "Finalizamos => Error de test final: " << erroresTest[i] << endl;
	}

	cout << "TERMINADAS LAS CINCO SEMILLAS" << endl;

	// Calcular media y desviación típica de los errores de Train y de Test
	// ....

	double mediaErrorTrain=0.0, desviacionTipicaErrorTrain=0.0, mediaErrorTest=0.0, desviacionTipicaErrorTest=0.0;
	for (int i=0; i<5; i++){
		mediaErrorTrain+=erroresTrain[i];
		mediaErrorTest+=erroresTest[i];
	}
	mediaErrorTrain/=5;
	mediaErrorTest/=5;

	for (int i=0; i<5; i++){
		desviacionTipicaErrorTrain+=pow(erroresTrain[i]-mediaErrorTrain, 2);
		desviacionTipicaErrorTest+=pow(erroresTest[i]-mediaErrorTest, 2);
	}

	desviacionTipicaErrorTrain/=4;
	desviacionTipicaErrorTest/=4;

	desviacionTipicaErrorTrain=sqrt(desviacionTipicaErrorTrain);
	desviacionTipicaErrorTest=sqrt(desviacionTipicaErrorTest);

	cout << "RESUMEN FINAL" << endl;
	cout << "*************" << endl;
	cout << "Error de entrenamiento (Media +- DT): " << mediaErrorTrain << " +- " << desviacionTipicaErrorTrain << endl;
	cout << "Error de test (Media +- DT):          " << mediaErrorTest << " +- " << desviacionTipicaErrorTest << endl;
	return EXIT_SUCCESS;
}

