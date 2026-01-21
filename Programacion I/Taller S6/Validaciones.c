#include "Validaciones.h"

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
