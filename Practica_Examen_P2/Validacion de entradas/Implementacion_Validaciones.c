#include <stdio.h>
#include <stdlib.h>  //Libreria para hacer funcionar nuestra validacion de enteros
#include "validaciones.h" // Incluimos nuestro archivo con comillas dobles

int main() {
    char nombre[50];
    char apellido[50];
    char ciudad[50];

    printf("=== BIENVENIDO AL SISTEMA ===\n\n");

    // Llamamos a la función. Ella sola se encarga de pedir, leer y validar.
    // Si el usuario se equivoca, la función no le dejará avanzar.
    
    validar_strings(nombre, 50, "Ingrese su Nombre: ");
    validar_strings(apellido, 50, "Ingrese su Apellido: ");
    validar_strings(ciudad, 50, "Ingrese su Ciudad: ");

    printf("\n--- Datos Guardados ---\n");
    printf("Nombre: %s\n", nombre);
    printf("Apellido: %s\n", apellido);
    printf("Ciudad: %s\n", ciudad);

    return 0;
}