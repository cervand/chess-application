/* Square.c structure module */
#include <stdio.h>
#include "Square.h"
#include <stdlib.h>
SQUARE * CreateSquare() {
    SQUARE * temp = malloc(sizeof(SQUARE));
    temp->piece_ptr = NULL;
    temp->x = 0;
    temp->y = 0;
    return temp;
}

int DeleteSquare(SQUARE * square) {
    if (square) {
        square->piece_ptr = NULL;
        square->x = 0;
        square->y = 0;
        free(square);
        square = NULL;
        return 0;
    } else {
        return 1;
    }
}
