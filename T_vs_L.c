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

  if (fila < 0 || fila >= mi_tablero->altura) {
    printf("Fila no valida\n");
    return 0;
  }

  // Si es en la punta de la piramide coloca el valor en las 3 cabezas
  if(fila == 0) {
    if(mi_tablero->col0->cabeza->valor == 0) {
      mi_tablero->col0->cabeza->valor = jugador;
      mi_tablero->col1->cabeza->valor = jugador;
      mi_tablero->col2->cabeza->valor = jugador;
      return 1;
    } else {
      printf("La posicion ya esta ocupada\n");
      return 0;
    }
  }

  printf("Jugador %d, ingrese la columna (0 a 2): ", jugador);
  scanf("%d", &columna);

  if (columna < 0 || columna > 2) {
    printf("Columna no valida\n");
    return 0;
  }

  struct lista_doble* columnas[3] = {mi_tablero->col0, mi_tablero->col1, mi_tablero->col2};
  struct nodo* nodo = columnas[columna]->cabeza;

  for (int i = 0; i < fila; i++) {
    if (nodo != NULL) {
      nodo = nodo->sig;
    } else {
      printf("Fila no valida\n");
      return 0;
    }
  }

  if (nodo->valor != 0) {
    printf("La posicion ya esta ocupada\n");
    return 0;
  }

  nodo->valor = jugador;
  return 1;
}

int mover_ficha(struct tablero* mi_tablero, int jugador) {
  int fila_origen, columna_origen, fila_destino, columna_destino;

  printf("Jugador %d, ingrese la fila de la ficha a mover (0 a %d): ", jugador, mi_tablero->altura - 1);
  scanf("%d", &fila_origen);

  if (fila_origen < 0 || fila_origen >= mi_tablero->altura) {
    printf("Fila no válida\n");
    return 0;
  }

  if(fila_origen != 0) {
    printf("Jugador %d, ingrese la columna de la ficha a mover (0, 1, 2): ", jugador);
    scanf("%d", &columna_origen);
  } else {
    columna_origen = 0;
  }

  if (columna_origen < 0 || columna_origen > 2) {
    printf("Columna no válida\n");
    return 0;
  }

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
      printf("Columna no valida\n");
      return 0;
  }

  // Encuentra el nodo en la posición indicada.
  struct nodo* nodo_origen = columna_origen_seleccionada->cabeza;
  for (int i = 0; i < fila_origen; i++) {
    if (nodo_origen != NULL) {
      nodo_origen = nodo_origen->sig;
    } else {
      printf("Fila no valida\n");
      return 0;
    }
  }

  if (nodo_origen->valor != jugador) {
    printf("No hay una ficha del jugador %d en la posicion de origen\n", jugador);
    return 0;
  }

  printf("Jugador %d, ingrese la fila de destino (0 a %d): ", jugador, mi_tablero->altura - 1);
  scanf("%d", &fila_destino);

  if (fila_destino < 0 || fila_destino >= mi_tablero->altura) {
    printf("Fila de destino no válida\n");
    return 0;
  }

  if(fila_destino != 0) {
    printf("Jugador %d, ingrese la columna de destino (0, 1, 2): ", jugador);
    scanf("%d", &columna_destino);
  } else {
    columna_destino = 0;
  }

  if (columna_destino < 0 || columna_destino > 2) {
    printf("Columna de destino no válida\n");
    return 0;
  }

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
      printf("Columna de destino no válida\n");
      return 0;
  }

  // Encuentra el nodo en la posición de destino.
  struct nodo* nodo_destino = columna_destino_seleccionada->cabeza;
  for (int i = 0; i < fila_destino; i++) {
    if (nodo_destino != NULL) {
      nodo_destino = nodo_destino->sig;
    } else {
      printf("Fila no valida\n");
      return 0;
    }
  }

  // Verifica si la posición de destino está ocupada.
  if (nodo_destino->valor == 1 || nodo_destino->valor == 2) {
    printf("No puedes mover la ficha sobre otra ficha.\n");
    return 0;
  }

  // Realiza el movimiento normal de forma vertical cuando el origen es la punta.
  if(fila_origen == 0 && abs(fila_origen - fila_destino) == 1) {
    nodo_destino->valor = jugador;

    mi_tablero->col0->cabeza->valor = 0;
    mi_tablero->col1->cabeza->valor = 0;
    mi_tablero->col2->cabeza->valor = 0;

    return 1;
  }

  // Realiza el movimiento normal de forma vertical cuando el destino es la punta.
  if(fila_destino == 0 && abs(fila_origen - fila_destino) == 1) {
    nodo_origen->valor = 0;

    mi_tablero->col0->cabeza->valor = jugador;
    mi_tablero->col1->cabeza->valor = jugador;
    mi_tablero->col2->cabeza->valor = jugador;

    return 1;
  }

  // Realiza el movimiento normal de forma vertical.
  if((abs(fila_origen - fila_destino) == 1) && (columna_origen - columna_destino == 0)) {
  // Si la fila destino es la punta de la piramide copia el valor en las 3 columnas 
    if(fila_destino == 0) {
      mi_tablero->col0->cabeza->valor = jugador;
      mi_tablero->col1->cabeza->valor = jugador;
      mi_tablero->col2->cabeza->valor = jugador;
    } else {
      nodo_destino->valor = jugador;
      // Si la fila origen es la punta de la piramide deja el valor en 0 en las 3 columnas 
      if(fila_origen == 0) {
        mi_tablero->col0->cabeza->valor = 0;
        mi_tablero->col1->cabeza->valor = 0;
        mi_tablero->col2->cabeza->valor = 0;
      } else {
        nodo_origen->valor = 0;
      }
    }
    return 1;
  }

  // Realiza el movimiento normal de forma horizontal.
  else if((abs(columna_origen - columna_destino) == 1) && (fila_origen - fila_destino == 0)) {
    nodo_destino->valor = jugador;
    nodo_origen->valor = 0;
    return 1;
  }

  // Realiza el movimiento para comer.
  if(jugador == 1) {
    //Comer hacia arriba
    if(fila_origen - fila_destino == 2 && getValor(mi_tablero, fila_destino+1, columna_origen) == 2) {
      setZeroNodo(mi_tablero, fila_destino+1, columna_origen);//Elimina la ficha comida

      //Si el destino era la punta del triangulo cambia las 3 cabezas
      if(fila_destino == 0) {
        mi_tablero->col0->cabeza->valor = jugador;
        mi_tablero->col1->cabeza->valor = jugador;
        mi_tablero->col2->cabeza->valor = jugador;

        nodo_origen->valor = 0;
      } else {
        nodo_destino->valor = jugador;
        // Si la fila origen es la punta de la piramide deja el valor en 0 en las 3 columnas 
        if(fila_origen == 0) {
          mi_tablero->col0->cabeza->valor = 0;
          mi_tablero->col1->cabeza->valor = 0;
          mi_tablero->col2->cabeza->valor = 0;
        } else {
          nodo_origen->valor = 0;
        }
      }
      return 1;
    }

    //Comer hacia abajo
    else if(fila_origen - fila_destino == -2 && getValor(mi_tablero, fila_destino-1, columna_destino) == 2) {
      nodo_destino->prev->valor = 0;//Elimina la ficha comida

      //Si el destino era la punta del triangulo cambia las 3 cabezas
      if(fila_destino == 0) {
        mi_tablero->col0->cabeza->valor = jugador;
        mi_tablero->col1->cabeza->valor = jugador;
        mi_tablero->col2->cabeza->valor = jugador;
      } else {
        nodo_destino->valor = jugador;
        // Si la fila origen es la punta de la piramide deja el valor en 0 en las 3 columnas 
        if(fila_origen == 0) {
          mi_tablero->col0->cabeza->valor = 0;
          mi_tablero->col1->cabeza->valor = 0;
          mi_tablero->col2->cabeza->valor = 0;
        } else {
          nodo_origen->valor = 0;
        }
      }
      return 1;
    }

    //Comer hacia la derecha
    else if(columna_origen - columna_destino == -2 && getValor(mi_tablero, fila_destino, columna_destino-1) == 2) {

      // Elimina la ficha comida
      // Encuentra el nodo a la izquierda de la posición de destino y lo coloca con valor 0.
      setZeroNodo(mi_tablero, fila_destino, columna_destino-1);

      nodo_destino->valor = jugador;
      nodo_origen->valor = 0;
      return 1;
    }

    //Comer hacia la izquierda
    else if(columna_origen - columna_destino == 2 && getValor(mi_tablero, fila_destino, columna_destino+1) == 2) {

      // Elimina la ficha comida
      // Encuentra el nodo a la derecha de la posición de destino y lo coloca con valor 0.
      setZeroNodo(mi_tablero, fila_destino, columna_destino-1);

      nodo_destino->valor = jugador;
      nodo_origen->valor = 0;
      return 1;
    }
  }

  printf("Movimiento invalido.\n");
  return 0;
}

//Retorna el valor que hay en la fila y columna o un 99 en caso de estar fuera de rango
int getValor(struct tablero* mi_tablero, int fila, int columna) {
  if (fila < 0 || fila >= mi_tablero->altura || columna < 0 || columna > 2) {
    return 99;  // Fuera de rango
  }

  struct lista_doble* columnas[3] = {mi_tablero->col0, mi_tablero->col1, mi_tablero->col2};
  struct nodo* nodo = columnas[columna]->cabeza;

  for (int i = 0; i < fila; i++) {
    if (nodo != NULL) {
      nodo = nodo->sig;
    } else {
      return 99;  // Fuera de rango
    }
  }

  return nodo->valor;
}

//Retorna el valor que hay en la fila y columna o un 99 en caso de estar fuera de rango
void setZeroNodo(struct tablero* mi_tablero, int fila, int columna) {
  if (fila < 0 || fila >= mi_tablero->altura || columna < 0 || columna > 2) {
    return;  // Fuera de rango
  }

  struct lista_doble* columnas[3] = {mi_tablero->col0, mi_tablero->col1, mi_tablero->col2};
  struct nodo* nodo = columnas[columna]->cabeza;

  for (int i = 0; i < fila; i++) {
    if (nodo != NULL) {
      nodo = nodo->sig;
    } else {
      return;  // Fuera de rango
    }
  }

  nodo->valor = 0;
}

int ganador(struct tablero* mi_tablero, int jugador) {
  if(jugador == 1) {
    //Arreglo con las columnas del tablero para acceder mas facilmente
    struct lista_doble* columnas[3] = {mi_tablero->col0, mi_tablero->col1, mi_tablero->col2};

    // Arreglo de nodos para recorrer las 3 columnas del tablero.
    struct nodo* nodos_actuales[3] = {columnas[0]->cabeza->sig, columnas[1]->cabeza->sig, columnas[2]->cabeza->sig};

    //Paso 1. Encontrar donde esta la ficha del jugador 1(el cazador).
    int fila = 0;
    int columna = 0;

    //Recorrido
    for (int j = 0; nodos_actuales[0] != NULL; j++) {
      for (int i = 0; i < 3; i++) {
        if(nodos_actuales[i]->valor == 1) {
          columna = i;
          fila = j+1;
        }
      }

      // Avanza a los siguientes nodos.
      for (int i = 0; i < 3; i++) {
        nodos_actuales[i] = nodos_actuales[i]->sig;
      }
    }//Para este punto ya se encontro la fila y columna

    //Paso 2. Verificar si el cazador esta encerrado
    if(
      getValor(mi_tablero, fila-1, columna) > 0 &&
      getValor(mi_tablero, fila+1, columna) > 0 &&
      getValor(mi_tablero, fila, columna+1) > 0 &&
      getValor(mi_tablero, fila, columna-1) > 0 &&
      getValor(mi_tablero, fila-2, columna) > 0 &&
      getValor(mi_tablero, fila+2, columna) > 0 &&
      getValor(mi_tablero, fila, columna+2) > 0 &&
      getValor(mi_tablero, fila, columna-2) > 0
    ) {
      return 1;
    }
    return 0;
  } else {
    return 0;
  }
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

  printf("Felicidades al ganador!!!");

  return 0;
}

     

