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

  // Variables para dar formato a la pirámide.
  int sangria = mi_tablero->altura;
  int separacion = 1;

  // Imprime espacios antes del nodo inicial("Sangria").
  for (int i = 0; i < sangria; i++) {
    printf(" ");
  }
  sangria -= 2;

  // Imprime el primer valor(la punta del triángulo).
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

      // Imprime la separación entre los valores.
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

void colocar_ficha(struct tablero* mi_tablero, int jugador) {
  int columna, fila;
  printf("Jugador %d, ingrese la columna (0, 1, 2): ", jugador);
  scanf("%d", &columna);

  if (columna >= 0 && columna <= 2) {
    printf("Jugador %d, ingrese la fila (0 a %d): ", jugador, mi_tablero->altura - 1);
    scanf("%d", &fila);

    if (fila >= 0 && fila < mi_tablero->altura) {
      struct lista_doble* columna_seleccionada;
      switch (columna) {
        case 0:
          columna_seleccionada = mi_tablero->col0;
          break;
        case 1:
          columna_seleccionada = mi_tablero->col1;
          break;
        case 2:
          columna_seleccionada = mi_tablero->col2;
          break;
        default:
          printf("Columna no válida\n");
          return;
      }

      // Encuentra el nodo en la posición indicada y coloca la ficha del jugador.
      struct nodo* nodo = columna_seleccionada->cabeza->sig;
      for (int i = 0; i < fila; i++) {
        if (nodo != NULL) {
          nodo = nodo->sig;
        } else {
          printf("Fila no válida\n");
          return;
        }
      }

      nodo->valor = jugador;
    } else {
      printf("Fila no válida\n");
    }
  } else {
    printf("Columna no válida\n");
  }
}

int main() {
  struct tablero* mi_tablero = crear_tablero();
  int jugador_actual = 1;

  mostrar_tablero(mi_tablero);

  for (int turno = 0; turno < mi_tablero->altura * 3; turno++) {
    colocar_ficha(mi_tablero, jugador_actual);

    mostrar_tablero(mi_tablero);

    // Cambia al siguiente jugador
    jugador_actual = (jugador_actual == 1) ? 2 : 1;
  }

  return 0;
}