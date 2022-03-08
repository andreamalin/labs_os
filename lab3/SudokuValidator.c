#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int sudoku[9][9] = {
        {1, 2, 3, 4, 6, 9, 7, 5, 8},
        {8, 4, 6, 5, 7, 1, 2, 9, 3},
        {9, 5, 7, 2, 8, 3, 6, 4, 1},
        {3, 1, 2, 7, 5, 8, 4, 6, 9},
        {4, 6, 8, 3, 9, 2, 1, 7, 5},
        {5, 7, 9, 1, 4, 6, 8, 3, 2},
        {2, 3, 4, 6, 1, 5, 9, 8, 7},
        {6, 8, 1, 9, 3, 7, 5, 2, 4},
        {7, 9, 5, 8, 2, 4, 3, 1, 6}
    };

    checkGrid(sudoku);
    checkColumn(sudoku);
    checkRow(sudoku);
    return 0;
}

// Columna
int checkColumn(int sudoku[9][9]) {
    int cells[9];

    for (int i = 0; i < 9; i++)
    {
        // Se obtiene una columna
        for (int j = 0; j < 9; j++)
        {
            cells[j] = sudoku[j][i];
        }
     
        int checkNumbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        // Por celda en la columna
        for (int cell = 0; cell < 9; cell++)
        {
            // Por cada numero en los numeros
            for (int i = 0; i < 9; i++)
            {
                // Si el numero es igual al de la celda
                if (checkNumbers[i] == cells[cell]) {
                    // Colorcarlo en -1 (ya usado)
                    checkNumbers[i] = -1;
                    break;
                }
            }
        }

        // Por celda en la columna
        for (int cell = 0; cell < 9; cell++)
        {
            if (checkNumbers[cell] != -1) {
                printf("Alguna columna NO contiene todos los numeros 1-9\n");
                // La columna NO contiene todos los numeros del 1 al 9 
                return 0;
            }
        }
        
    }

    printf("Todas las columnas contienen los numeros 1-9\n");
    // Todas las columnas contienen todos los numeros del 1 al 9 
    return 1; // True
}


// Fila
int checkRow(int sudoku[9][9]) {
    int cells[9];

    for (int i = 0; i < 9; i++)
    {
        // Se obtiene una fila
        for (int j = 0; j < 9; j++)
        {
            cells[j] = sudoku[i][j];
        }

        int checkNumbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        // Por celda en la fila
        for (int cell = 0; cell < 9; cell++)
        {
            // Por cada numero en los numeros
            for (int i = 0; i < 9; i++)
            {
                // Si el numero es igual al de la celda
                if (checkNumbers[i] == cells[cell]) {
                    // Colorcarlo en -1 (ya usado)
                    checkNumbers[i] = -1;
                    break;
                }
            }
        }

        // Por celda en la fila
        for (int cell = 0; cell < 9; cell++)
        {
            if (checkNumbers[cell] != -1) {
                printf("Alguna fila NO contiene todos los numeros 1-9\n");
                // La fila NO contiene todos los numeros del 1 al 9 
                return 0;
            }
        }
        
    }

    printf("Todas las filas contienen los numeros 1-9\n");
    // Todas las filas contienen todos los numeros del 1 al 9 
    return 1; // True
}


// Cuadricula 3x3
int checkGrid(int sudoku[9][9]) {
    int cells[9];

    // Nos vamos moviendo por filas
    for (int contadorFilas = 0; contadorFilas < 9; contadorFilas+=3)
    {
        // Nos vamos moviendo por columnas
        for (int contadorColumna = 0; contadorColumna < 9; contadorColumna+=3)
        {
            // Resetear el contador
            int contador = 0;
            
            // Se obtiene una fila
            for (int i = 0; i < 3; i++)
            {
                // Se obtiene una columna
                for (int j = 0; j < 3; j++)
                {
                    // Se suman los contadores para el grid correcto
                    cells[contador] = sudoku[i+contadorFilas][j+contadorColumna];
                    contador++;
                }
            }

            // Todas las posiciones de las celdas
            for (int i = 0; i < 9; i++) {
                int checkNumbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                // Por celda en la cuadricula
                for (int cell = 0; cell < 9; cell++)
                {
                    // Por cada numero en los numeros
                    for (int i = 0; i < 9; i++)
                    {
                        // Si el numero es igual al de la celda
                        if (checkNumbers[i] == cells[cell]) {
                            // Colorcarlo en -1 (ya usado)
                            checkNumbers[i] = -1;
                            break;
                        }
                    }
                }

                // Por celda en la cuadricula
                for (int cell = 0; cell < 9; cell++)
                {
                    if (checkNumbers[cell] != -1) {
                        printf("Alguna cuadricula NO contiene todos los numeros 1-9\n");
                        // La cuadricula NO contiene todos los numeros del 1 al 9 
                        return 0;
                    }
                }   
            }
        }
    }
   
    printf("Todas las cuadriculas contienen los numeros 1-9\n");
    // Todas las cuadriculas contienen todos los numeros del 1 al 9 
    return 1; // True
}