#include "operaciones.h"
#include "Validaciones.h"


void ingresarDatos(char nombres[][MAX_NOMBRE], float precios[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Ingresando datos del Libro #%d ---\n", i + 1);
        //Implementamos funcion que valida el nombre de un producto.
        validar_strings(nombres[i], MAX_NOMBRE, "Ingrese el nombre de su producto:");
        printf("Ingrese el precio del producto %i: \n", i + 1);
        scanf("%f", &precios[i]);
        while (getchar() != '\n');   // Sentencia para limpiar salto de linea despues de scanf            

    }
}

float calcularTotal(float precios[], int n) {
    float total = 0;
    for (int i = 0; i < n; i++)
        total += precios[i];
    return total;
}

float calcularPromedio(float precios[], int n) {
    return calcularTotal(precios, n) / n;
}

int indiceMasCaro(float precios[], int n) {
    int indice = 0;
    for (int i = 1; i < n; i++)
        if (precios[i] > precios[indice])
            indice = i;
    return indice;
}

int indiceMasBarato(float precios[], int n) {
    int indice = 0;
    for (int i = 1; i < n; i++)
        if (precios[i] < precios[indice])
            indice = i;
    return indice;
}

int buscarProducto(char nombres[][MAX_NOMBRE], float precios[], int n, char nombreBuscado[]) {
    for (int i = 0; i < n; i++) {
        if (strcmp(nombres[i], nombreBuscado) == 0)
            return i;
    }
    return -1;
}
