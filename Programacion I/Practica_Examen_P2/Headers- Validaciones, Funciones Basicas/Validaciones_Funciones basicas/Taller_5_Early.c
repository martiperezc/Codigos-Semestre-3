#include <stdio.h>
#include "val_func.h"


int main (){
    
     //Preguntamos cuantos libros quiere registrar
    int n;
   
    int l=0;
    while (l!=1){
         n = obtener_entero_valido("Cuantos libros quiere registrar: \n");   
        if (n<10){
            l=1;
        } else {
            printf("No se pueden ingresar mas de 10 libros\n");
        }
    }
    // Creamos el nombre de nuestra struct 
    Libros biblioteca[n];
    // Lamamos a funcion que llena datos
    registrar_libros(biblioteca, n);
    imprimir(biblioteca, n);









    return 0;
}





// Comando para ejecutar:gcc Taller_5.c val_func.c -otaller 
