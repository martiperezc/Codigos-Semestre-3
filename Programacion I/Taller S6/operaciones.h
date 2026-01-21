#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_PROD 10
#define MAX_NOMBRE 30

void ingresarDatos(char nombres[][MAX_NOMBRE], float precios[], int n);
float calcularTotal(float precios[], int n);
float calcularPromedio(float precios[], int n);
int indiceMasCaro(float precios[], int n);
int indiceMasBarato(float precios[], int n);
int buscarProducto(char nombres[][MAX_NOMBRE], float precios[], int n, char nombreBuscado[]);

#endif
