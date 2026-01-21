#include <stdio.h>
#include <stdlib.h>
#include "Validaciones.h"
#include "operaciones.h"

int main() {
    char nombres[MAX_PROD][MAX_NOMBRE];
    float precios[MAX_PROD];
    int n;




   /*printf("cuantos productos desea ingresar (max 10): ");
    scanf("%d", &n);*/
    n= obtener_entero_valido ("Cuantos productos va a ingresar?:" );
    if (n < 1 || n > MAX_PROD) {
        printf("numero invalido\n");
        return 0;
    }

    ingresarDatos(nombres, precios, n);

    float total = calcularTotal(precios, n);
    float promedio = calcularPromedio(precios, n);
    int caro = indiceMasCaro(precios, n);
    int barato = indiceMasBarato(precios, n);

    printf("precio total: %.2f\n", total);
    printf("precio promedio: %.2f\n", promedio);
    printf("producto mas caro: %s (%.2f)\n", nombres[caro], precios[caro]);
    printf("producto mas barato: %s (%.2f)\n", nombres[barato], precios[barato]);

    char nombreBuscado[MAX_NOMBRE];\
    validar_strings(nombreBuscado, MAX_NOMBRE, "Ingrese el nombre a buscar:");
    int pos = buscarProducto(nombres, precios, n, nombreBuscado);
    if (pos != -1)
        printf("precio de %s: %.2f\n", nombres[pos], precios[pos]);
    else
        printf("producto no encontrado\n");
    return 0;
}
