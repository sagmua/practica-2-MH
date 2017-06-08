 
#ifndef __ALGORITMOS__
#define __ALGORITMOS__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <chrono>
#include <random>
#include <deque>
#include <set>



using namespace std;

class Algoritmos{

 public:
 	vector<vector<double> > datos;	//matriz que contendrá los datos
 	vector<string> etiquetas; //vector con las etiquetas correspondientes a cada fila de la matriz
 	vector<  pair< vector<int>, vector<int> >     > particiones; //Vector de vectores que contiene 5 tablas de índices.
 public:

 	Algoritmos(string s);

 	//Funciones para leer fichero csv y para realizar las particiones:
 	void leerFichero(string nombre);
 	void realizaParticiones();

 	//Metodos para imprimir vectores:
 	void imprime(vector<double> v);
 	void imprime(vector<int> v);

 	//ALGORITMOS:
 	pair<double, double> knn(const vector<int> & train, const vector<int> & test, const vector<double> &pesos, bool bl, double & tasa_red, double & tasa_clas); //devuelve el porcentaje de acierto y el tiempo empleado.
 	pair<vector<double> , double> relief(const vector<int> & train, const vector<int> & test);
 	pair<vector<double> , double> BL(const vector<int> & indices_datos, double & tasa_mejor_solucion,int max_evaluaciones=15000, vector<double> sol_inicial=vector<double>());

 	//Algoritmos PRACTICA 2:
 	//-----------:
 	pair<vector<double> , double> SA(const vector<int> & indices_datos);
 	pair<vector<double> , double> ILS(const vector<int> & indices_datos);
 	pair<vector<double> , double> DE_RAND(const vector<int> & indices_datos);
 	pair<vector<double> , double> DE_CURRENT_TO_BEST(const vector<int> & indices_datos);
 	//Calcular la temperatura en determinada iteración:
 	double generarTemperatura(double temp_actual, double temp_inicial, double temp_final, int max_enfriamientos);
 	vector<double> mutacionILS(const vector<double> & pesos,vector<int> & indices_mutar);

 	//-------------

 	//genéticos:
	void operadorCruceBLX(const vector<double> & padre1, const vector<double> & padre2, vector<double> & h1, vector<double> & h2 );
	void operadorCruceAritmetico(const vector<double> & padre1, const vector<double> & padre2, vector<double> & h1);
	pair<vector<double>, double> AGG_BLX(const vector<int> & indices_datos);
	pair<vector<double>, double> AGG_CA(const vector<int> & indices_datos);
	pair<vector<double>, double> AGE_BLX(const vector<int> & indices_datos);
	pair<vector<double>, double> AGE_CA(const vector<int> & indices_datos);

	pair<vector<double>, double> AM_10_10(const vector<int> & indices_datos);
	pair<vector<double>, double> AM_10_01(const vector<int> & indices_datos);
	pair<vector<double>, double> AM_10_01_mej(const vector<int> & indices_datos);

	vector<int> torneoBinario(const vector<double> & tasas, int num_padres, int tam_poblacion);

 	double calcularDistancia(const int a, const int b, const vector<double> & v);



 	void normalizaPesos(vector<double> & pesos);
 	vector<double> generarVecino(const vector<double> & pesos, int indice_mutar);



 };



 #endif