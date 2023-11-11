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

int colocar_ficha(struct tablero* mi_tablero, int jugador) {
  int fila, columna;

  printf("Jugador %d, ingrese la fila (0 a %d): ", jugador, mi_tablero->altura - 1);
  scanf("%d", &fila);

  //Si es en la punta de la piramide coloca el valor en las 3 cabezas
  if(fila == 0) {
    mi_tablero->col0->cabeza->valor = jugador;
    mi_tablero->col1->cabeza->valor = jugador;
    mi_tablero->col2->cabeza->valor = jugador;

    return 1;
  }

  if (fila < mi_tablero->altura && fila >= 0) {
    printf("Jugador %d, ingrese la columna (0 a 2): ", jugador);
    scanf("%d", &columna);

    if (fila > 0) {
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
          printf("Columna no valida\n");
          return 0;
      }

      // Encuentra el nodo en la posición indicada y coloca la ficha del jugador.
      struct nodo* nodo = columna_seleccionada->cabeza;
      for (int i = 0; i < fila; i++) {
        if (nodo != NULL) {
          nodo = nodo->sig;
        } else {
          printf("Fila no valida\n");
          return 0;
        }
      }
      
      nodo->valor = jugador;
      return 1;
    } else {
      printf("Fila no valida\n");
      return 0;
    }
  } else {
    printf("Fila no valida\n");
    return 0;
  }
}

int mover_ficha(struct tablero* mi_tablero, int jugador) {
  int columna_origen, fila_origen, columna_destino, fila_destino;
  printf("Jugador %d, ingrese la columna de la ficha a mover (0, 1, 2): ", jugador);
  scanf("%d", &columna_origen);

  if (columna_origen >= 0 && columna_origen <= 2) {
    printf("Jugador %d, ingrese la fila de la ficha a mover (0 a %d): ", jugador, mi_tablero->altura - 1);
    scanf("%d", &fila_origen);

    if (fila_origen >= 0 && fila_origen < mi_tablero->altura) {
      struct lista_doble* columna_origen_seleccionada;
      switch (columna_origen) {
        case 0:
          columna_origen_seleccionada = mi_tablero->col0;
          break;
        case 1:
          columna_origen_seleccionada = mi_tablero->col1;
          break;
        case 2:
          columna_origen_seleccionada = mi_tablero->col2;
          break;
        default:
          printf("Columna no válida\n");
          return 0;
      }
      
      // Encuentra el nodo en la posición indicada.
      struct nodo* nodo_origen = columna_origen_seleccionada->cabeza;
      for (int i = 0; i < fila_origen; i++) {
        if (nodo_origen != NULL) {
          nodo_origen = nodo_origen->sig;
        } else {
          printf("Fila no válida\n");
          return 0;
        }
      }

      if (nodo_origen->valor == jugador) {
        printf("Jugador %d, ingrese la columna de destino (0, 1, 2): ", jugador);
        scanf("%d", &columna_destino);

        if (columna_destino >= 0 && columna_destino <= 2) {
          printf("Jugador %d, ingrese la fila de destino (0 a %d): ", jugador, mi_tablero->altura - 1);
          scanf("%d", &fila_destino);

          if (fila_destino >= 0 && fila_destino < mi_tablero->altura) {
            struct lista_doble* columna_destino_seleccionada;
            switch (columna_destino) {
              case 0:
                columna_destino_seleccionada = mi_tablero->col0;
                break;
              case 1:
                columna_destino_seleccionada = mi_tablero->col1;
                break;
              case 2:
                columna_destino_seleccionada = mi_tablero->col2;
                break;
              default:
                printf("Columna no válida\n");
                return 0;
            }

            // Encuentra el nodo en la posición de destino.
            struct nodo* nodo_destino = columna_destino_seleccionada->cabeza;
            for (int i = 0; i < fila_destino; i++) {
              if (nodo_destino != NULL) {
                nodo_destino = nodo_destino->sig;
              } else {
                printf("Fila no válida\n");
                return 0;
              }
            }

            // Realiza el movimiento de la ficha.
            nodo_destino->valor = jugador;
            nodo_origen->valor = 0;
          } else {
            printf("Fila de destino no válida\n");
            return 0;
          }
        } else {
          printf("Columna de destino no válida\n");
          return 0;
        }
      } else {
        printf("No hay una ficha del jugador %d en la posición de origen\n", jugador);
        return 0;
      }
    } else {
      printf("Fila de origen no válida\n");
      return 0;
    }
  } else {
    printf("Columna de origen no válida\n");
    return 0;
  }
}

int ganador(struct tablero* mi_tablero, int jugador) {
  return 0;
}

int main() {
  struct tablero* mi_tablero = crear_tablero();
  int jugador1 = 1;
  int jugador2 = 2;
  int jugador_actual = jugador1; // Empieza el juego con el jugador1

  mostrar_tablero(mi_tablero);

  int turno = 1;
  while(!ganador(mi_tablero, jugador_actual)){

    //Verifica si todavia se tienen que colocar fichas
    if((jugador_actual == 1 && turno== 1)||(jugador_actual == 2 && turno<= 12)) {
      //Si se logra colocar la ficha hace el cambio de turno y de jugador, sino se intenta de nuevo
      if(colocar_ficha(mi_tablero, jugador_actual)) {
        if (jugador_actual == jugador1) {
          jugador_actual = jugador2;
        } else {
          jugador_actual = jugador1;
        }
        turno++;
      }
    } else {
      if(mover_ficha(mi_tablero, jugador_actual)) {
        if (jugador_actual == jugador1) {
            jugador_actual = jugador2;
          } else {
            jugador_actual = jugador1;
          }
          turno++;
      }
    }
    mostrar_tablero(mi_tablero);
  }

  return 0;
}

     

