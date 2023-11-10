#include <stdio.h>
#include <stdlib.h>

struct nodo {
  struct nodo* sig;
  struct nodo* prev;
  int valor;
};

struct lista_doble {
  struct nodo* cabeza;
};

struct tablero {
  struct lista_doble* col0;
  struct lista_doble* col1;
  struct lista_doble* col2;
  int altura;
};

void insertar(struct lista_doble* lista, int valor) {
  struct nodo* nuevo_nodo = (struct nodo*)calloc(1, sizeof(struct nodo));
  nuevo_nodo->valor = valor;

  if (lista == NULL) {
    printf("La lista no existe\n");
  } else {
    if (lista->cabeza == NULL) {
      lista->cabeza = nuevo_nodo;
    } else {
      struct nodo* nodo = lista->cabeza;
      while (nodo->sig != NULL) {
        nodo = nodo->sig;
      }
      nodo->sig = nuevo_nodo;
      nuevo_nodo->prev = nodo;
    }
  }
}

struct lista_doble* crear_lista(int altura) {
  struct lista_doble* lista = (struct lista_doble*)calloc(1, sizeof(struct lista_doble));

  for (int i = 0; i < altura; i++) {
    int valor = 0;
    insertar(lista, valor);
  }

  return lista;
}

struct tablero* crear_tablero() {
  int altura;
  printf("Ingrese la altura del tablero: ");
  scanf("%d", &altura);

  struct tablero* nuevo_tablero = (struct tablero*)calloc(1, sizeof(struct tablero));

  nuevo_tablero->col0 = crear_lista(altura);
  nuevo_tablero->col1 = crear_lista(altura);
  nuevo_tablero->col2 = crear_lista(altura);

  nuevo_tablero->altura = altura;

  return nuevo_tablero;
}

void mostrar_tablero(const struct tablero* mi_tablero) {
  struct lista_doble* columnas[3] = {mi_tablero->col0, mi_tablero->col1, mi_tablero->col2};

  // Arreglo de nodos para recorrer las 3 columnas del tablero.
  struct nodo* nodos_actuales[3] = {columnas[0]->cabeza->sig, columnas[1]->cabeza->sig, columnas[2]->cabeza->sig};

  // Variables para dar formato a la piramide
  int sangria = mi_tablero->altura;
  int separacion = 1;

  // Imprime espacios antes del nodo inicial("Sangria").
  for (int i = 0; i < sangria; i++) {
    printf(" ");
  }
  sangria-=2;

  //Imprime el primer valor(la punta del triangulo).
  printf("%d", columnas[0]->cabeza->valor);
  printf("\n");

  while (nodos_actuales[0] != NULL) {
    // Imprime espacios al principio de cada línea("Sangria").
    for (int i = 0; i < sangria; i++) {
      printf(" ");
    }

    // Imprime los valores de los nodos.
    for (int i = 0; i < 3; i++) {
      printf("%d", nodos_actuales[i]->valor);

      // Imprime la separacion entre los valores
      for (int i = 0; i < separacion; i++) {
        printf(" ");
      }
    }

    printf("\n");

    // Avanza a los siguientes nodos.
    for (int i = 0; i < 3; i++) {
      nodos_actuales[i] = nodos_actuales[i]->sig;
    }
    sangria--;
    separacion++;
  }
}



int main() {
  struct tablero* mi_tablero = crear_tablero();
  
  mostrar_tablero(mi_tablero);

  return 0;
}