#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


struct nodoArbol {
    string nombreLugar;
    int codigoLugar;
    int cantidadPersonas;
    nodoArbol* nodoDerecho;
    nodoArbol* nodoIzquierdo;
};

typedef struct nodoArbol *arbol;


nodoArbol *crearNodo(string nom,int cod,int cantidad)
{
	nodoArbol *aux = new (struct nodoArbol);
	aux->nombreLugar=nom;
	aux->codigoLugar=cod;
	aux->cantidadPersonas=cantidad;
	aux->nodoDerecho=NULL;
	aux->nodoIzquierdo=NULL;
	return aux;
}

nodoArbol* agregarNodo(nodoArbol* raiz, string nom, int cod, int cantidad) {
  // Creamos un nuevo nodo con los datos ingresados
  nodoArbol* nuevoNodo = crearNodo(nom, cod, cantidad);

  // Verificamos si el nodo actual es NULL
  if (raiz == NULL) {
    // Si es NULL, entonces el árbol está vacío, así que podemos asignar el nuevo nodo como la raíz
    raiz = nuevoNodo;
  } else {
    // Si el árbol ya tiene nodos, entonces debemos encontrar donde insertar el nuevo nodo
    nodoArbol* actual = raiz;
    while (actual != NULL) {
      if (actual->cantidadPersonas < cantidad) {
        // Si el nodo actual tiene menos personas que el nodo que queremos insertar, podemos insertarlo a la izquierda
        actual->nodoIzquierdo = agregarNodo(actual->nodoIzquierdo, nom, cod, cantidad);
        break;
      } else if (actual->cantidadPersonas > cantidad) {
        // Si el nodo actual tiene más personas que el nodo que queremos insertar, podemos insertarlo a la derecha
        actual->nodoDerecho = agregarNodo(actual->nodoDerecho, nom, cod, cantidad);
        break;
      } else {
        // Si el nodo actual tiene la misma cantidad de personas que el nodo que queremos insertar, podemos insertarlo en cualquier lugar
        actual->nodoIzquierdo = agregarNodo(actual->nodoIzquierdo, nom, cod, cantidad);
        break;
      }
      actual = actual->nodoDerecho;
    }
  }

  return raiz;
}

void modificarDato(nodoArbol*& raiz, int codigo, int nuevaCantidad) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->codigoLugar == codigo) {
        raiz->cantidadPersonas = nuevaCantidad;
    } else {
        modificarDato(raiz->nodoIzquierdo, codigo, nuevaCantidad);
        modificarDato(raiz->nodoDerecho, codigo, nuevaCantidad);
    }
}

int contarHojas(nodoArbol* raiz) {
    if (raiz == NULL) {
        return 0;
    }

    // Si el nodo actual no tiene hijos, es una hoja
    if (raiz->nodoIzquierdo == NULL && raiz->nodoDerecho == NULL) {
        return 1;
    }

    // Contar hojas en los subárboles izquierdo y derecho
    int hojasIzquierda = contarHojas(raiz->nodoIzquierdo);
    int hojasDerecha = contarHojas(raiz->nodoDerecho);

    return hojasIzquierda + hojasDerecha;
}



nodoArbol* espejo(nodoArbol* raiz) {
    if (raiz == NULL) {
        return NULL;
    }

    nodoArbol* espejoIzquierdo = espejo(raiz->nodoDerecho);
    nodoArbol* espejoDerecho = espejo(raiz->nodoIzquierdo);

    raiz->nodoIzquierdo = espejoDerecho;
    raiz->nodoDerecho = espejoIzquierdo;

    return raiz;
}

int sumaNodos(nodoArbol* raiz) {
    if (raiz == NULL) {
        return 0;
    }
	
	//se suma de manera separada
    int sumaIzquierda = sumaNodos(raiz->nodoIzquierdo);
    int sumaDerecha = sumaNodos(raiz->nodoDerecho);

	//se suma ambas partes
    return sumaIzquierda + sumaDerecha + 1; // 1 es el nodo raiz
}

nodoArbol* buscarValorMinimo(nodoArbol* raiz) {
    if (raiz == NULL) {
        return NULL;
    }

    nodoArbol* nodoMinimo = raiz;

    // Buscar el mínimo en el subárbol izquierdo
    nodoArbol* minIzquierdo = buscarValorMinimo(raiz->nodoIzquierdo);
    if (minIzquierdo != NULL && minIzquierdo->cantidadPersonas < nodoMinimo->cantidadPersonas) {
        nodoMinimo = minIzquierdo;
    }

    // Buscar el mínimo en el subárbol derecho
    nodoArbol* minDerecho = buscarValorMinimo(raiz->nodoDerecho);
    if (minDerecho != NULL && minDerecho->cantidadPersonas < nodoMinimo->cantidadPersonas) {
        nodoMinimo = minDerecho;
    }

	
    return nodoMinimo;
}

void eliminarNodo(nodoArbol*& raiz, int codigo) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->codigoLugar == codigo) {
        // Eliminar nodo hoja
        if (raiz->nodoIzquierdo == NULL && raiz->nodoDerecho == NULL) {
            raiz = NULL;
        } else {
            // Eliminar nodo con un hijo
            if (raiz->nodoIzquierdo == NULL) {
                raiz->nodoDerecho = NULL;
            } else if (raiz->nodoDerecho == NULL) {
                raiz->nodoIzquierdo = NULL;
            } else {
                // Eliminar nodo con dos hijos
                nodoArbol* aux = raiz->nodoIzquierdo;
                raiz->nodoIzquierdo = aux->nodoDerecho;
                aux->nodoDerecho = raiz->nodoDerecho;
                raiz->nodoDerecho = aux;
            }
        }
    } else {
        eliminarNodo(raiz->nodoIzquierdo, codigo);
        eliminarNodo(raiz->nodoDerecho, codigo);
    }
}


bool esArbolBusqueda(nodoArbol* raiz) {
    if (raiz == NULL) {
        return true; // El árbol vacío es un árbol de búsqueda válido
    }

    // Verificar que el nodo raíz cumpla la propiedad de árbol de búsqueda
    if (raiz->nodoIzquierdo != NULL && raiz->nodoDerecho != NULL) {
        if (raiz->nodoIzquierdo->cantidadPersonas > raiz->cantidadPersonas || raiz->nodoDerecho->cantidadPersonas < raiz->cantidadPersonas) {
            return false; // El nodo raíz no cumple la propiedad de árbol de búsqueda
        }
    }

    // Verificar los subárboles izquierdo y derecho
    if (!esArbolBusqueda(raiz->nodoIzquierdo) || !esArbolBusqueda(raiz->nodoDerecho)) {
        return false; // Alguno de los subárboles no es un árbol de búsqueda válido
    }

    return true; // El árbol es un árbol de búsqueda válido
}



nodoArbol* encontrarCamino(nodoArbol* raiz, int codigo) {
    if (raiz == NULL) {
        return NULL;
    }

    if (raiz->codigoLugar == codigo) {
        return raiz;
    }

    nodoArbol* aux = NULL;

    if (raiz->nodoIzquierdo != NULL) {
        aux = encontrarCamino(raiz->nodoIzquierdo, codigo);
    }

    if (aux == NULL && raiz->nodoDerecho != NULL) {
        aux = encontrarCamino(raiz->nodoDerecho, codigo);
    }

    return aux;
}

void imprimirArbol(nodoArbol* raiz) {
    if (raiz == NULL) {
    	
        return;
    }

    imprimirArbol(raiz->nodoIzquierdo);
    cout << raiz->nombreLugar << " (" << raiz->codigoLugar << ") con " << raiz->cantidadPersonas << " personas." << endl;
    imprimirArbol(raiz->nodoDerecho);
}

void verarbol(nodoArbol* raiz, int nro)// total de nodos del arbol
{
 int i;
 if(raiz==NULL)
 return;
 	verarbol(raiz->nodoDerecho,nro+1);
 	
	for(i=0;i<nro;i++)
    cout<<"    ";
    
	cout << raiz->nombreLugar << " (" << raiz->codigoLugar << ") con " << raiz->cantidadPersonas << " personas." << endl;
 
 	verarbol(raiz->nodoIzquierdo,nro+1);
}

void verarbolEspejo(nodoArbol* raiz, int nro)// total de nodos del arbol
{
 int i;
 if(raiz==NULL)
 return;
 	verarbolEspejo(raiz->nodoIzquierdo,nro+1);
 	
    for(i=0;i<nro;i++)
    cout<<"*****";
    
	cout << raiz->nombreLugar << " (" << raiz->codigoLugar << ") con " << raiz->cantidadPersonas << " personas." << endl;
 
 	verarbolEspejo(raiz->nodoDerecho,nro+1);
}


int main()
{
	

	
	nodoArbol* raiz = NULL;

	raiz = agregarNodo(raiz, "Taller", 123, 1500);
    raiz = agregarNodo(raiz, "Cafeteria", 321, 2000);
    raiz = agregarNodo(raiz, "Hospital", 213, 5000);
    raiz = agregarNodo(raiz, "Tienda", 1, 800);
    raiz = agregarNodo(raiz, "Casa", 2, 5);
    raiz = agregarNodo(raiz, "Tren", 6, 900);
	
	cout<< "Ver el arbol" <<endl;
	verarbol(raiz,6);
	system("pause");
	system("cls");
    
	cout<< "Se modifica el dato y se puede ver el cambio" <<endl;
	modificarDato(raiz, 123, 14000);

	imprimirArbol(raiz);
	system("pause");
	system("cls");
	
	
	cout<< "Cantidad de hojas en el arbol" <<endl;
	cout<< contarHojas(raiz) <<endl;
	system("pause");
	system("cls");
	
	
	cout<< "Arbol espejo" <<endl;
	verarbolEspejo(raiz,6);
	system("pause");
	system("cls");
	
	
	cout<< "Cantidad de Nodos" <<endl;
	cout<< sumaNodos(raiz) <<endl;
	system("pause");
	system("cls");
	
	
	cout<< "Valor minimo en cantidad de personas" <<endl;
	nodoArbol* nodoMinimo = buscarValorMinimo(raiz);
	if (nodoMinimo != NULL) {
    cout << "El valor mínimo es: " << nodoMinimo->cantidadPersonas << " en el lugar " << nodoMinimo->nombreLugar << " (Codigo: " << nodoMinimo->codigoLugar << ")" << endl;
	} else {
	cout << "El árbol está vacío." << endl;
	}
	system("pause");
	system("cls");
	
	
	cout<< "Se elimina un nodo, en este caso 'Tren'" <<endl;
	eliminarNodo(raiz,6);
	system("pause");
	verarbol(raiz,6);
	system("pause");
	system("cls");
	
	
	cout<< "Si es arbol de busqueda" <<endl;
	cout<< esArbolBusqueda(raiz) <<endl;
	system("pause");
	system("cls");
	
	
	cout<< "Encontrar camino, PD este la verdad no lo entendi muy bien" <<endl;
	cout<< encontrarCamino(raiz,213) <<endl;
	system("pause");
	system("cls");
	
	
	return 0;
}
