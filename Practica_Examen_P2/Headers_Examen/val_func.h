#ifndef validaciones_funciones
#define validaciones_funciones
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PROD 10
#define MAX_NOMBRE 30


typedef struct {
    char titulo[100];
    char autor[50];
    int id, anio;
    char estado[50];
} Libros;

//Funcion para imprimir tabla
void imprimir(Libros biblioteca[], int n);


// Funcion para rellenar data de un struct de tipo arreglo
void registrar_libros(Libros biblioteca[], int n);



// Funciones para operaciones
float calcularTotal(float precios[], int n);
float calcularPromedio(float precios[], int n);
int indiceMasCaro(float precios[], int n);
int indiceMasBarato(float precios[], int n);
int buscarProducto(Libros biblioteca[], int n, char nombreBuscado[], int k, char estado[]);
int buscarProductoID(Libros biblioteca[], int n, int idbuscado, int k, char estado[]);
void imprimir_info(Libros biblioteca[], int indice);



// Funciones para Validar

void eliminar_salto_linea(char *cadena);
void validar_strings(char *destino, int tamano, const char *mensaje);
int obtener_entero_valido(const char *mensaje);






#endif