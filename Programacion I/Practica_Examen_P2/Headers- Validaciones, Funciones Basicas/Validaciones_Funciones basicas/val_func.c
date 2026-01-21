#include "val_func.h"
/* Importante definir dentro de nuestro main las variables
- Char nombre []    para usar nuestra funcion de validar strings.
Forma de validar:  validar_strings(nombre, 100, "Ingrese Titulo del libro: ");
                    Para structures:
                    validar_strings(biblioteca[i].autor, 50, "Ingrese Autor: ");

- int n;     para usar nuestra funcion que devuelve un entero.
Forma de validar: n = obtener_entero_valido("Ingrese el ID del libro: ");
 Para structures: biblioteca[i].id = obtener_entero_valido("Ingrese el ID del libro: ");


 - float precios [N-productos];       para usar nuestra funcion que calcula promedios, nota: Dentro del vector precios podemos
                            almacenar varios segun el indice del mismo, este indice puede ser constante o variable.



    Nota: Si se quiere scanear un numero flotante despues de validar un char o un entero entonces se tiene que usar:
    ////////////////////while (getchar() != '\n');  /////// Sentencia para limpiar el salto de linea despues de un scanf.


*/
#define MAX_PROD 10             //Numero max de productos
#define MAX_NOMBRE 30           //Numero max de de caracteres para el nombre

void imprimir(Libros biblioteca[], int n){

    printf("%-5s %-25s %-10s\n", "N", "NOMBRE", "ID");
    printf("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n");
    for (int i = 0; i < n ; i++) {
        printf("%-5d %-25s %-10d\n", i+1 , biblioteca[i].titulo, biblioteca[i].id);
    }
}





void registrar_libros(Libros biblioteca[], int n) {
    printf("=== REGISTRO DE BIBLIOTECA ===\n");

    for (int i = 0; i < n; i++) {
        printf("\n--- Ingresando datos del Libro #%i ---\n", i + 1);
        // Nota: Al pasar el arreglo, accedemos directamente a la memoria original.
        // No estamos trabajando sobre una copia, sino sobre el arreglo real del main.

        // Asumiendo que tienes acceso a validar_strings y obtener_entero_valido:
        biblioteca[i].id = obtener_entero_valido("Ingrese el ID del libro: ");
        validar_strings(biblioteca[i].titulo, 100, "Ingrese Titulo del libro: ");
        validar_strings(biblioteca[i].autor, 50, "Ingrese Autor: ");
        biblioteca[i].anio = obtener_entero_valido("Ingrese el anio de publicacion: ");
        validar_strings(biblioteca[i].estado, 50, "Ingrese el estado del libro 'Prestado'o 'Disponible': ");
   
    }
}






float calcularTotal(float precios[], int n) {   //  funcion que suma el total de los costos de un producto
          
    float total = 0;
    for (int i = 0; i < n; i++)
        total += precios[i];
    return total;                   // Devuelve el promedio de los precios.
}

float calcularPromedio(float precios[], int n) {   // "n" simboliza cuantos productos ingresaron

    return calcularTotal(precios, n) / n;     // Devuelve el total de los precios acumulados.
}

int indiceMasCaro(float precios[], int n) {
    int indice = 0;
    for (int i = 1; i < n; i++)
        if (precios[i] > precios[indice])
            indice = i;
    return indice;              // Funcion que devuelve el indice del producto mas caro; Precios [indice]

}
int buscarProducto(char nombres[][MAX_NOMBRE], int n, char nombreBuscado[]) {
    for (int i = 0; i < n; i++) {
        if (strcmp(nombres[i], nombreBuscado) == 0)
            return i;           //Devuelve el indice del producto buscaco.
    }
    return -1;                  // Devuelve el valor -1, que podria significarse no encontrado
}


int indiceMasBarato(float precios[], int n) { 
    int indice = 0;
    for (int i = 1; i < n; i++)
        if (precios[i] < precios[indice])
            indice = i;              
    return indice;              //Devuelve el indice del producto mas barato.
}  







// Implementación de la función auxiliar
void eliminar_salto_linea(char *cadena) {
    cadena[strcspn(cadena, "\n")] = 0;
}

//Funcion para validar string.
void validar_strings(char *destino, int tamano, const char *mensaje) {
    int es_valido; // Bandera para saber si el string es correcto

    do {
        es_valido = 1; // Asumimos que es válido al iniciar el ciclo

        // 1. Pedimos el dato
        printf("%s", mensaje);
        
        // 2. Leemos el string (usamos fgets porque es más seguro que scanf)
        if (fgets(destino, tamano, stdin) == NULL) {
            continue; // Error de lectura, intentamos de nuevo
        }

        eliminar_salto_linea(destino);   //Funcion que elimina el salto de linea que fgets captura.
    

        // Si el usuario solo dio Enter (cadena vacía), no es válido
        if (strlen(destino) == 0) {
            es_valido = 0;
        }

        // 4. Validamos letra por letra
        for (int i = 0; i < strlen(destino); i++) {
            // Nota: isalpha retorna falso para los espacios.
            // Si quieres permitir espacios, cambia esto a: if (!isalpha(destino[i]) && !isspace(destino[i]))
            // Validamos que sea letra O sea un espacio en blanco
                if (!isalpha(destino[i]) && !isspace(destino[i])) { 
                es_valido = 0;
                break;
                }

        }

        // 5. Mensaje de error si falló
        if (!es_valido) {
            printf("Error: La entrada debe contener solo letras (sin numeros).\n");
        }

    } while (!es_valido); // Repetir mientras NO sea válido
}
// 2. Implementación para validar ENTEROS
int obtener_entero_valido(const char *mensaje) {
    char buffer[100];  //Aqui se alamacenara nuestro numero entero pero en forma de caracter.
    int es_valido;
    int resultado = 0;

    do {
        es_valido = 1;
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);
        eliminar_salto_linea(buffer);

        // Validación: Cadena vacía
        if (strlen(buffer) == 0) {
            es_valido = 0;
        }

        // Validación: Revisar si es número
        for (int i = 0; buffer[i] != '\0'; i++) {
            // Permitimos el signo negativo '-' solo en la primera posición
            if (i == 0 && buffer[i] == '-') {
                continue;
            }
            // Si no es un dígito
            if (!isdigit(buffer[i])) {
                printf(">> Error: '%s' no es un numero entero valido.\n", buffer);
                es_valido = 0;
                break;
            }
        }
        
    } while (es_valido == 0);

    // Convertir la cadena validada a entero
    resultado = atoi(buffer);
    return resultado;
}
