//SplayTree
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;

struct Nodo{
    int llave;
    char* llaveChar;
    char nombre[20];
    struct Nodo *nodo_izquierdo;
    struct Nodo *nodo_derecho;
};

struct Nodo* nuevoNodo(int llave_insertar,char const nombre_plataforma[20], char* llave_char){
	struct Nodo* nodo = (struct Nodo*)malloc(sizeof(struct Nodo));
   	nodo->llave= llave_insertar;
   	nodo->llaveChar = llave_char;
   	strcpy(nodo->nombre,nombre_plataforma);
    nodo->nodo_izquierdo= NULL;
	nodo->nodo_derecho= NULL;
	return nodo;
}


/* Funcion para rotar a la derecha el subarbol de manera que "y" quede en la raiz que recibe un nodo x como parametro
Retorna un puntero al nodo y
No tiene restricciones*/
struct Nodo *rotacion_derecha(struct Nodo *x){
    struct Nodo *y = x->nodo_izquierdo;
    x->nodo_izquierdo = y->nodo_derecho;
    y->nodo_derecho = x;
    return y;
}

/* Funcion para rotar a la izquierda el subarbol de manera que "y" quede en la raiz que recibe un nodo x como parametro
No tiene restricciones
retorna un puntero al nodo y
*/
struct Nodo *rotacion_izquierda(struct Nodo *x){
    struct Nodo *y = x->nodo_derecho;
    x->nodo_derecho = y->nodo_izquierdo;
    y->nodo_izquierdo= x;
    return y;
}

/* Biscela el arbol splay recibe la raiz y la llave del nodo a insertar
Salidas: retorna la raiz; 
*/
struct Nodo *splay(struct Nodo *raiz, int llave_insertar){
	// Si la raiz es nula o es la llave, la retorna
    if (raiz==NULL || raiz->llave == llave_insertar){
        return raiz;
      }
    if (llave_insertar < raiz->llave){
    	// Si la raiz no tiene subarbol izquierdo, la llave no esta en el arbol
        if (raiz->nodo_izquierdo==NULL)
            return raiz;
        // Zig-Zig
        // Si la llave a insertar es mayor a la llave que se encuentra en el subarbol izquierdo de la raiz
        if (llave_insertar < raiz->nodo_izquierdo->llave){
            raiz->nodo_izquierdo->nodo_izquierdo= splay(raiz->nodo_izquierdo->nodo_izquierdo,llave_insertar);
            raiz =rotacion_derecha(raiz);
        }
        // Si la llave a insertar es mayor a la llave que se encuentra en el subarbol derecho de la raiz
        // Zig-Zag
        else if (llave_insertar > raiz->nodo_izquierdo->llave){
            raiz->nodo_izquierdo->nodo_derecho= splay(raiz->nodo_izquierdo->nodo_derecho,llave_insertar);
            if (raiz->nodo_izquierdo->nodo_derecho!= NULL)
                raiz->nodo_izquierdo = rotacion_izquierda(raiz->nodo_izquierdo);
        }
        if(raiz->nodo_izquierdo== NULL){
        	return raiz;
		}
		else{
			return rotacion_derecha(raiz);
		}
	}
	// La llave se encuentra en el subarbol derecho
    else{
        if (raiz->nodo_derecho== NULL){
            return raiz;
        }
        if (llave_insertar < raiz->nodo_derecho->llave){
            raiz->nodo_derecho->nodo_izquierdo= splay(raiz->nodo_derecho->nodo_izquierdo,llave_insertar);
            if (raiz->nodo_derecho->nodo_izquierdo!= NULL)
                raiz->nodo_derecho = rotacion_derecha(raiz->nodo_derecho);
        }
        //Zag-zag
        else if (llave_insertar > raiz->nodo_derecho->llave){
            raiz->nodo_derecho->nodo_derecho= splay(raiz->nodo_derecho->nodo_derecho,llave_insertar);
            raiz = rotacion_izquierda(raiz);
        }
        if(raiz->nodo_derecho==NULL){
        	return raiz;
		} 
		else{
			return rotacion_izquierda(raiz);
		} 
    }
}

/* Funcion para insertar un nodo a un arbol splay, entrandole como parametro una raiz, una llave,una plataforma, un usuario, una contrasena y una bandera
retorna un nodo
No tiene restricciones*/
struct Nodo *insertar(struct Nodo *raiz, int llave_insertar,char* llave_char,char const nombre[20]){
	//char* llave_Char= (char*)llave_insertar;
	// Si el arbol no existe
    if (raiz== NULL){
        return nuevoNodo(llave_insertar,nombre,llave_char);
    }
    // Trae la hoja mas cercana a la raiz
    raiz= splay(raiz, llave_insertar);
    // Si la llave ya existe, retorna
    if (raiz->llave== llave_insertar)
          return raiz;
    // Si no, creamos memoria para un nuevo nodo
    struct Nodo *nuevo_Nodo=nuevoNodo(llave_insertar,nombre,llave_char);
    // Si la llave a insertar es menor que la llave de la raiz, se hace la raiz el hijo derecho
    // del nuevo nodo y se copia el hijo izquierdo de la raiz al nuevo nodo
    if (raiz->llave > llave_insertar){
        nuevo_Nodo->nodo_derecho = raiz;
        nuevo_Nodo->nodo_izquierdo = raiz->nodo_izquierdo;
        raiz->nodo_izquierdo= NULL;
    }
    // Si la llave a insertar es mayor que la llave de la raiz, el hijo izquierdo se hace la raiz
    // del nuevo nodo y se copia el hijo derecho de la raiz al nuevo nodo
    else{
        nuevo_Nodo->nodo_izquierdo = raiz;
        nuevo_Nodo->nodo_derecho = raiz->nodo_derecho;
        raiz->nodo_derecho= NULL;
    }
    // El nuevo nodo se convierte en la raiz
    return nuevo_Nodo;
}


/*Funcion para buscar en un arbol splay que recibe un Nodo raiz y una llave
Salidas: retorna un llamado a splay
No tiene restricciones  
*/
struct Nodo *buscar(struct Nodo *raiz, int llave_buscar){ 
    return splay(raiz,llave_buscar); 
} 

/* Funcion para eliminar
Recibe la raiz y la llave a borrar
Retorna la nueva raiz luego de borrar la llave 
*/
struct Nodo* eliminar_nodo(struct Nodo *raiz, int llave_borrar){
    struct Nodo *temp;
    if (!raiz){
      return NULL;
    }
    // se le hacen rotaciones Splay al nodo con la llave indicada para traerla a la raiz
    raiz= splay(raiz, llave_borrar);
    // Si la llave no se encuentra se retorna la raiz
    if (llave_borrar!= raiz->llave){
    	cout<<"Este numero de identificador no es valido\n"<<endl;
        return raiz;
    }
    // Si la llave si se encuentra
    else{
    	// Si la raiz no tiene subarbol izquierdo
    	// el nodo derecho de la raiz se hace la nueva raiz
    	if(raiz->nodo_izquierdo==NULL){  //si falla es por esto xd, lo que habia antes era if (!raiz->nodo_izquierdo)
        	temp= raiz;
        	raiz= raiz->nodo_derecho;
    	}
    	// Si la raiz si tiene subarbol izquierdo
    	else{
        	temp = raiz;
        	// Hago splay con el subarbol izquierdo y la llave, para que me traiga el dato más cercano a la llave
    	    // El nodo mayor del subarbol izquierdo a ser la raiz
        	raiz= splay(raiz->nodo_izquierdo,llave_borrar);
        	// Se añade a la raiz nueva el subarbol derecho de la raiz anterior
        	raiz->nodo_derecho= temp->nodo_derecho;
    	}
    	// Se libera la antigua raiz, que es el nodo que contiene la llave
    	cout<<"Se ha borrado la llave \n"<<llave_borrar<<endl;
    	free(temp);
    	// Se retorna la nueva raiz del arbol splay
    	return raiz;
	}
}
