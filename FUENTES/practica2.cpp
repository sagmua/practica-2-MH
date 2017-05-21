 #include <iostream>
#include "algoritmos.h"
using namespace std;





int main(int argc, char ** argv){

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

	Algoritmos algoritmo(datos_deseados);
	pair<double, double> error_tiempo;
	pair <vector<double> , double> pesos_tiempo;
	double media_tasa_clas, media_tasa_red, media_fun_objetivo, media_tiempo;


	/*
	cout << endl << "--------------DATOS ELEGIDOS: " << datos_deseados<< " -------"<<endl;
	
	cout << "Tamanio datos: " << algoritmo.datos.size() << endl;

	for(auto it = algoritmo.particiones.begin(); it != algoritmo.particiones.end(); it++){
		cout << endl << "tamanio TRAIN: " << (*it).first.size();
		cout << endl << "tamanio TEST: " << (*it).second.size();
		cout << endl << "tamanio TOTAL: " << (*it).second.size() + (*it).first.size();
		//algoritmo.imprime(algoritmo.datos[((*it).second)[0]]);
		cout << endl << "------";

	}
	*/
	
	
	//************* ALGORITMO AS *********
	cout << "\n************* ALGORITMO SA *********\n";

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




	//************* ALGORITMO KNN *********
	
	
	cout << "\n************* ALGORITMO 1NN *********\n";
	media_tiempo = 0;
	media_tasa_clas = 0;
	media_tasa_red = 0;
	media_fun_objetivo = 0;

	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << "-----PARTICION " << i+1<<  endl;
		cout << "TRAIN/TEST:"<<endl;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first, algoritmo.particiones[i].second
			,vector<double> (algoritmo.datos[i].size(), 1), false, tasa_red, tasa_clas);
		cout << endl << "El tiempo empleado es: "<< error_tiempo.second << endl;
		cout << "La tasa de clasificacion es: "<< tasa_clas << endl;
		cout << "La tasa de reduccion es: "<< tasa_red << endl;
		cout << "La funcion objetivo es: "<< error_tiempo.first;
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

/*

	
	//************* ALGORITMO RELIEF ********* /
	cout << "\n************* ALGORITMO RELIEF *********\n";
	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.relief(algoritmo.particiones[i].first, algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.relief(algoritmo.particiones[i].second, algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA RELIEF: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA RELIEF: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;
	

	
	// **************ALGORITMO BL ************** /
		
	cout << "\n************* ALGORITMO BL *********\n";
	media_tiempo = 0;
	media_tasas = 0;
	double basura;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.BL(algoritmo.particiones[i].first, basura);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.BL(algoritmo.particiones[i].second, basura);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA BL: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA BL: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;



	
	cout << "********ALGORTIMO GENÉTICO***************"<< endl;

	
	cout << "********AGG-BLX***************" << endl;
	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AGG_BLX(algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.AGG_BLX(algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA AGG-BLX: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA AGG-BLX: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;

	

	
	
	
	cout << "********AGG-CA***************" << endl;
	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AGG_CA(algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.AGG_CA(algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA AGG-AC: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA AGG-AC: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;
	
	
	


	cout << "********AGE-BLX***************" << endl;
	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AGE_BLX(algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.AGE_BLX(algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA AGE-BLX: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA AGE-BLX: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;



	

	cout << "********AGE-CA***************" << endl;

	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AGE_CA(algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.AGE_CA(algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA AGE-CA: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA AGE-CA: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;





	

	cout << "********ALGORTIMO MEMETICO***************"<< endl;

	
	cout << "********AM_10_10***************" << endl;

	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AM_10_10(algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.AM_10_10(algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA AGE-CA: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA AGE-CA: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;



	

	cout << "********AM_10_01***************" << endl;

	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AM_10_01(algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.AM_10_01(algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA AGE-CA: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA AGE-CA: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;
	


	cout << "********AM_10_01_mej***************" << endl;

	media_tiempo = 0;
	media_tasas = 0;
	for(int i = 0; i < algoritmo.particiones.size(); i++){
		cout << endl<< "-----PARTICION " << i+1 << "----"<< endl;

		cout << "TRAIN/TEST:"<<endl;
		pesos_tiempo = algoritmo.AM_10_01_mej(algoritmo.particiones[i].first);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].first,algoritmo.particiones[i].second, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

		cout <<endl<<  "TEST/TRAIN:"<<endl;
		pesos_tiempo = algoritmo.AM_10_01_mej(algoritmo.particiones[i].second);
		cout << endl << "El tiempo empleado es: "<< pesos_tiempo.second << endl;
		media_tiempo+=pesos_tiempo.second;
		error_tiempo = algoritmo.knn(algoritmo.particiones[i].second,algoritmo.particiones[i].first, pesos_tiempo.first);
		cout << "La tasa de clasificacion es: "<< error_tiempo.first;
		media_tasas += error_tiempo.first;

	}

	cout << endl << "****************" << endl;
	cout << "TIEMPO DE MEDIA AGE-CA: "<< media_tiempo/10.0<< endl;
	cout << "TASA DE MEDIA AGE-CA: "<< media_tasas/10.0<< endl;
	cout << endl << "****************" << endl;
	
	*/
	

	return 0;
}


