
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

class Punto
{
private:
	int IDpunto, IDcluster;
	int dimensiones;
	vector<double> valores;

public:
	Punto(int id_punto, string cadena)
	{
		dimensiones = NULL;
		IDpunto = id_punto;
		stringstream s(cadena);    //rompe las cadenas
		double valor;              // Almacena la cadena en un double
		while (s >> valor)
		{
			valores.push_back(valor);
			dimensiones++;                //Asigna el numero de columnas como el numero de dimensiones 
		}
		IDcluster = 0;
	}

	int obtenerID()
	{
		return IDpunto;
	}

	int obtenerCluster()
	{
		return IDcluster;
	}

	int obtenerDimensiones()
	{
		return dimensiones;
	}

	double darValor(int pos)
	{
		return valores[pos];       //posicion de cada fila en un dataSet vectorial
	}

	void darCluster(int valor)
	{
		IDcluster = valor;
	}

};

///////////Distancia Euclediana////////////
/*float Euclediana(Punto a, Punto b)
{

}*/


class Cluster
{
private:
	int IDcluster;
	vector<double> centroide;
	vector<Punto> Cpuntos;
public:
	Cluster(int IDcluster, Punto centroide)
	{
		this->IDcluster = IDcluster;
		for (auto i = 0; i < centroide.obtenerDimensiones(); i++)    //Determianr el punto centroide con sus valores correspondientes
			this->centroide.push_back(centroide.darValor(i));
		this->agregarPunto(centroide);                           //darle al punto centroide el id de su cluster
	}

	void agregarPunto(Punto pt)                 //agrega un punto a un determinado cluster
	{
		pt.darCluster(this->IDcluster);
		Cpuntos.push_back(pt);
	}

	bool eliminarPunto(int IDpunto)                //Elimina un punto de un determinado cluster
	{
		int tam = Cpuntos.size();
		for (auto i = 0; i < tam; i++)
		{
			if (Cpuntos[i].obtenerID() == IDpunto)    //utilizando el id del punto
			{
				Cpuntos.erase(Cpuntos.begin() + i);
				return 1;
			}
		}
		return 0;
	}

	int obtenerId()
	{
		return IDcluster;
	}

	Punto obtenerPunto(int pos)
	{
		return Cpuntos[pos];
	}

	int obtenerTam()
	{
		return Cpuntos.size();
	}

	double obtenerPosCentroide(int pos)
	{
		return centroide[pos];
	}

	void darPosCentroide(int pos, double valor)
	{
		this->centroide[pos] = valor;
	}
};


class PowerKmeans
{
private:
	int K, iteraciones, dimensiones, puntosTotales;
	vector<Cluster> Kclusters;

	int clusterMasCercanoID(Punto punto)             //retornar el identificador del cluster mas cercano a un punto
	{
		double suma = 0.0, distanciaMin;
		int IDclusterMasCercano;

		for (auto i = 0; i < dimensiones; i++)

			suma += pow(Kclusters[0].obtenerPosCentroide(i) - punto.darValor(i), 2);  

		distanciaMin = sqrt(suma);                                                   //distanciaMin es la distancia euclediana 
		IDclusterMasCercano = Kclusters[0].obtenerId();   //obtener el id del cluster mas cercano a un punto

		for (auto i = 0; i < K; i++)
		{
			double distancia;
			suma = 0.0;

			for (auto j = 0; j < dimensiones; j++)

				suma += pow(Kclusters[i].obtenerPosCentroide(j) - punto.darValor(j), 2); //Compara la distancia de cada punto con los k centroides

			distancia = sqrt(suma);

			if (distancia < distanciaMin)
			{
				distanciaMin = distancia;
				IDclusterMasCercano = Kclusters[i].obtenerId();
			}
		}
		return IDclusterMasCercano;
	}

	///////ya tengo el id del cluster mas cercano a un punto///////

public:
	PowerKmeans(int K, int iteracion)
	{
		this->K = K;
		this->iteraciones = iteracion;
	}

	void Kmeans(vector<Punto>& setPuntos)                   //Algoritmo de k-means partitioning
	{
		puntosTotales = setPuntos.size();
		dimensiones = setPuntos[0].obtenerDimensiones();

		vector<int> IDpuntos;

		for (auto i = 0; i <= K; i++)
		{
			while (true)
			{
				int indice = rand() % puntosTotales;
				if (find(IDpuntos.begin(), IDpuntos.end(), indice) == IDpuntos.end())
				{
					IDpuntos.push_back(indice);
					setPuntos[indice].darCluster(i);
					Cluster cluster(i, setPuntos[indice]);
					Kclusters.push_back(cluster);
					break;
				}
			}
		}

		cout << "En cuantos clusters desea dividir el conjunto de datos?   " << Kclusters.size() << endl;
		cout << "\n";

		cout << "Ejecutando el algoritmo K-means, espere..." << endl;

		///////////Necesito agregar asignar k centroides y calcular la distancia euclediana de cada punto con los centroides///////
	

		int iteracion = 1;
		while (true)
		{
			cout << "Iteracion nro: " << iteracion << "de" << iteraciones << endl;
			bool listo = true;

			//Agregar todos los puntos a su cluster mas cercano
			for (auto i = 0; i < puntosTotales; i++)
			{
				int IDclusterActual = setPuntos[i].obtenerCluster();
				int IDclusterVecino = clusterMasCercanoID(setPuntos[i]);

				if (IDclusterActual != IDclusterVecino)
				{
					if (IDclusterActual != 0)
					{
						for (auto j = 0; j < K; j++)
						{
							if (Kclusters[j].obtenerId() == IDclusterActual)
									Kclusters[j].eliminarPunto(setPuntos[i].obtenerID());
						}
					}

					for (auto j = 0; j < K; j++)
					{
						if (Kclusters[j].obtenerId() == IDclusterVecino)
							Kclusters[j].agregarPunto(setPuntos[i]);                //Agrega el punto al cluster mas cercano 
					}
					setPuntos[i].darCluster(IDclusterVecino);
					listo = false;
				}
			}

			////////Ahora necesito asignar un nuevo centroide a cada cluster/////////

			///////Actualizacion de centroides de cada cluster/////////////

			for (auto i = 0; i < K; i++)
			{
				int clusterTam = Kclusters[i].obtenerTam();

				for (auto j = 0; j < dimensiones; j++)
				{
					double suma = 0.0;
					if (clusterTam > 0)
					{
						for (auto pt = 0; pt < clusterTam; pt++)
							suma += Kclusters[i].obtenerPunto(pt).darValor(j);
						Kclusters[i].darPosCentroide(j, suma / clusterTam);
					}
				}
			}

			////////Ya no hay ningun cambio en la agrupacion de los grupos///////////

			if (listo or iteracion >= iteraciones)
			{
				cout << "La agrupacion termino en la iteracion: " << iteracion << endl;
				break;
			}
			iteracion++;
		}

		////////Imprimir los id's de los puntos en cada iteracion////
		for (auto i = 0; i < K; i++)
		{
			cout << "Puntos en el cluster: " << Kclusters[i].obtenerId() << " : ";
			for (auto j = 0; j < Kclusters[i].obtenerTam(); j++)
				cout << Kclusters[i].obtenerPunto(j).obtenerID() << " ";

			cout << "\n";
			cout << "\n";

		}
		cout << "--------------------------------------------" << endl;

		//////Obtener los centroides de los datos del archivo/////////
		ofstream bd;
		bd.open("dataBase.csv");
		if (bd.is_open())
		{
			for (auto i = 0; i < K; i++)
			{
				cout << "Cluster " << Kclusters[i].obtenerId() << " Su centroide: ";
				for (auto j = 0; j < dimensiones; j++)
				{
					//salida en la consola//
					cout << Kclusters[i].obtenerPosCentroide(j) << " "; 
					//salida al archivo//
					bd << Kclusters[i].obtenerPosCentroide(j) << " ";
				}
				cout << endl;
				bd << endl;
			}
			bd.close();
		}
		else
		{
			cout << "Error al leer el archivo" << endl;
		}

	}
	
};


	int main(int argc, char** argv) 
	{
		cout << "****************************ALGORITMO PARA CALCULAR K-means PARTITIONING****************************";
		cout << endl;
		cout << endl;
		cout << "Empezemos!" << endl;

		
		if (argc != 3) {
			cout << "Error";
			return 1;
		}

		
		int K = atoi(argv[2]);

		
		string filename = argv[1];
		ifstream infile(filename.c_str());

		if (!infile.is_open()) {
			cout << "Error: Failed to open file." << endl;
			return 1;
		}

		
		int pointId = 1;
		vector<Punto> all_points;
		string cadena;

		while (getline(infile, cadena)) {
			Punto punto(pointId, cadena);
			all_points.push_back(punto);
			pointId++;
		}
		infile.close();
		cout << "\nData fetched successfully!" << endl << endl;

		
		if (all_points.size() < K) {
			cout << "Error" << endl;
			return 1;
		}

	
		int iteraciones = 100;

		PowerKmeans kmeans(K, iteraciones);
		kmeans.Kmeans(all_points);

		getchar();

		
		return 0;

	}