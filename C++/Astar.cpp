#include <iostream>
#include <vector>
#include <queue>
#include <stdlib.h>
using namespace std;



struct nodo{
	double x;
	double y;
	nodo *p;
	double f;
	double g;
	double h;
	bool operator ==(nodo &n){
		if(x == n.x && y == n.y)
			return true;
		else
			return false;
	}

};

struct prioridadInversa{
	bool operator()(nodo &n1, nodo &n2){
		if(n1.f >= n2.f)
			return true;
		else
			return false;
	}
};



bool termino(int tamFull, nodo n){
	nodo *aux;
	aux = &n;
	int tam = 0;

	while(aux != 0){
		tam++;
		aux = aux->p;
	}

	if(tam == tamFull)
		return true;
	else
		return false;
}

bool estaNodo(vector<nodo> lista, nodo n){
	for(vector<nodo>::iterator it = lista.begin(); it != lista.end(); it++)
		if(*it == n)
			return true;

	return false;
}


vector<nodo> expandirHijos(nodo n, vector<nodo> listaNodos){
	vector<nodo> listaPadres;
	nodo *aux;
	aux = &n;

	while (aux != 0){
		listaPadres.push_back(*aux);
		aux = aux->p;
	}



	vector<nodo> listaHijos;
	for(vector<nodo>::iterator it = listaNodos.begin(); it != listaNodos.end(); it++)
		if(estaNodo(listaPadres, *it) == false)
			listaHijos.push_back(*it);


	return listaHijos;

}




double obtenerCoste(nodo padre, nodo hijo, vector<vector<double> > m, vector<nodo> listaNodos){
	int posP=0, posH=0;
	int i=0;
	for(vector<nodo>::iterator it = listaNodos.begin(); it != listaNodos.end(); it++){
		if(padre == *it)
			posP = i;
		else if(hijo == *it)
			posH = i;

		i++;
	}
	return m[posP][posH];
}

//Introducir matriz m con los costes y un vector con los nodos.
vector<nodo> a_estrella(vector<vector<double> > m, vector<nodo> listaNodos){
	priority_queue<nodo, vector<nodo>, prioridadInversa> abiertos;
	vector<nodo> cerrados;
	//vector para almacenar los nodos sin explorar y obtener h, es un vector de abiertos sin prioridad;
	vector<nodo> sinExplorar;

	abiertos.push(listaNodos.front());
	sinExplorar.push_back(listaNodos.front());
	nodo *n; //En este nodo acabara el nodo hoja o solucion.

	do{
		n = new nodo;
 		*n = abiertos.top();

		abiertos.pop();
		cerrados.push_back(*n);
		sinExplorar.pop_back();
		//Expandir los hijos
		vector<nodo> hijos = expandirHijos(*n, listaNodos);

		//meter los hijos en una vector de nodos sin explorar
		for(vector<nodo>::iterator it = hijos.begin(); it != hijos.end(); it++){
			sinExplorar.push_back(*it);
		}

		for(vector<nodo>::iterator it = hijos.begin(); it != hijos.end(); it++){
			nodo hijo = *it;

			//Obtener g del nodo
			hijo.g = obtenerCoste(*n, hijo, m, listaNodos);
			if(hijo.p != 0)
				hijo.g += hijo.p->g;

			//Obtener h del nodo
			hijo.h = 999999;
			for(vector<nodo>::iterator it2 = sinExplorar.begin(); it2 != sinExplorar.end(); it2++){
				double valor = 	obtenerCoste(hijo, *it2, m, listaNodos);
				if(hijo.h > valor)
					hijo.h=valor;
			}
			//Obtener f del nodo f(n) = g(n) + h(n)
			hijo.f = hijo.g + hijo.h;

			//anadir hijo de n a abiertos para explorar.
			hijo.p = n;
			abiertos.push(hijo);

		}



	}while(termino(listaNodos.size(), *n) == false);

	//Crear el vector de nodos solucion apartir del nodo hijo y todos los padres.
	vector<nodo> solucion;
	while(n!=0){
		solucion.push_back(*n);
		n = n->p;
	}

	return solucion;

}



int main(){
	vector<vector<double> > m(4);
	m[0].resize(4);
	m[1].resize(4);
	m[2].resize(4);
	m[3].resize(4);

	m[0][0] = 0;
	m[1][1] = 0;
	m[2][2] = 0;
	m[3][3] = 0;
	m[0][1] = m[1][0] = 3;
	m[0][2] = m[2][0] = 4;
	m[0][3] = m[3][0] = 1;
	m[1][2] = m[2][1] = 8;
	m[1][3] = m[3][1] = 5;
	m[2][3] = m[3][2] = 2;
	vector<nodo> nodos;
	nodo p1, p2, p3, p4;
	p1.x = 1, p2.x = 2, p3.x = 3, p4.x = 4;
	p1.y = 0, p2.y = 0, p3.y = 0, p4.y = 0;
	p1.p = 0, p2.p = 0, p3.p = 0, p4.p = 0;




	nodos.push_back(p1);
	nodos.push_back(p2);
	nodos.push_back(p3);
	nodos.push_back(p4);

	//cout << obtenerCoste(p2, p1, m, nodos) << endl;

	vector<nodo> solucion = a_estrella(m, nodos);

	for(vector<nodo>::iterator it = solucion.begin(); it != solucion.end(); it++)
		cout << "P" << it->x << " ";

	cout << endl;







}
