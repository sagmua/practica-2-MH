


#include "algoritmos.h"



default_random_engine generator (5);
normal_distribution<double> distribution (0.0,0.3);
std::uniform_real_distribution<double> uniforme(0.0,1.0);
double basura= 0;


//Matodos para imprimir vectores por pantalla (simplemente para hacer debug)
void Algoritmos::imprime(vector<int> v){
	cout << "vector: ";
	for(auto it = v.begin(); it != v.end(); it++){
		cout << *it << "  ";
	}
	cout << endl << endl;
}

void Algoritmos::imprime(vector<double> v){
	cout << "vector: ";
	for(auto it = v.begin(); it != v.end(); it++){
		cout << *it << "  ";
	}
	cout << endl << endl;
}






//sobrecarga del operados multiplicacion para obtener la multiplicacion elemento a elemento
// entre dos vectores:


Algoritmos::Algoritmos(string nombre){
	leerFichero(nombre);
	realizaParticiones();
}


void Algoritmos::realizaParticiones(){
	//Creamos un vector que contendrá 5 vectores de índices, que 
	// corresponderán a cada una de las 5 particiones:

	srand(5);//fijamos una semilla.

	//Obtenemos todos los indices de los datos:
	vector<int> indices;
	for(int i = 0; i < datos.size(); i++){
		indices.push_back(i);
	}

	//Mezclamos los índices:
	random_shuffle(indices.begin(), indices.end());

	//tamaño de cada uno de los 5 conjuntos test conjunto test:
	int tam_test = indices.size()/5;

	//repetimos 5 veces para tener 5 particiones
	int resto = indices.size()%5;
	auto it = indices.begin();
	for(int i = 0; i < 5; i++, it+=tam_test){

		
		
		pair<vector<int>, vector<int> >p;
		//separamos el conjunto en test y train:
		if(i == 4){
			vector<int> test (it+resto, indices.end());
			vector<int> train (indices.begin(), it+resto);
			p.first = train;
			p.second = test;
		}
		else if (i == 0){
			vector<int> test (it, it+tam_test);
			vector<int> train (it+tam_test, indices.end());
			p.first = train;
			p.second = test;
		}
		else{
			vector<int> test (it, it+tam_test);
			vector<int> train (it+tam_test, indices.end());
			train.insert(train.end(), indices.begin(), it);
			p.first = train;
			p.second = test;
		}

		

		//lo añadimos al vector de particiones
		
		particiones.push_back(p);
	}




}



//Función para leer un fichero csv que contiene la información ya normalizada
// mediante el script en R.
void Algoritmos::leerFichero(string nombre){

	ifstream file ( nombre ); 
	string linea;

	string cabecera;
	getline ( file, cabecera, '\n' ); 

	int i = 0;

	vector<double> v;
	datos.push_back(v);

	
	while ( file.good() ){

	    getline ( file, linea, '\n' ); 
	    
	    if(linea != ""){
	    	
	    
		    //Quitamos el número de linea:
		   	istringstream la_cadena (linea);
		   	

		   	string atributo;
		   	getline ( la_cadena, atributo, ',' );
		   	


		   	while(la_cadena.good()){
		   		
		   		getline ( la_cadena, atributo, ',');
		   		
		   		if(atributo.find('"') != string::npos){	//Encontramos una etiqueta
		   			atributo.pop_back();
		   			atributo = atributo.substr(1);
		   			
		   			etiquetas.push_back(atributo);
		   		}
		   		else{
		   			
		   			double un_dato = atof(atributo.c_str());
		   			datos[i].push_back(un_dato);

		   		}
		   		
		   		

		   	}


		   i++;
		   vector<double> v;
		   datos.push_back(v);
		   
		}
	}

	datos.pop_back();
	
}






//Función para calcular la distancia entre dos carasterísticas teniendo en cuenta los pesos:
double Algoritmos::calcularDistancia(const int a, const int b, const vector<double> & pesos){

	double distancia=0;
	int tam = pesos.size();
	

	for(int i = 0; i < tam; i+=2){
		distancia+=(datos[a][i]-datos[b][i])*(datos[a][i]-datos[b][i])*pesos[i];
	}

	return (sqrt(distancia));

	
}





//Función que calcula el 1-NN:
pair<double, double> Algoritmos::knn(const vector<int> & train, const vector<int> & test, const vector<double> &pesos, bool bl, double & tasa_red, double & tasa_clas){
	//creamos un par donde almacenaremos el error y el tiempo:
	pair<double,double> error_tiempo;
	double numero_etiquetas_bien_clasificadas = 0;

	//recorremos el conjunto de datos

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución

	//Recorremos cada uno de las características y camos calculando la discancia con 
	// respecto al resto:
	for(vector<int>::const_iterator i = test.cbegin(); i != test.cend(); i++){
		

		double distancia_min=1000000000000;	//lo ponemos a un valor muy grande.
		int posicion_min;	//guardaremos la posición del más cercano a 'i'

		//Vamos calculando las distancias con el resto, quedándonos ssiempre con la
		//que menos distancia tenga:
		for(vector<int>::const_iterator j = train.cbegin(); j != train.cend(); j++){

			if(((*j)!=(*i)) && bl){ //leave one out
				double d = calcularDistancia(*i, *j, pesos);
				

				if(d <= distancia_min){
					distancia_min = d;
					posicion_min = *j;
				}

			}
			else if(!bl){
				double d = calcularDistancia(*i, *j, pesos);
				

				if(d <= distancia_min){
					distancia_min = d;
					posicion_min = *j;
				}

			}
		}

		//Comprobamos si la etiqueta obtenida es correcta:
		if(etiquetas[*i] == etiquetas[posicion_min]){
			numero_etiquetas_bien_clasificadas++;
		}
		
	}

	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	//Calculamos la tasa de clasificación:
	
	tasa_clas = 100.0*(numero_etiquetas_bien_clasificadas/test.size());
	

	//Obtenemos el número de pesos que son menores que 0:
	int menores_cero = 0;
	for(auto it = pesos.begin(); it != pesos.end(); it++){
		if(*it < 0.1){
			menores_cero++;
		}
	}

	tasa_red = 100.0*(menores_cero)/(pesos.size());
	//cout << tasa_red;
	//cout << "tasa clasificación: "<< tasa_clas << endl;
	//cout << "menores_cero= "<< menores_cero<< endl;

	double alfa = 0.5;
	double funcion_objetivo = tasa_clas*alfa + tasa_red*(1-alfa); 
	//cout << "funcion_objetivo: "<<funcion_objetivo << endl;

	error_tiempo.first = funcion_objetivo;
	error_tiempo.second = microseconds/1000000;	//pasamos a segundos



	return error_tiempo;	//devolvemos la tasa junto con el tiempo empleado en un pair.



}



//Algoritmo RELIEF que devuelve los pesos y el tiempo empleado:

//Función que calcula el 1-NN:
pair<vector<double> , double> Algoritmos::relief(const vector<int> & train, const vector<int> & test){
	//creamos un par donde almacenaremos el error y el tiempo:
	pair<vector<double> ,double> pesos_tiempo;
	double numero_etiquetas_bien_clasificadas;

	//Creamos un vector de pesos, originalmente a 0
	vector<double> pesos (datos[0].size(), 0);

	//recorremos el conjunto de datos
	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución

	//Recorremos cada uno de las características y camos calculando la discancia con 
	// respecto al resto:
	for(auto i = test.cbegin(); i != test.cend(); i++){
		

		double distancia_min_enemigo=1000000000000;	//lo ponemos a un valor muy grande.
		double distancia_min_amigo=1000000000000;	//lo ponemos a un valor muy grande.

		int posicion_min_amigo;	//guardaremos la posición del amigo más cercano a 'i'
		int posicion_min_enemigo;	//guardaremos la posición del amigo más cercano a 'i'

		//Vamos calculando las distancias con el resto, quedándonos ssiempre con la
		//que menos distancia tenga:
		for(auto j = train.cbegin(); j != train.cend(); j++){

			if((j)!=(i)){ //leave one out
				//Los pesos para calcular la distancia valen 1:
				double d = calcularDistancia(*i, *j, vector<double> (datos[0].size(), 1));
				
				//Si se trata de un amigo:
				if(etiquetas[*i] == etiquetas[*j]){
					if(d <= distancia_min_amigo){
						distancia_min_amigo = d;
						posicion_min_amigo = *j;
					}

				}
				else{	//Si se trata de un enemigo
					if(d <= distancia_min_enemigo){
						distancia_min_enemigo = d;
						posicion_min_enemigo = *j;
					}
				}

			}
		}

		//Actualizamos los pesos:
		for(int j = 0; j < datos[0].size(); j++){
			
			double resta_enemigo = (datos[*i][j] - datos[posicion_min_enemigo][j]);
			double resta_amigo = (datos[*i][j] - datos[posicion_min_amigo][j]);
			//valor absoluto
			resta_amigo = (resta_amigo < 0)? -resta_amigo : resta_amigo;
			resta_enemigo = (resta_enemigo < 0)? -resta_enemigo : resta_enemigo;

			pesos[j] += resta_enemigo - resta_amigo;
		}



		
		
		
	}

	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();


	normalizaPesos(pesos);
	pesos_tiempo.first = pesos;
	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos



	return pesos_tiempo;	//devolvemos la tasa junto con el tiempo empleado en un pair.



}


//Función que implementa la búsquedad local, iniciando los pesos mediante un criterio greedy (relief)
pair<vector<double> , double> Algoritmos::BL(const vector<int> & indices_datos, double & tasa_mejor_solucion, vector<double> solucion_inicial){
	deque<int> secuencia_atributos;
	for(int i = 0; i < datos[0].size(); i++)
		secuencia_atributos.push_back(i);

	pair<vector<double> ,double> pesos_tiempo; //par pesos-tiempo_ejecucion

	//generamos una solución aleatoria inicial en caso que no se indique una.
	// Si se nos indica, utilizamos esta

	
	

	if(solucion_inicial.empty()){

		solucion_inicial.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			solucion_inicial.push_back(f);
		}
	}
	

	//Ejecutamos el algoritmo hasta que no mejoremos la solucion actual o hasta haber realizado
	// 15000 evaluaciones de la solución.
	
	int num_evaluaciones = 0;

	//vamos guardando la mejor solución obtenida hasta ahora (inicialmente la aleatoria):
	vector<double> * mejores_pesos = &solucion_inicial;
	double mejor_tasa_obtenida  = (knn(indices_datos,indices_datos, solucion_inicial, true, basura, basura)).first;
	num_evaluaciones++;

	//cola de donde sacaremos aleatoriamente la componene que modificara la funcion generar vecino:
	deque<int> vecinos_por_generar = secuencia_atributos;
	random_shuffle(vecinos_por_generar.begin(), vecinos_por_generar.end());
	
	//Generamos el vecino correspondiente
	bool genera_nuevo_vecindario = true;

	//Contador de num_vecinos generados
	int num_vecinos_generados = 0;
	const int N = datos[0].size();

	vector<double> pesos_vecino;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución

	while((num_evaluaciones < 15000) && (num_vecinos_generados < 20*N) ){

		//Aqui vamos generando vecindario, de la mejor solucion obtenida:
		if(genera_nuevo_vecindario){
			vecinos_por_generar = secuencia_atributos;
			random_shuffle(vecinos_por_generar.begin(), vecinos_por_generar.end());

			genera_nuevo_vecindario = false;
			
		}
		else if(vecinos_por_generar.empty()){	//Si ya hemos explorado todo el vecindario sin mejorar.
			genera_nuevo_vecindario = true;
		}
		else{
			pesos_vecino = generarVecino(*(mejores_pesos), vecinos_por_generar.back());
			vecinos_por_generar.pop_back();//eliminamos esa componente del peso de la cola.
			num_vecinos_generados++;

			//Comprobamos si se obtiene una mejor tasa para el vecino generado:
			double tasa_vecino = (knn(indices_datos,indices_datos, pesos_vecino, true, basura, basura)).first;
			num_evaluaciones++;


			//si la tasa obtenida es mejor que la que llevamos hasta ahora
			// nos 'mudamos' generando un nuevo vecindario con la nueva solucion:
			if(tasa_vecino > mejor_tasa_obtenida){
				mejor_tasa_obtenida = tasa_vecino;
				
				mejores_pesos = &pesos_vecino;

				genera_nuevo_vecindario = true;
			}

		}


	}

	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	

	pesos_tiempo.first = *(mejores_pesos);
	tasa_mejor_solucion = mejor_tasa_obtenida;
	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos

	return(pesos_tiempo);

}


vector<double> Algoritmos::generarVecino(const vector<double> & pesos,int indice_mutar){
	vector<double> vecino = pesos;
	
  	double mutacion = distribution(generator);

  	//mutamos:
  	vecino[indice_mutar]+= mutacion;

  	if(vecino[indice_mutar] >1)
  		vecino[indice_mutar] = 1;
  	else if(vecino[indice_mutar] <0)
  		vecino[indice_mutar] = 0;



	return vecino;
}


//Función para normalizar los pesos:
void Algoritmos::normalizaPesos(vector<double> & pesos){

	//Calculamos el elemento máximo del vector:
	auto max = max_element(pesos.begin(),pesos.end());
	double maximo = *max;

	for(int i = 0; i < pesos.size(); i++){
		if(pesos[i] < 0){
			pesos[i] = 0;
		}
		else{
			pesos[i] = pesos[i]/maximo;
		}
	}
}



void Algoritmos::operadorCruceBLX(const vector<double> & padre1, const vector<double> & padre2, vector<double> & h1, vector<double> & h2 ){
	
	//Obtenemos el máximo elemento y el mínimo de cada uno de los padres:
	auto c_max1 = max_element(padre1.cbegin(), padre1.cend());
	auto c_max2 =max_element(padre2.cbegin(), padre2.cend());

	auto c_min1 = min_element(padre1.cbegin(), padre1.cend());
	auto c_min2 =min_element(padre2.cbegin(), padre2.cend());



	double alfa = 0.3;

	double c_min, c_max;

	if(*c_min2 < *c_min1)
		c_min = *c_min2;
	else
		c_min = *c_min1;

	if(*c_max2 > *c_max1)
		c_max = *c_max2;
	else
		c_max = *c_max1;

	//Obtenemos el intervalo donde generar números indice apartir del que dividiremos los padres:
	pair<double, double> intervalo (c_min-(c_max-c_min)*alfa,  c_max+(c_max-c_min)*alfa);
	


	//vamos generando a los hijos:
	for(int i = 0; i < padre2.size(); i++){
		double f = (double)rand() / RAND_MAX;
		double aleatorio =intervalo.first + f * (intervalo.second - intervalo.first);
		aleatorio = (aleatorio>1)? 1: aleatorio;
		aleatorio = (aleatorio<0)? 0: aleatorio;
		h1.push_back( aleatorio);

		f = (double)rand() / RAND_MAX;
		aleatorio =intervalo.first + f * (intervalo.second - intervalo.first);
		aleatorio = (aleatorio>1)? 1: aleatorio;
		aleatorio = (aleatorio<0)? 0: aleatorio;
		h2.push_back(aleatorio );
	}


	
}

void Algoritmos::operadorCruceAritmetico(const vector<double> & padre1, const vector<double> & padre2, vector<double> & h1){
	h1.reserve(padre1.size());
	//Sumamos la componente 'i' de los padres y la dividimos por 2 (media artimetica)
	for(int i = 0; i < padre1.size(); i++)
		h1.push_back((padre1[i]+padre2[i])/2.0);
	
}


//Operador de cruce

//Algortimo AGG_BLX 
pair<vector<double>, double> Algoritmos::AGG_BLX(const vector<int> & indices_datos){
	//contador para llevar cuantas evaluaciones llevamos:
	int num_evaluaciones = 0;

	pair<vector<double>, double> pesos_tiempo;
	//generamos una poblacion aleatoria inicial, tomando esta como mejor solucion encontrada
	//al comienzo:

	//generamos los individuos de forma aleatoria:
	vector<double> individuo;

	//matriz donde almacenamos la población:
	vector<vector<double> > poblacion_actual;
	for(int i = 0; i < 30; i++){
		individuo.clear();
		individuo.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			individuo.push_back(f);
		}
		poblacion_actual.push_back(individuo);
	}

	//Una vez aquí tenemos una población de 30 individuos generados aleatoriamente.
	// procedemos a la evaluación
	
	vector<double> tasas;
	tasas.reserve(30);
	for(int i = 0; i < 30; i++){
		tasas.push_back(knn(indices_datos, indices_datos, poblacion_actual[i], true, basura, basura).first);
		num_evaluaciones++;
	}

	bool hemos_perdido_mejor_padre = true;
	int mejor_padre_poblacion_actual;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución


	while(num_evaluaciones < 15000){
		//calculamos el mejor padre de la generacion actual:
		mejor_padre_poblacion_actual = 0;
		double max_tasa = tasas[0];
		for(int i = 1; i < tasas.size(); i++){
			if(tasas[i] > max_tasa){
				max_tasa = tasas[i];
				mejor_padre_poblacion_actual = i;
			}

		}

		//Una vez tenemos las tasas y sus pesos ordenados de menor a mayor, llamamos a la función torneo binario
		// que nos devolverá indices de 30 padres: 
		int num_padres = 30;
		vector<int> padres_cruzar = torneoBinario(tasas, num_padres, 30);

		//Calculamos la esperanza matemática para saber cuantos cruces vamos a realizar, teniendo en cuenta que la
		// probabilidad de cruce es de 0.7*M
		int num_cruces = (0.7*tasas.size()/2)+1;

		//Reproducimos el número de padres, creando una nueva poblacion (al ser generacional
		// la nueva generación sustituye a la actual:
		vector<vector<double> > nueva_poblacion;
		int j = 0;
		for(int i = 0; i < num_cruces; i++, j+=2){
			vector<double> h1, h2;
			operadorCruceBLX(poblacion_actual[padres_cruzar[j]], poblacion_actual[padres_cruzar[j+1]], h1, h2);
			//aniadimos los hijos a una nueva generación
			nueva_poblacion.push_back(h1);
			nueva_poblacion.push_back(h2);
		}

		

		//Aniadimos los padres que no se han cruzado:
		for(int i = num_cruces*2; i < poblacion_actual.size(); i++){
			nueva_poblacion.push_back(poblacion_actual[padres_cruzar[i]]);
			//tenemos encuenta si hemos aniadido al mejor padre de la poblacion actual
			// para aniadirlo en un futuro si no;
			if(i == mejor_padre_poblacion_actual){
				hemos_perdido_mejor_padre = false;
			}
		}

		

		//ahora calculamos el número de mutaciones que vamos a realizar (con una probabilidad de 0.001) redondeando:
		int num_mutaciones = 0.001 *  poblacion_actual.size() * poblacion_actual[0].size() +1;

		for(int i = 0; i < num_mutaciones; i++){
			int individuo_a_mutar = rand()%poblacion_actual.size();
			int gen_a_mutar = rand()%poblacion_actual[0].size();

			nueva_poblacion[individuo_a_mutar] = generarVecino(nueva_poblacion[individuo_a_mutar], gen_a_mutar);
		}


		//Ahora generamos las tasas de la nueva población:
		vector<double> tasas_nueva_poblacion;

		//Calculamos el peor de la nueva generación:
		int peor_nueva_generacion;
		double min_tasa = 101;
		tasas_nueva_poblacion.reserve(30);
		for(int i = 0; i < 30; i++){
			tasas_nueva_poblacion.push_back(knn(indices_datos, indices_datos, nueva_poblacion[i], true, basura, basura).first);

			//calculamos el peor de la generación:
			if(tasas_nueva_poblacion[i] < min_tasa){
				min_tasa = tasas_nueva_poblacion[i];
				peor_nueva_generacion = i;
			}

			num_evaluaciones++;
		}

		//Una vez que tenemos las tasas de la poblacion, si hemos perdido el mejor padre de la anterior
		// sustituimos la peor solución por este padre:
		if(hemos_perdido_mejor_padre){
			nueva_poblacion[peor_nueva_generacion]=poblacion_actual[mejor_padre_poblacion_actual];
			tasas_nueva_poblacion[peor_nueva_generacion]= tasas[mejor_padre_poblacion_actual];
		}

		//Cambiamos los valores para la siguiente generación:
		hemos_perdido_mejor_padre = true;
		tasas = tasas_nueva_poblacion;
		poblacion_actual = nueva_poblacion;

		
		
	}


	//devolvemos la mejor solución obtenida y el tiempo:
	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	double mejor_tasa = 0;
	int mejor;
	for(int i = 0; i < tasas.size(); i++){
		if(tasas[i] > mejor_tasa){
			mejor = i;
			mejor_tasa = tasas[i];
		}
	}

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = poblacion_actual[mejor];

	return(pesos_tiempo);

}

vector<int> Algoritmos::torneoBinario(const vector<double> & tasas, int num_padres, int tam_poblacion){

	//Vector de índices donde almacenaremos los mejores padres elegidos por torneo binario (30):
	vector<int> mejores_padres;
	mejores_padres.reserve(tam_poblacion);

	for(int i = 0; i < num_padres; i++){
		//elegimos dos padres aleatorios
		int padre1 = rand()%tam_poblacion;
		int padre2;

		//Si los padres son iguales por casualidad, volvemos a generar el segundo
		do{
			padre2 = rand()%tam_poblacion;
		}
		while(padre2 == padre1);

		//Nos quedamos con el que mejor tasa tiene;
		int mejor_padre = (tasas[padre1]<tasas[padre2])? padre1 : padre2;

		//lo aniadimos:
		mejores_padres.push_back(mejor_padre);
	}

	return(mejores_padres);


}



//Algortimo AGG_CA
pair<vector<double>, double> Algoritmos::AGG_CA(const vector<int> & indices_datos){
	//contador para llevar cuantas evaluaciones llevamos:
	int num_evaluaciones = 0;

	pair<vector<double>, double> pesos_tiempo;
	//generamos una poblacion aleatoria inicial, tomando esta como mejor solucion encontrada
	//al comienzo:

	//generamos los individuos de forma aleatoria:
	vector<double> individuo;

	//matriz donde almacenamos la población:
	vector<vector<double> > poblacion_actual;
	for(int i = 0; i < 30; i++){
		individuo.clear();
		individuo.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			individuo.push_back(f);
		}
		poblacion_actual.push_back(individuo);
	}

	//Una vez aquí tenemos una población de 30 individuos generados aleatoriamente.
	// procedemos a la evaluación
	
	vector<double> tasas;
	tasas.reserve(30);
	for(int i = 0; i < 30; i++){
		tasas.push_back(knn(indices_datos, indices_datos, poblacion_actual[i], true, basura, basura).first);
		num_evaluaciones++;
	}

	bool hemos_perdido_mejor_padre = true;
	int mejor_padre_poblacion_actual;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución


	while(num_evaluaciones < 15000){
		//calculamos el mejor padre de la generacion actual:
		mejor_padre_poblacion_actual = 0;
		double max_tasa = tasas[0];
		for(int i = 1; i < tasas.size(); i++){
			if(tasas[i] > max_tasa){
				max_tasa = tasas[i];
				mejor_padre_poblacion_actual = i;
			}

		}

		//Una vez tenemos las tasas y sus pesos ordenados de menor a mayor, llamamos a la función torneo binario
		// que nos devolverá indices de 30 padres: 
		int num_padres = 60;
		vector<int> padres_cruzar = torneoBinario(tasas, num_padres, 30);

		//Calculamos la esperanza matemática para saber cuantos cruces vamos a realizar, teniendo en cuenta que la
		// probabilidad de cruce es de 0.7*M
		int num_cruces = (0.7*padres_cruzar.size()/2);

		//Reproducimos el número de padres, creando una nueva poblacion (al ser generacional
		// la nueva generación sustituye a la actual:
		vector<vector<double> > nueva_poblacion;
		int j = 0;
		for(int i = 0; i < num_cruces; i++, j+=2){
			vector<double> h1;
			operadorCruceAritmetico(poblacion_actual[padres_cruzar[j]], poblacion_actual[padres_cruzar[j+1]], h1);
			//aniadimos los hijos a una nueva generación
			nueva_poblacion.push_back(h1);
			
		}

		

		

		//Aniadimos los padres que no se han cruzado:
		for(int i = nueva_poblacion.size(); i < poblacion_actual.size(); i++,j++){
			nueva_poblacion.push_back(poblacion_actual[padres_cruzar[j]]);
			//tenemos encuenta si hemos aniadido al mejor padre de la poblacion actual
			// para aniadirlo en un futuro si no;
			if(i == mejor_padre_poblacion_actual){
				hemos_perdido_mejor_padre = false;
			}
		}


		

		//ahora calculamos el número de mutaciones que vamos a realizar (con una probabilidad de 0.001) redondeando:
		int num_mutaciones = 0.001 *  poblacion_actual.size() * poblacion_actual[0].size() +1;

		for(int i = 0; i < num_mutaciones; i++){
			int individuo_a_mutar = rand()%poblacion_actual.size();
			int gen_a_mutar = rand()%poblacion_actual[0].size();

			nueva_poblacion[individuo_a_mutar] = generarVecino(nueva_poblacion[individuo_a_mutar], gen_a_mutar);
		}


		//Ahora generamos las tasas de la nueva población:
		vector<double> tasas_nueva_poblacion;

		//Calculamos el peor de la nueva generación:
		int peor_nueva_generacion;
		double min_tasa = 101;
		tasas_nueva_poblacion.reserve(30);
		for(int i = 0; i < 30; i++){
			tasas_nueva_poblacion.push_back(knn(indices_datos, indices_datos, nueva_poblacion[i], true, basura, basura).first);

			//calculamos el peor de la generación:
			if(tasas_nueva_poblacion[i] < min_tasa){
				min_tasa = tasas_nueva_poblacion[i];
				peor_nueva_generacion = i;
			}

			num_evaluaciones++;
		}

		//Una vez que tenemos las tasas de la poblacion, si hemos perdido el mejor padre de la anterior
		// sustituimos la peor solución por este padre:
		if(hemos_perdido_mejor_padre){
			nueva_poblacion[peor_nueva_generacion]=poblacion_actual[mejor_padre_poblacion_actual];
			tasas_nueva_poblacion[peor_nueva_generacion]= tasas[mejor_padre_poblacion_actual];
		}

		//Cambiamos los valores para la siguiente generación:
		hemos_perdido_mejor_padre = true;
		tasas = tasas_nueva_poblacion;
		poblacion_actual = nueva_poblacion;

		
		
	}


	//devolvemos la mejor solución obtenida y el tiempo:
	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	double mejor_tasa = 0;
	int mejor;
	for(int i = 0; i < tasas.size(); i++){
		if(tasas[i] > mejor_tasa){
			mejor = i;
			mejor_tasa = tasas[i];
		}
	}

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = poblacion_actual[mejor];

	return(pesos_tiempo);

}



//Algoritmo estacionario que implementa el operador de cruce BLX:

pair<vector<double>, double> Algoritmos::AGE_BLX(const vector<int> & indices_datos){
	//contador para llevar cuantas evaluaciones llevamos:
	int num_evaluaciones = 0;

	pair<vector<double>, double> pesos_tiempo;
	//generamos una poblacion aleatoria inicial, tomando esta como mejor solucion encontrada
	//al comienzo:

	//generamos los individuos de forma aleatoria:
	vector<double> individuo;

	//matriz donde almacenamos la población:
	vector<vector<double> > poblacion_actual;
	for(int i = 0; i < 30; i++){
		individuo.clear();
		individuo.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			individuo.push_back(f);
		}
		poblacion_actual.push_back(individuo);
	}

	//Una vez aquí tenemos una población de 30 individuos generados aleatoriamente.
	// procedemos a la evaluación
	
	vector<double> tasas;
	tasas.reserve(30);
	for(int i = 0; i < 30; i++){
		tasas.push_back(knn(indices_datos, indices_datos, poblacion_actual[i], true, basura, basura).first);
		num_evaluaciones++;
	}

	bool hemos_perdido_mejor_padre = true;
	int mejor_padre_poblacion_actual;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución


	while(num_evaluaciones < 15000){
		//calculamos el mejor padre de la generacion actual:
		mejor_padre_poblacion_actual = 0;
		double max_tasa = tasas[0];
		for(int i = 1; i < tasas.size(); i++){
			if(tasas[i] > max_tasa){
				max_tasa = tasas[i];
				mejor_padre_poblacion_actual = i;
			}

		}

		//Una vez tenemos las tasas y sus pesos ordenados de menor a mayor, llamamos a la función torneo binario
		// que nos devolverá indices de 2 padres (estacionario): 
		int num_padres = 2;
		vector<int> padres_cruzar = torneoBinario(tasas, num_padres, 30);
		

		//En el caso del estacionario la probabilidad de cruce es 1, por tanto generamos dos descentdientes:


		//Reproducimos el número de padres, creando una nueva poblacion (al ser generacional
		// la nueva generación sustituye a la actual:
		vector<vector<double> > nueva_poblacion;
		
		
		vector<double> h1, h2;
		operadorCruceBLX(poblacion_actual[padres_cruzar[0]], poblacion_actual[padres_cruzar[1]], h1, h2);
		
		
		
		//nueva poblacion = poblacion actual
		nueva_poblacion = poblacion_actual;

		
		//Copiamos la poblacion actial en la nueva, y las tasas tambien;
		vector<double> tasas_nueva_poblacion = tasas;


		



		//ahora calculamos el número de mutaciones que vamos a realizar (con una probabilidad de 0.001) redondeando:
		int num_mutaciones = 0.001 *  poblacion_actual.size() * poblacion_actual[0].size() +1;

		for(int i = 0; i < num_mutaciones; i++){
			int individuo_a_mutar = rand()%poblacion_actual.size();
			int gen_a_mutar = rand()%poblacion_actual[0].size();

			nueva_poblacion[individuo_a_mutar] = generarVecino(nueva_poblacion[individuo_a_mutar], gen_a_mutar);
			tasas_nueva_poblacion[individuo_a_mutar]= knn(indices_datos, indices_datos, h1, true, basura, basura).first;
			num_evaluaciones++;

		}

		//comparamos los hijos generados con los dos peores de la poblacion actual:
		int peor_padre = 0, segundo_peor_padre = 1;

		if(tasas[segundo_peor_padre] < tasas[peor_padre] ){
			int aux = peor_padre;
			peor_padre = segundo_peor_padre;
			segundo_peor_padre = aux;
		}

		for(int i = 2; i < poblacion_actual.size(); i++){
			if(tasas[i] < tasas[peor_padre]){
				segundo_peor_padre = peor_padre;
				peor_padre = i;
			}
			else if(tasas[i] < tasas[segundo_peor_padre])
				segundo_peor_padre = i;

		}

		//Una vez tenemos los dos peores padres de la generacion actual, comparamos con los dos descendientes del cruce;
		// si son mejores los sustituimos:
		double tasa_h1 = knn(indices_datos, indices_datos, h1, true, basura, basura).first;
		double tasa_h2 = knn(indices_datos, indices_datos, h2, true, basura, basura).first;

		num_evaluaciones+=2;

		if(tasa_h1 > tasas[peor_padre]){
			nueva_poblacion[peor_padre] = h1;
			tasas_nueva_poblacion[peor_padre] = tasa_h1;

			if(tasa_h2 > tasas[segundo_peor_padre]){
				nueva_poblacion[segundo_peor_padre] = h2;
				tasas_nueva_poblacion[segundo_peor_padre] = tasa_h2;

			}

		}
		else if(tasa_h2 > tasas[peor_padre]){
			nueva_poblacion[peor_padre] = h2;
			tasas_nueva_poblacion[peor_padre] = tasa_h2;

			if(tasa_h1 > tasas[segundo_peor_padre]){
				nueva_poblacion[segundo_peor_padre] = h1;
				tasas_nueva_poblacion[segundo_peor_padre] = tasa_h1;

			}

		}


		

		

		

		//Cambiamos los valores para la siguiente generación:
		tasas.clear();
		poblacion_actual.clear();
		tasas = tasas_nueva_poblacion;
		poblacion_actual = nueva_poblacion;

		
		
	}


	//devolvemos la mejor solución obtenida y el tiempo:
	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	double mejor_tasa = 0;
	int mejor;
	for(int i = 0; i < tasas.size(); i++){
		if(tasas[i] > mejor_tasa){
			mejor = i;
			mejor_tasa = tasas[i];
		}
	}

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = poblacion_actual[mejor];

	return(pesos_tiempo);

}




//Algoritmo estacionario que implementa el operador de cruce CA:

pair<vector<double>, double> Algoritmos::AGE_CA(const vector<int> & indices_datos){
	//contador para llevar cuantas evaluaciones llevamos:
	int num_evaluaciones = 0;

	pair<vector<double>, double> pesos_tiempo;
	//generamos una poblacion aleatoria inicial, tomando esta como mejor solucion encontrada
	//al comienzo:

	//generamos los individuos de forma aleatoria:
	vector<double> individuo;

	//matriz donde almacenamos la población:
	vector<vector<double> > poblacion_actual;
	for(int i = 0; i < 30; i++){
		individuo.clear();
		individuo.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			individuo.push_back(f);
		}
		poblacion_actual.push_back(individuo);
	}

	//Una vez aquí tenemos una población de 30 individuos generados aleatoriamente.
	// procedemos a la evaluación
	
	vector<double> tasas;
	tasas.reserve(30);
	for(int i = 0; i < 30; i++){
		tasas.push_back(knn(indices_datos, indices_datos, poblacion_actual[i], true, basura, basura).first);
		num_evaluaciones++;
	}

	bool hemos_perdido_mejor_padre = true;
	int mejor_padre_poblacion_actual;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución


	while(num_evaluaciones < 15000){
		//calculamos el mejor padre de la generacion actual:
		mejor_padre_poblacion_actual = 0;
		double max_tasa = tasas[0];
		for(int i = 1; i < tasas.size(); i++){
			if(tasas[i] > max_tasa){
				max_tasa = tasas[i];
				mejor_padre_poblacion_actual = i;
			}

		}

		//Una vez tenemos las tasas y sus pesos ordenados de menor a mayor, llamamos a la función torneo binario
		// que nos devolverá 4 indices (pues generamos 2 hijos) padres (estacionario): 
		int num_padres = 4;
		vector<int> padres_cruzar = torneoBinario(tasas, num_padres, 30);
		

		//En el caso del estacionario la probabilidad de cruce es 1, por tanto generamos dos descentdientes:


		//Reproducimos el número de padres, creando una nueva poblacion (al ser generacional
		// la nueva generación sustituye a la actual:
		vector<vector<double> > nueva_poblacion;
		
		
		vector<double> h1, h2;
		operadorCruceAritmetico(poblacion_actual[padres_cruzar[0]], poblacion_actual[padres_cruzar[1]], h1);
		operadorCruceAritmetico(poblacion_actual[padres_cruzar[2]], poblacion_actual[padres_cruzar[3]], h2);
		
		
		
		//nueva poblacion = poblacion actual
		nueva_poblacion = poblacion_actual;

		//Copiamos la poblacion actial en la nueva, y las tasas tambien;
		vector<double> tasas_nueva_poblacion = tasas;


		//ahora calculamos el número de mutaciones que vamos a realizar (con una probabilidad de 0.001) redondeando:
		int num_mutaciones = 0.001 *  poblacion_actual.size() * poblacion_actual[0].size() +1;

		for(int i = 0; i < num_mutaciones; i++){
			int individuo_a_mutar = rand()%poblacion_actual.size();
			int gen_a_mutar = rand()%poblacion_actual[0].size();

			nueva_poblacion[individuo_a_mutar] = generarVecino(nueva_poblacion[individuo_a_mutar], gen_a_mutar);
			tasas_nueva_poblacion[individuo_a_mutar]= knn(indices_datos, indices_datos, h1, true, basura, basura).first;
			num_evaluaciones++;

		}


		//comparamos los hijos generados con los dos peores de la poblacion actual:
		int peor_padre = 0, segundo_peor_padre = 1;

		if(tasas[segundo_peor_padre] < tasas[peor_padre] ){
			int aux = peor_padre;
			peor_padre = segundo_peor_padre;
			segundo_peor_padre = aux;
		}

		for(int i = 2; i < poblacion_actual.size(); i++){
			if(tasas[i] < tasas[peor_padre]){
				segundo_peor_padre = peor_padre;
				peor_padre = i;
			}
			else if(tasas[i] < tasas[segundo_peor_padre])
				segundo_peor_padre = i;

		}


		//Una vez tenemos los dos peores padres de la generacion actual, comparamos con los dos descendientes del cruce;
		// si son mejores los sustituimos:
		double tasa_h1 = knn(indices_datos, indices_datos, h1, true, basura, basura).first;
		double tasa_h2 = knn(indices_datos, indices_datos, h2, true, basura, basura).first;

		num_evaluaciones+=2;



		if(tasa_h1 > tasas[peor_padre]){
			nueva_poblacion[peor_padre] = h1;
			tasas_nueva_poblacion[peor_padre] = tasa_h1;

			if(tasa_h2 > tasas[segundo_peor_padre]){
				nueva_poblacion[segundo_peor_padre] = h2;
				tasas_nueva_poblacion[segundo_peor_padre] = tasa_h2;

			}

		}
		else if(tasa_h2 > tasas[peor_padre]){
			nueva_poblacion[peor_padre] = h2;
			tasas_nueva_poblacion[peor_padre] = tasa_h2;

			if(tasa_h1 > tasas[segundo_peor_padre]){
				nueva_poblacion[segundo_peor_padre] = h1;
				tasas_nueva_poblacion[segundo_peor_padre] = tasa_h1;

			}

		}



		//Cambiamos los valores para la siguiente generación:
		tasas.clear();
		poblacion_actual.clear();
		tasas = tasas_nueva_poblacion;
		poblacion_actual = nueva_poblacion;

		
		
	}


	//devolvemos la mejor solución obtenida y el tiempo:
	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	double mejor_tasa = 0;
	int mejor;
	for(int i = 0; i < tasas.size(); i++){
		if(tasas[i] > mejor_tasa){
			mejor = i;
			mejor_tasa = tasas[i];
		}
	}

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = poblacion_actual[mejor];

	return(pesos_tiempo);

}




//Algortimo memético,  AGG_CA, aplicando una búsqueda local a toda la población. 
pair<vector<double>, double> Algoritmos::AM_10_10(const vector<int> & indices_datos){


	//contador para llevar cuantas evaluaciones llevamos:
	int num_evaluaciones = 0;

	pair<vector<double>, double> pesos_tiempo;
	//generamos una poblacion aleatoria inicial, tomando esta como mejor solucion encontrada
	//al comienzo:

	//generamos los individuos de forma aleatoria:
	vector<double> individuo;

	//matriz donde almacenamos la población:
	vector<vector<double> > poblacion_actual;
	for(int i = 0; i < 10; i++){
		individuo.clear();
		individuo.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			individuo.push_back(f);
		}
		poblacion_actual.push_back(individuo);
	}

	//contador para saber la generacion que llevamos hasta ahora:
	int generacion = 1;

	//Una vez aquí tenemos una población de 10 individuos generados aleatoriamente.
	// procedemos a la evaluación
	
	vector<double> tasas;
	tasas.reserve(10);
	for(int i = 0; i < 10; i++){
		tasas.push_back(knn(indices_datos, indices_datos, poblacion_actual[i], true, basura, basura).first);
		num_evaluaciones++;
	}

	bool hemos_perdido_mejor_padre = true;
	int mejor_padre_poblacion_actual;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución


	while(num_evaluaciones < 15000){
		//calculamos el mejor padre de la generacion actual:
		mejor_padre_poblacion_actual = 0;
		double max_tasa = tasas[0];
		for(int i = 1; i < tasas.size(); i++){
			if(tasas[i] > max_tasa){
				max_tasa = tasas[i];
				mejor_padre_poblacion_actual = i;
			}

		}

		//Una vez tenemos las tasas y sus pesos ordenados de menor a mayor, llamamos a la función torneo binario
		// que nos devolverá indices de 10 padres: 
		int num_padres = 20;
		vector<int> padres_cruzar = torneoBinario(tasas, num_padres, 10);

		//Calculamos la esperanza matemática para saber cuantos cruces vamos a realizar, teniendo en cuenta que la
		// probabilidad de cruce es de 0.7*M
		int num_cruces = (0.7*padres_cruzar.size()/2);

		//Reproducimos el número de padres, creando una nueva poblacion (al ser generacional
		// la nueva generación sustituye a la actual:
		vector<vector<double> > nueva_poblacion;
		int j = 0;
		for(int i = 0; i < num_cruces; i++, j+=2){
			vector<double> h1;
			operadorCruceAritmetico(poblacion_actual[padres_cruzar[j]], poblacion_actual[padres_cruzar[j+1]], h1);
			//aniadimos los hijos a una nueva generación
			nueva_poblacion.push_back(h1);
			
		}

		

		

		//Aniadimos los padres que no se han cruzado:
		for(int i = nueva_poblacion.size(); i < poblacion_actual.size(); i++,j++){
			nueva_poblacion.push_back(poblacion_actual[padres_cruzar[j]]);
			//tenemos encuenta si hemos aniadido al mejor padre de la poblacion actual
			// para aniadirlo en un futuro si no;
			if(i == mejor_padre_poblacion_actual){
				hemos_perdido_mejor_padre = false;
			}
		}
		


		

		//ahora calculamos el número de mutaciones que vamos a realizar (con una probabilidad de 0.001) redondeando:
		int num_mutaciones = 0.001 *  poblacion_actual.size() * poblacion_actual[0].size() +1;

		for(int i = 0; i < num_mutaciones; i++){
			int individuo_a_mutar = rand()%poblacion_actual.size();
			int gen_a_mutar = rand()%poblacion_actual[0].size();

			nueva_poblacion[individuo_a_mutar] = generarVecino(nueva_poblacion[individuo_a_mutar], gen_a_mutar);
		}


		//Ahora generamos las tasas de la nueva población:
		vector<double> tasas_nueva_poblacion;

		//Calculamos el peor de la nueva generación:
		int peor_nueva_generacion;
		double min_tasa = 101;
		tasas_nueva_poblacion.reserve(10);
		for(int i = 0; i < 10; i++){
			tasas_nueva_poblacion.push_back(knn(indices_datos, indices_datos, nueva_poblacion[i], true, basura, basura).first);

			//calculamos el peor de la generación:
			if(tasas_nueva_poblacion[i] < min_tasa){
				min_tasa = tasas_nueva_poblacion[i];
				peor_nueva_generacion = i;
			}

			num_evaluaciones++;
		}

		//Una vez que tenemos las tasas de la poblacion, si hemos perdido el mejor padre de la anterior
		// sustituimos la peor solución por este padre:
		if(hemos_perdido_mejor_padre){
			nueva_poblacion[peor_nueva_generacion]=poblacion_actual[mejor_padre_poblacion_actual];
			tasas_nueva_poblacion[peor_nueva_generacion]= tasas[mejor_padre_poblacion_actual];
		}

		//Cambiamos los valores para la siguiente generación:
		hemos_perdido_mejor_padre = true;
		tasas = tasas_nueva_poblacion;
		poblacion_actual = nueva_poblacion;

		//Si ya llevamos 10 generaciones aplicamos la búsqueda local
		if((generacion %10)== 0){
			

			for(int i = 0; i < poblacion_actual.size(); i++){
				double la_tasa;
				poblacion_actual[i] = BL(indices_datos, la_tasa, poblacion_actual[i]).first;
				tasas[i] = la_tasa;

				num_evaluaciones+= 20*datos[0].size()+1;
			}			

		}

		generacion ++;
		
	}


	//devolvemos la mejor solución obtenida y el tiempo:
	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	double mejor_tasa = 0;
	int mejor;
	for(int i = 0; i < tasas.size(); i++){
		if(tasas[i] > mejor_tasa){
			mejor = i;
			mejor_tasa = tasas[i];
		}
	}

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = poblacion_actual[mejor];

	return(pesos_tiempo);

}



//Algortimo memético,  AGG_CA, aplicando una búsqueda local a toda la población. 
pair<vector<double>, double> Algoritmos::AM_10_01(const vector<int> & indices_datos){

	//Vector de indices para realizar luego la BL sobre el 10 porciento aleatorio de la poblacion:
	vector<int> aleatorio;
	aleatorio.reserve(10);
	for(int i = 0; i < 10; i++){
		aleatorio.push_back(i);
	}


	//contador para llevar cuantas evaluaciones llevamos:
	int num_evaluaciones = 0;

	pair<vector<double>, double> pesos_tiempo;
	//generamos una poblacion aleatoria inicial, tomando esta como mejor solucion encontrada
	//al comienzo:

	//generamos los individuos de forma aleatoria:
	vector<double> individuo;

	//matriz donde almacenamos la población:
	vector<vector<double> > poblacion_actual;
	for(int i = 0; i < 10; i++){
		individuo.clear();
		individuo.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			individuo.push_back(f);
		}
		poblacion_actual.push_back(individuo);
	}

	//contador para saber la generacion que llevamos hasta ahora:
	int generacion = 1;

	//Una vez aquí tenemos una población de 10 individuos generados aleatoriamente.
	// procedemos a la evaluación
	
	vector<double> tasas;
	tasas.reserve(10);
	for(int i = 0; i < 10; i++){
		tasas.push_back(knn(indices_datos, indices_datos, poblacion_actual[i], true, basura, basura).first);
		num_evaluaciones++;
	}

	bool hemos_perdido_mejor_padre = true;
	int mejor_padre_poblacion_actual;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución


	while(num_evaluaciones < 15000){
		//calculamos el mejor padre de la generacion actual:
		mejor_padre_poblacion_actual = 0;
		double max_tasa = tasas[0];
		for(int i = 1; i < tasas.size(); i++){
			if(tasas[i] > max_tasa){
				max_tasa = tasas[i];
				mejor_padre_poblacion_actual = i;
			}

		}

		//Una vez tenemos las tasas y sus pesos ordenados de menor a mayor, llamamos a la función torneo binario
		// que nos devolverá indices de 10 padres: 
		int num_padres = 20;
		vector<int> padres_cruzar = torneoBinario(tasas, num_padres, 10);

		//Calculamos la esperanza matemática para saber cuantos cruces vamos a realizar, teniendo en cuenta que la
		// probabilidad de cruce es de 0.7*M
		int num_cruces = (0.7*padres_cruzar.size()/2);

		//Reproducimos el número de padres, creando una nueva poblacion (al ser generacional
		// la nueva generación sustituye a la actual:
		vector<vector<double> > nueva_poblacion;
		int j = 0;
		for(int i = 0; i < num_cruces; i++, j+=2){
			vector<double> h1;
			operadorCruceAritmetico(poblacion_actual[padres_cruzar[j]], poblacion_actual[padres_cruzar[j+1]], h1);
			//aniadimos los hijos a una nueva generación
			nueva_poblacion.push_back(h1);
			
		}

		

		

		//Aniadimos los padres que no se han cruzado:
		for(int i = nueva_poblacion.size(); i < poblacion_actual.size(); i++,j++){
			nueva_poblacion.push_back(poblacion_actual[padres_cruzar[j]]);
			//tenemos encuenta si hemos aniadido al mejor padre de la poblacion actual
			// para aniadirlo en un futuro si no;
			if(i == mejor_padre_poblacion_actual){
				hemos_perdido_mejor_padre = false;
			}
		}
		


		

		//ahora calculamos el número de mutaciones que vamos a realizar (con una probabilidad de 0.001) redondeando:
		int num_mutaciones = 0.001 *  poblacion_actual.size() * poblacion_actual[0].size() +1;

		for(int i = 0; i < num_mutaciones; i++){
			int individuo_a_mutar = rand()%poblacion_actual.size();
			int gen_a_mutar = rand()%poblacion_actual[0].size();

			nueva_poblacion[individuo_a_mutar] = generarVecino(nueva_poblacion[individuo_a_mutar], gen_a_mutar);
		}


		//Ahora generamos las tasas de la nueva población:
		vector<double> tasas_nueva_poblacion;

		//Calculamos el peor de la nueva generación:
		int peor_nueva_generacion;
		double min_tasa = 101;
		tasas_nueva_poblacion.reserve(10);
		for(int i = 0; i < 10; i++){
			tasas_nueva_poblacion.push_back(knn(indices_datos, indices_datos, nueva_poblacion[i], true, basura, basura).first);

			//calculamos el peor de la generación:
			if(tasas_nueva_poblacion[i] < min_tasa){
				min_tasa = tasas_nueva_poblacion[i];
				peor_nueva_generacion = i;
			}

			num_evaluaciones++;
		}

		//Una vez que tenemos las tasas de la poblacion, si hemos perdido el mejor padre de la anterior
		// sustituimos la peor solución por este padre:
		if(hemos_perdido_mejor_padre){
			nueva_poblacion[peor_nueva_generacion]=poblacion_actual[mejor_padre_poblacion_actual];
			tasas_nueva_poblacion[peor_nueva_generacion]= tasas[mejor_padre_poblacion_actual];
		}

		//Cambiamos los valores para la siguiente generación:
		hemos_perdido_mejor_padre = true;
		tasas = tasas_nueva_poblacion;
		poblacion_actual = nueva_poblacion;

		//Si ya llevamos 10 generaciones aplicamos la búsqueda local al 10 % de la poblacion
		if((generacion %10)== 0){
			//calculamos el numero de BL a realizar
			int n_busquedas_locales = 0.1 * poblacion_actual.size();
			//mezclamos aleatoriamente:
			random_shuffle(aleatorio.begin(), aleatorio.end());

			for(int i = 0; i < n_busquedas_locales; i++){
				double la_tasa;
				poblacion_actual[i] = BL(indices_datos, la_tasa, poblacion_actual[aleatorio[i]]).first;
				tasas[i] = la_tasa;

				num_evaluaciones+= 20*datos[0].size()+1;
			}			

		}

		generacion ++;
		
	}


	//devolvemos la mejor solución obtenida y el tiempo:
	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	double mejor_tasa = 0;
	int mejor;
	for(int i = 0; i < tasas.size(); i++){
		if(tasas[i] > mejor_tasa){
			mejor = i;
			mejor_tasa = tasas[i];
		}
	}

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = poblacion_actual[mejor];

	return(pesos_tiempo);

}




//Algortimo memético,  AGG_CA, aplicando una búsqueda local a toda la población. 
pair<vector<double>, double> Algoritmos::AM_10_01_mej(const vector<int> & indices_datos){


	//contador para llevar cuantas evaluaciones llevamos:
	int num_evaluaciones = 0;

	pair<vector<double>, double> pesos_tiempo;
	//generamos una poblacion aleatoria inicial, tomando esta como mejor solucion encontrada
	//al comienzo:

	//generamos los individuos de forma aleatoria:
	vector<double> individuo;

	//matriz donde almacenamos la población:
	vector<vector<double> > poblacion_actual;
	for(int i = 0; i < 10; i++){
		individuo.clear();
		individuo.reserve(datos[0].size());

		for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			individuo.push_back(f);
		}
		poblacion_actual.push_back(individuo);
	}

	//contador para saber la generacion que llevamos hasta ahora:
	int generacion = 1;

	//Una vez aquí tenemos una población de 10 individuos generados aleatoriamente.
	// procedemos a la evaluación
	
	vector<double> tasas;
	tasas.reserve(10);
	for(int i = 0; i < 10; i++){
		tasas.push_back(knn(indices_datos, indices_datos, poblacion_actual[i], true, basura, basura).first);
		num_evaluaciones++;
	}

	bool hemos_perdido_mejor_padre = true;
	int mejor_padre_poblacion_actual;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución


	while(num_evaluaciones < 15000){
		//calculamos el mejor padre de la generacion actual:
		mejor_padre_poblacion_actual = 0;
		double max_tasa = tasas[0];
		for(int i = 1; i < tasas.size(); i++){
			if(tasas[i] > max_tasa){
				max_tasa = tasas[i];
				mejor_padre_poblacion_actual = i;
			}

		}

		//Una vez tenemos las tasas y sus pesos ordenados de menor a mayor, llamamos a la función torneo binario
		// que nos devolverá indices de 10 padres: 
		int num_padres = 20;
		vector<int> padres_cruzar = torneoBinario(tasas, num_padres, 10);

		//Calculamos la esperanza matemática para saber cuantos cruces vamos a realizar, teniendo en cuenta que la
		// probabilidad de cruce es de 0.7*M
		int num_cruces = (0.7*padres_cruzar.size()/2);

		//Reproducimos el número de padres, creando una nueva poblacion (al ser generacional
		// la nueva generación sustituye a la actual:
		vector<vector<double> > nueva_poblacion;
		int j = 0;
		for(int i = 0; i < num_cruces; i++, j+=2){
			vector<double> h1;
			operadorCruceAritmetico(poblacion_actual[padres_cruzar[j]], poblacion_actual[padres_cruzar[j+1]], h1);
			//aniadimos los hijos a una nueva generación
			nueva_poblacion.push_back(h1);
			
		}

		

		

		//Aniadimos los padres que no se han cruzado:
		for(int i = nueva_poblacion.size(); i < poblacion_actual.size(); i++,j++){
			nueva_poblacion.push_back(poblacion_actual[padres_cruzar[j]]);
			//tenemos encuenta si hemos aniadido al mejor padre de la poblacion actual
			// para aniadirlo en un futuro si no;
			if(i == mejor_padre_poblacion_actual){
				hemos_perdido_mejor_padre = false;
			}
		}
		


		

		//ahora calculamos el número de mutaciones que vamos a realizar (con una probabilidad de 0.001) redondeando:
		int num_mutaciones = 0.001 *  poblacion_actual.size() * poblacion_actual[0].size() +1;

		for(int i = 0; i < num_mutaciones; i++){
			int individuo_a_mutar = rand()%poblacion_actual.size();
			int gen_a_mutar = rand()%poblacion_actual[0].size();

			nueva_poblacion[individuo_a_mutar] = generarVecino(nueva_poblacion[individuo_a_mutar], gen_a_mutar);
		}


		//Ahora generamos las tasas de la nueva población:
		vector<double> tasas_nueva_poblacion;

		//Calculamos el peor de la nueva generación:
		int peor_nueva_generacion;
		double min_tasa = 101;
		tasas_nueva_poblacion.reserve(10);
		for(int i = 0; i < 10; i++){
			tasas_nueva_poblacion.push_back(knn(indices_datos, indices_datos, nueva_poblacion[i], true, basura, basura).first);

			//calculamos el peor de la generación:
			if(tasas_nueva_poblacion[i] < min_tasa){
				min_tasa = tasas_nueva_poblacion[i];
				peor_nueva_generacion = i;
			}

			num_evaluaciones++;
		}

		//Una vez que tenemos las tasas de la poblacion, si hemos perdido el mejor padre de la anterior
		// sustituimos la peor solución por este padre:
		if(hemos_perdido_mejor_padre){
			nueva_poblacion[peor_nueva_generacion]=poblacion_actual[mejor_padre_poblacion_actual];
			tasas_nueva_poblacion[peor_nueva_generacion]= tasas[mejor_padre_poblacion_actual];
		}

		//Cambiamos los valores para la siguiente generación:
		hemos_perdido_mejor_padre = true;
		tasas = tasas_nueva_poblacion;
		poblacion_actual = nueva_poblacion;

		//Si ya llevamos 10 generaciones aplicamos la búsqueda local sobre el 
		// 0.1 * tam_poblacion mejores, en este caso, como 0.1*10 = 1, aplicamos sobre
		// el mejor de la poblacion.
		if((generacion %10)== 0){
			

			
				double la_tasa;
				poblacion_actual[mejor_padre_poblacion_actual] = BL(indices_datos, la_tasa, poblacion_actual[mejor_padre_poblacion_actual]).first;
				tasas[mejor_padre_poblacion_actual] = la_tasa;

				num_evaluaciones+= 20*datos[0].size()+1;
					

		}

		generacion ++;
		
	}


	//devolvemos la mejor solución obtenida y el tiempo:
	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	
	//Calculamos los microsegundos empleados
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	double mejor_tasa = 0;
	int mejor;
	for(int i = 0; i < tasas.size(); i++){
		if(tasas[i] > mejor_tasa){
			mejor = i;
			mejor_tasa = tasas[i];
		}
	}

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = poblacion_actual[mejor];

	return(pesos_tiempo);

}


double Algoritmos::generarTemperatura(double temp_actual, double temp_inicial, double temp_final, int max_enfriamientos){

	double beta = (temp_inicial- temp_final)/(max_enfriamientos*temp_inicial*temp_final);
	return(temp_actual/(1+beta*temp_actual));
}




pair<vector<double> , double> Algoritmos::SA(const vector<int> & indices_datos){
	pair<vector<double>, double> pesos_tiempo;

	//Calculamos una solución inicial de forma aleatoria:
	vector<double> solucion_actual;
	solucion_actual.reserve(datos[0].size());

	for(int i = 0; i < datos[0].size(); i++){
			double f = (double)rand() / RAND_MAX;
			solucion_actual.push_back(f);
	}

	//Una vez tenemos la solución inicial, calculamos la temperatura inicial:
	double coste_actual = knn(indices_datos, indices_datos, solucion_actual, true, basura, basura).first;

	//Almacenamos la mejor solución:
	vector<double> mejor_solucion = solucion_actual;
	double coste_mejor_solucion = coste_actual;

	//Calculamos la temperatura inicial:
	double temp_inicial = 0.3*coste_actual/(-log(0.3));
	double temp_actual = temp_inicial;

	//Fijamos las condiciones del algortimo:
	int max_vecinos = 10 * datos[0].size();
	int max_exitos = 0.1 * max_vecinos;
	int max_evaluaciones = 15000;
	int max_enfriamientos = max_evaluaciones /max_vecinos;
	//max_enfriamientos = 1;
	
	double temp_final = 0.001;
	int enfriamientos_realizados = 0;
	//Comenzamos la ejecución del algortimo:
	int num_evaluaciones = 1;

	auto start_time = chrono::high_resolution_clock::now();//medimos el tiempo de ejecución
	//cout << "Max vecinos: "<< max_vecinos << endl;
	//cout << "MAX EXFRIAMIENTOS: " << max_enfriamientos<< endl;

	int exitos = -1;

	while( (enfriamientos_realizados< max_enfriamientos) && (num_evaluaciones < max_evaluaciones)
		&&(exitos !=0)){
		
		int vecinos_generados = 0;
		exitos = 0;
		while((vecinos_generados < max_vecinos)&& (exitos <max_exitos)&& (num_evaluaciones < max_evaluaciones)){

			vector<double> vecino = generarVecino(solucion_actual, rand()%datos[0].size());
			vecinos_generados++;

			double coste_vecino = knn(indices_datos, indices_datos, vecino, true, basura, basura).first;
			num_evaluaciones++;
			double dif_coste = coste_vecino - coste_actual;
			//Generamos un valor de una distribución uniforme (0,1):
			double unif = uniforme(generator);
			
			if((dif_coste > 0)|| (unif <= exp(-dif_coste/enfriamientos_realizados*temp_actual))   ){
				solucion_actual = vecino;
				coste_actual = coste_vecino;
				exitos++;
				

				//Comprobamos si es mejor que la mejor solucion, y en ese caso actualizamos esta:
				if(coste_actual > coste_mejor_solucion){
					mejor_solucion = solucion_actual;
					coste_mejor_solucion = coste_actual;
					

				}

			}
			

		}

		
		//Calculamos la temperatura de esta iteracion:
		temp_actual = generarTemperatura(temp_actual, temp_inicial, temp_final, max_enfriamientos);
		enfriamientos_realizados++;
	}

	auto end_time = chrono::high_resolution_clock::now(); //paramos de medir tiempo.
	double microseconds = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	pesos_tiempo.second = microseconds/1000000;	//pasamos a segundos
	pesos_tiempo.first = mejor_solucion;


	return (pesos_tiempo);
}
