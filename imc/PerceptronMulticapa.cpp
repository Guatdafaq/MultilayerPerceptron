/*********************************************************************
 * File  : PerceptronMulticapa.cpp
 * Date  : 2015
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // Para establecer la semilla srand() y generar números aleatorios rand()
#include <limits>
#include <math.h>

#include "PerceptronMulticapa.h"

using namespace imc;
using namespace std;

// ------------------------------
// Obtener un número real aleatorio en el intervalo [Low,High]
double realAleatorio(double Low, double High)
{
	double rango=High-Low;
	double resultado=drand48()*rango;
	return resultado+Low;
}

// ------------------------------
// CONSTRUCTOR: Dar valor por defecto a todos los parámetros
PerceptronMulticapa::PerceptronMulticapa()
{

}

// Reservar memoria para las estructuras de datos
// nl tiene el numero de capas y npl es un vector que contiene el número de neuronas por cada una de las capas
// Rellenar vector Capa* pCapas
int PerceptronMulticapa::inicializar(int nl, int npl[]) {
	nNumCapas=nl;
	pCapas = new Capa [nl];
	for (int i=0; i<nl; i++){
		pCapas[i].nNumNeuronas = npl[i];
		pCapas[i].pNeuronas = new Neurona [npl[i]];
	}
	for (int i=1; i<nl; i++){
		for (int j=0; j<pCapas[i].nNumNeuronas; j++){
			pCapas[i].pNeuronas[j].w = new double [pCapas[i-1].nNumNeuronas+1];
			pCapas[i].pNeuronas[j].deltaW = new double [pCapas[i-1].nNumNeuronas+1];
			pCapas[i].pNeuronas[j].ultimoDeltaW = new double [pCapas[i-1].nNumNeuronas+1];
			pCapas[i].pNeuronas[j].wCopia = new double [pCapas[i-1].nNumNeuronas+1];
		}
	}


	for (int i=1; i<nNumCapas; i++){
		for (int j=0; j<pCapas[i].nNumNeuronas; j++){
			for (int k=0; k<pCapas[i-1].nNumNeuronas+1; k++){
				pCapas[i].pNeuronas[j].w[k]=0.0;
				pCapas[i].pNeuronas[j].deltaW[k]=0.0;
				pCapas[i].pNeuronas[j].ultimoDeltaW[k]=0.0;
			}
			pCapas[i].pNeuronas[j].x=0.0;
			pCapas[i].pNeuronas[j].dX=0.0;
			
		}
	}
	return 1;
}


// ------------------------------
// DESTRUCTOR: liberar memoria
PerceptronMulticapa::~PerceptronMulticapa() {
	//liberarMemoria();
}


// ------------------------------
// Liberar memoria para las estructuras de datos
void PerceptronMulticapa::liberarMemoria() {
	for (int i=1; i<nNumCapas-1; i++){
		for (int j=0; j<pCapas[i].nNumNeuronas; j++){
				delete []pCapas[i].pNeuronas[j].w;
				delete []pCapas[i].pNeuronas[j].deltaW;
				delete []pCapas[i].pNeuronas[j].ultimoDeltaW;
				delete []pCapas[i].pNeuronas[j].wCopia;
		}
	}
	for (int i=0; i<pCapas->nNumNeuronas; i++){
		delete []pCapas[i].pNeuronas;
	}

	delete[]pCapas;
}

// ------------------------------
// Rellenar todos los pesos (w) aleatoriamente entre -1 y 1
void PerceptronMulticapa::pesosAleatorios() {
	if (bSesgo){
		for (int i=1; i<nNumCapas; i++){
			for (int j=0; j<pCapas[i].nNumNeuronas; j++){
				for (int k=0; k<pCapas[i-1].nNumNeuronas+1; k++){
					pCapas[i].pNeuronas[j].w[k]=realAleatorio(-1.0, 1.0);
				}
			}
		}
	}else{
		for (int i=1; i<nNumCapas; i++){
			for (int j=0; j<pCapas[i].nNumNeuronas; j++){
				for (int k=0; k<pCapas[i-1].nNumNeuronas; k++){
					pCapas[i].pNeuronas[j].w[k]=realAleatorio(-1.0, 1.0);
				}
			}
		}

	}
}

// ------------------------------
// Alimentar las neuronas de entrada de la red con un patrón pasado como argumento
void PerceptronMulticapa::alimentarEntradas(double* input) {

	for (int i=0; i<pCapas[0].nNumNeuronas; i++){
		pCapas[0].pNeuronas[i].x=input[i];
	}

}

// ------------------------------
// Recoger los valores predichos por la red (out de la capa de salida) y almacenarlos en el vector pasado como argumento
void PerceptronMulticapa::recogerSalidas(double* output) {
	for (int i=0; i<pCapas[nNumCapas-1].nNumNeuronas; i++){
		output[i]=pCapas[nNumCapas-1].pNeuronas[i].x;
	}
}

// ------------------------------
// Hacer una copia de todos los pesos (copiar w en copiaW)
void PerceptronMulticapa::copiarPesos() {

	for(int i=1; i<nNumCapas; i++){
		for(int j=0; j<pCapas[i].nNumNeuronas; j++){
			pCapas[i].pNeuronas[j].wCopia = pCapas[i].pNeuronas[j].w;
		}
	}

}

// ------------------------------
// Restaurar una copia de todos los pesos (copiar copiaW en w)
void PerceptronMulticapa::restaurarPesos() {

	for(int i=1; i<nNumCapas; i++){
		for(int j=0; j<pCapas[i].nNumNeuronas; j++){
			pCapas[i].pNeuronas[j].w = pCapas[i].pNeuronas[j].wCopia;
		}
	}

}

// ------------------------------
// Calcular y propagar las salidas de las neuronas, desde la primera capa hasta la última
void PerceptronMulticapa::propagarEntradas() {

	for(int i=1; i<nNumCapas; i++){
		for(int j=0; j<pCapas[i].nNumNeuronas; j++){
			for(int k=0; k<pCapas[i-1].nNumNeuronas; k++){
				pCapas[i].pNeuronas[j].x+=pCapas[i-1].pNeuronas[k].x*pCapas[i].pNeuronas[j].w[k];
			}
			pCapas[i].pNeuronas[j].x+=pCapas[i].pNeuronas[j].w[pCapas[i-1].nNumNeuronas];
			pCapas[i].pNeuronas[j].x*=-1;
			pCapas[i].pNeuronas[j].x=1/(1+exp(pCapas[i].pNeuronas[j].x));

		}
	}

}

// ------------------------------
// Calcular el error de salida (MSE) del out de la capa de salida con respecto a un vector objetivo y devolverlo
double PerceptronMulticapa::calcularErrorSalida(double* target) {
	double error=0.0;
	
	for (int i=0; i<pCapas[nNumCapas-1].nNumNeuronas; i++){
		error+=pow(target[i]-pCapas[nNumCapas-1].pNeuronas[i].x, 2);
	}

	return error/pCapas[nNumCapas-1].nNumNeuronas;
}


// ------------------------------
// Retropropagar el error de salida con respecto a un vector pasado como argumento, desde la última capa hasta la primera
void PerceptronMulticapa::retropropagarError(double* objetivo) {

	for (int i=0; i<pCapas[nNumCapas-1].nNumNeuronas; i++){
		pCapas[nNumCapas-1].pNeuronas[i].dX=-(objetivo[i]-pCapas[nNumCapas-1].pNeuronas[i].x)*pCapas[nNumCapas-1].pNeuronas[i].x*(1-pCapas[nNumCapas-1].pNeuronas[i].x);
	}
	for(int i=nNumCapas-2;i>0;i--){
		for(int j=0; j<pCapas[i].nNumNeuronas; j++){
			//Pesos
			for(int k=0; k<pCapas[i+1].nNumNeuronas;k++){
				pCapas[i].pNeuronas[j].dX+=pCapas[i+1].pNeuronas[k].w[j]*pCapas[i+1].pNeuronas[k].dX;
			}
			//Sesgo
			for(int k=0; k<pCapas[i+1].nNumNeuronas;k++){
				pCapas[i].pNeuronas[j].dX+=pCapas[i+1].pNeuronas[k].w[pCapas[i].nNumNeuronas]*pCapas[i+1].pNeuronas[k].dX;
			}
			pCapas[i].pNeuronas[j].dX*=pCapas[i].pNeuronas[j].x*(1-pCapas[i].pNeuronas[j].x);
		}
	}

}

// ------------------------------
// Acumular los cambios producidos por un patrón en deltaW
void PerceptronMulticapa::acumularCambio() {

	for(int i=1; i<nNumCapas; i++){
		for (int j=0; j<pCapas[i].nNumNeuronas; j++){
			for(int k=0; k<pCapas[i-1].nNumNeuronas; k++){
				pCapas[i].pNeuronas[j].deltaW[k]+=pCapas[i].pNeuronas[j].dX*pCapas[i-1].pNeuronas[k].x;
			}
			pCapas[i].pNeuronas[j].deltaW[pCapas[i-1].nNumNeuronas]+=pCapas[i].pNeuronas[j].dX*bSesgo;
		}
	}

}

// ------------------------------
// Actualizar los pesos de la red, desde la primera capa hasta la última
void PerceptronMulticapa::ajustarPesos() {

	for (int i=1; i<nNumCapas; i++){
		for (int j=0; j<pCapas[i].nNumNeuronas; j++){
			for (int k=0; k<pCapas[i-1].nNumNeuronas; k++){
				pCapas[i].pNeuronas[j].w[k]+=-dEta*pCapas[i].pNeuronas[j].deltaW[k]-dMu*(dEta*pCapas[i].pNeuronas[j].ultimoDeltaW[k]);
			}
			pCapas[i].pNeuronas[j].w[pCapas[i-1].nNumNeuronas]+=-dEta*pCapas[i].pNeuronas[j].deltaW[pCapas[i-1].nNumNeuronas]-dMu*(dEta*pCapas[i].pNeuronas[j].ultimoDeltaW[pCapas[i-1].nNumNeuronas]);
		}
	}
}

// ------------------------------
// Imprimir la red, es decir, todas las matrices de pesos
void PerceptronMulticapa::imprimirRed() {
	//Hacer capa de entrada y de salida, diferenciarlas
	cout << "Capa de entrada -----------" << endl;
	cout << "Hay " << pCapas[0].nNumNeuronas << " variables de entrada" << endl;
	for(int i=1; i<nNumCapas; i++){
		cout << "Capa " << i << endl << "-----------" << endl;
		for (int j=0; j<pCapas[i].nNumNeuronas; j++){
			for (int k=0; k<pCapas[i-1].nNumNeuronas+1; k++){
				cout << pCapas[i].pNeuronas[j].w[k] << " ";
			}
			cout << endl;
		}
	}

}

// ------------------------------
// Simular la red: propagar las entradas hacia delante, computar el error, retropropagar el error y ajustar los pesos
// entrada es el vector de entradas del patrón y objetivo es el vector de salidas deseadas del patrón
void PerceptronMulticapa::simularRedOnline(double* entrada, double* objetivo) {

	for(int i=1; i<nNumCapas; i++){
		for(int j=0; j<pCapas[i].nNumNeuronas; j++){
			for (int k=0; k<pCapas[i-1].nNumNeuronas+1; k++){
				pCapas[i].pNeuronas[j].ultimoDeltaW[k]=pCapas[i].pNeuronas[j].deltaW[k];
				pCapas[i].pNeuronas[j].deltaW[k]=0.0;
			}
		}
	}
	alimentarEntradas(entrada);
	propagarEntradas();
	retropropagarError(objetivo);
	acumularCambio();
	ajustarPesos();
}

// ------------------------------
// Leer una matriz de datos a partir de un nombre de fichero y devolverla
Datos* PerceptronMulticapa::leerDatos(const char *archivo) {

	Datos* dat = new Datos;
	ifstream f;
	f.open(archivo);
	int n, k, N;

	char texto[128];
	
	f >> n;
	f >> k;
	f >> N;

	dat->nNumEntradas=n;
	dat->nNumSalidas=k;
	dat->nNumPatrones=N;
	
	dat->entradas  = new double *[dat->nNumPatrones];
	dat->salidas  = new double *[dat->nNumPatrones];


	for(int i=0; i<dat->nNumPatrones; i++){
		dat->entradas[i]= new double [dat->nNumEntradas];
		dat->salidas[i]= new double [dat->nNumSalidas];
	}
	
	for(int i=0; i<dat->nNumPatrones;i++){
		for (int e=0; e<dat->nNumEntradas; e++){
			f>>dat->entradas[i][e];
		}
		for (int s=0; s<dat->nNumSalidas; s++){
			f>>dat->salidas[i][s];	
		}
		f.getline(texto, 288);
	}

	return dat;

}

// ------------------------------
// Entrenar la red on-line para un determinado fichero de datos
void PerceptronMulticapa::entrenarOnline(Datos* pDatosTrain) {
	int i;
	for(i=0; i<pDatosTrain->nNumPatrones; i++)
		simularRedOnline(pDatosTrain->entradas[i], pDatosTrain->salidas[i]);
}

// ------------------------------
// Probar la red con un conjunto de datos y devolver el error MSE cometido
double PerceptronMulticapa::test(Datos* pDatosTest) {
	int i;
	double dAvgTestError = 0;
	for(i=0; i<pDatosTest->nNumPatrones; i++){
		// Cargamos las entradas y propagamos el valor
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas();
		dAvgTestError += calcularErrorSalida(pDatosTest->salidas[i]);
	}
	dAvgTestError /= pDatosTest->nNumPatrones;
	return dAvgTestError;
}

// ------------------------------
// Ejecutar el algoritmo de entrenamiento durante un número de iteraciones, utilizando pDatosTrain
// Una vez terminado, probar como funciona la red en pDatosTest
// Tanto el error MSE de entrenamiento como el error MSE de test debe calcularse y almacenarse en errorTrain y errorTest
void PerceptronMulticapa::ejecutarAlgoritmoOnline(Datos * pDatosTrain, Datos * pDatosTest, int maxiter, double *errorTrain, double *errorTest)
{
	int countTrain = 0;

	// Inicialización de pesos
	pesosAleatorios();

	double minTrainError = 0;
	int numSinMejorar;
	double testError = 0;
	
	// Aprendizaje del algoritmo
	do {

		entrenarOnline(pDatosTrain);
		double trainError = test(pDatosTrain);
		//Convergencia
		//testError = test(pDatosTest);
		// El 0.00001 es un valor de tolerancia, podría parametrizarse
		if(countTrain==0 || fabs(trainError - minTrainError) > 0.00001){
			minTrainError = trainError;
			copiarPesos();
			numSinMejorar = 0;
		}
		else{
			numSinMejorar++;
		}

		if(numSinMejorar==50){
			restaurarPesos();
			countTrain = maxiter;
		}
		
		
		countTrain++;

		cout << "Iteración " << countTrain << "\t Error de entrenamiento: " << trainError  << endl;
		//Convergencia
		//cout << "Iteración " << countTrain << "\t Error de entrenamiento: " << trainError << "\tError de test: " << testError  << endl;
		//getchar();
	} while ( countTrain<maxiter );

	
	cout << "PESOS DE LA RED" << endl;
	cout << "===============" << endl;
	
	imprimirRed();
	
	cout << "Salida Esperada Vs Salida Obtenida (test)" << endl;
	cout << "=========================================" << endl;
	for(int i=0; i<pDatosTest->nNumPatrones; i++){
		double* prediccion = new double[pDatosTest->nNumSalidas];

		// Cargamos las entradas y propagamos el valor
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas();
		recogerSalidas(prediccion);
		for(int j=0; j<pDatosTest->nNumSalidas; j++)
			cout << pDatosTest->salidas[i][j] << " -- " << prediccion[j];
		cout << endl;
		delete[] prediccion;

	}

	testError = test(pDatosTest);
	*errorTest=testError;
	*errorTrain=minTrainError;
	
}
