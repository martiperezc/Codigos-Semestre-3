#include <stdio.h>
#include <stdlib.h>
#include "Validaciones.h" // Tu archivo header

// Definimos la estructura
 


typedef struct {
    char titulo[100];
    char autor[50];
    char editorial[50];
    int id;
    float precio;
} Libro;

int main() {
    int n;
    n = obtener_entero_valido("Cuantos libros quiere registrar?: \n");

    Libro biblioteca[n]; // Arreglo de estructuras

    printf("=== REGISTRO DE BIBLIOTECA ===\n");

    for (int i = 0; i < n; i++) {
        printf("\n--- Ingresando datos del Libro #%d ---\n", i + 1);

        // LLAMADA A LA FUNCIÓN:
        // Simplemente pasamos el campo especifico del struct.
        // biblioteca[i].titulo funciona igual que una variable string normal.
        
        validar_strings(biblioteca[i].titulo, 100, "Ingrese Titulo del libro: ");
        validar_strings(biblioteca[i].autor, 50, "Ingrese Autor: ");
        validar_strings(biblioteca[i].editorial, 50, "Ingrese Editorial: ");
        biblioteca[i].id = obtener_entero_valido("Ingrese el ID del libro: ");
        printf("Ingrese el costo del libro: \n");
        scanf("%f", &biblioteca[i].precio);
        while (getchar() != '\n'); // Sentencia para limpiar el salto de linea despues de un scanf.
    }

    // Mostrar los datos para verificar
    printf("\n\n=== LIBROS REGISTRADOS ===\n");
    for (int i = 0; i < n; i++) {
        printf("Libro %d: %s, por %s (Ed. %s), id: %i, precio: %.2f\n", 
               i+1, 
               biblioteca[i].titulo, 
               biblioteca[i].autor, 
               biblioteca[i].editorial,
               biblioteca[i].id,
               biblioteca[i].precio);
    }

    return 0;
}