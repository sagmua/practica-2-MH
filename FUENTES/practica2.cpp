 #include <iostream>
#include "algoritmos.h"
#include <fstream>      // std::fstream

using namespace std;





int main(int argc, char ** argv){
	fstream salida;
  	

  	

	//recibimos el fichero de datos que deseamos ejecutar de los tres que tenemos:
	double tasa_clas, tasa_red;

	if(argc != 2){
		cerr << "ERROR ARGUMENTOS. USO: \"./practica1 <nombre_base_datos>\" "<<endl;
		cerr << "Los nombres disponibles como argumento son: "<<endl;
		cerr << "\t \"cancer\" "<<endl;
		cerr << "\t \"sonar\" "<<endl;
		cerr << "\t \"spam\" "<<endl;
		exit(-1);

	}
	char cancer[] = "cancer";
	char spam[] = "spam";
	char sonar[] = "sonar";

	string datos_deseados;
	if(strcmp(argv[1], cancer)==0){
		datos_deseados = "cancer_normalizado.csv";
	}
	else if (strcmp(argv[1], sonar)==0){
		datos_deseados = "sonar_normalizado.csv";
	}
	else if (strcmp(argv[1], spam)==0){
		datos_deseados = "spam_normalizado.csv";
	}	
	else{
		cerr << "ERROR ARGUMENTOS. USO: \"./practica1 <nombre_base_datos>\" "<<endl;
		cerr << "Los nombres disponibles como argumento son: "<<endl;
		cerr << "\t \"cancer\" "<<endl;
		cerr << "\t \"sonar\" "<<endl;
		cerr << "\t \"spam\" "<<endl;
		exit(-1);
	}

	string fichero_salida = argv[1];
	fichero_salida+= "_salida.txt";
	salida.open (fichero_salida, std::fstream::in | std::fstream::out | std::fstream::app);


	Algoritmos algoritmo(datos_deseados);
	pair<double, double> error_tiempo;
	pair <vector<double> , double> pesos_tiempo;
	double media_tasa_clas, media_tasa_red, media_fun_objetivo, media_tiempo;
	
	

	/*
	//************* ALGORITMO KNN *********
	
	
	cout << "\n************* ALGORITMO 1NN *********\n";
	salida << "\n************* ALGORITMO 1NN *********\n";
	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;

	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl << "-----PARTICION " << i+1<<  endl;
		cout << "TRAIN/TEST:"<<endl;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first, algoritmo.particiones[i].second
			,vector<double> (algoritmo.datos[i].size(), 1), false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< error_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << error_tiempo.second << endl;

		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA KNN: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION DE MEDIA KNN: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION DE MEDIA KNN: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO DE MEDIA KNN: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;





//************* ALGORITMO RELIEF *********
	cout << "\n************* ALGORITMO RELIEF *********\n";
	salida << "\n************* ALGORITMO RELIEF *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.relief(algoritmo.particiones[i].first, algoritmo.particiones[i].first);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO RELIEF MEDIA DE: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION RELIEF MEDIA DE: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION RELIEF MEDIA DE: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO RELIEF MEDIA DE: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;





	//************* ALGORITMO SA *********
	cout << "\n************* ALGORITMO SA *********\n";
	salida << "\n************* ALGORITMO SA *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.SA(algoritmo.particiones[i].first);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA SA: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION DE MEDIA SA: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION DE MEDIA SA: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO DE MEDIA SA: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;

*/

	//************* ALGORITMO ILS *********
	cout << "\n************* ALGORITMO ILS *********\n";
	salida << "\n************* ALGORITMO ILS *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.ILS(algoritmo.particiones[i].first);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA ILS: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION DE MEDIA ILS: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION DE MEDIA ILS: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO DE MEDIA ILS: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;





//************* ALGORITMO DE *********
	cout << "\n************* ALGORITMO DE *********\n";
	salida << "\n************* ALGORITMO DE *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.DE_RAND(algoritmo.particiones[i].first);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA DE: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION DE MEDIA DE: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION DE MEDIA DE: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO DE MEDIA DE: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;

	

//************* ALGORITMO DE_CURRENT_TO_BEST *********
	cout << "\n************* ALGORITMO DE_CURRENT_TO_BEST *********\n";
	salida << "\n************* ALGORITMO DE_CURRENT_TO_BEST *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.DE_CURRENT_TO_BEST(algoritmo.particiones[i].first);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE_CURRENT_TO_BEST MEDIA DE: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION DE_CURRENT_TO_BEST MEDIA DE: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION DE_CURRENT_TO_BEST MEDIA DE: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO DE_CURRENT_TO_BEST MEDIA DE: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;






//************* ALGORITMO BL *********
	cout << "\n************* ALGORITMO BL *********\n";
	salida << "\n************* ALGORITMO BL *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	double b;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.BL(algoritmo.particiones[i].first, b);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO BL MEDIA DE: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION BL MEDIA DE: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION BL MEDIA DE: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO BL MEDIA DE: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;


	//************* ALGORITMO AGG_BLX *********
	cout << "\n************* ALGORITMO AGG_BLX *********\n";
	salida << "\n************* ALGORITMO AGG_BLX *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AGG_BLX(algoritmo.particiones[i].first);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO AGG_BLX MEDIA DE: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION AGG_BLX MEDIA DE: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION AGG_BLX MEDIA DE: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO AGG_BLX MEDIA DE: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;



	//************* ALGORITMO AM_10_01_mej *********
	cout << "\n************* ALGORITMO AM_10_01_mej *********\n";
	salida << "\n************* ALGORITMO AM_10_01_mej *********\n";

	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AM_10_01_mej(algoritmo.particiones[i].first);
		
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first, false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;

		salida << tasa_clas << " " << tasa_red << " " << error_tiempo.first << " " << pesos_tiempo.second << endl;
		
		media_tasa_clas += tasa_clas;
		media_tasa_red+= tasa_red;
		media_fun_objetivo+=error_tiempo.first;
		media_tiempo+=error_tiempo.second;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO AM_10_01_mej MEDIA DE: "<< media_tiempo/5.0<< endl;
	cout << "TASA CLASIFICACION AM_10_01_mej MEDIA DE: "<< media_tasa_clas/5.0<< endl;
	cout << "TASA REDUCCION AM_10_01_mej MEDIA DE: "<< media_tasa_red/5.0<< endl;
	cout << "FUNCION OBJETIVO AM_10_01_mej MEDIA DE: "<< media_fun_objetivo/5.0<< endl;
	cout << endl << "****************" << endl;




	

	return 0;
}


