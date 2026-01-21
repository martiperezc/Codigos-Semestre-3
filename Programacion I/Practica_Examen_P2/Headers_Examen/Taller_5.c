#include <stdio.h>
#include "val_func.h"


int main (){
    
     //Preguntamos cuantos libros quiere registrar
    int n, indice = 0, decision, idbuscado, menu, back, k;
    char nombreBuscado[50], estado[50];
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

    //Funcion que busca el producto
    
    do {
        back=1;
         menu = obtener_entero_valido("Bienvenido al menu interactivo, elija el numero con la accion que requiera\n 1) Mostrar info de todos los libros\n 2) Busqueda de libros, y actualizacion de estado\n 3 Eliminar un libro: \n");
        switch (menu)
    {
    case 1: // Info libros
        //Funcion que imprime los productos
        imprimir(biblioteca, n);
        back = obtener_entero_valido("Desea volver al menu principal, donde 0) NO \t 1) Si\n");
        break;
    case 2: // Busqueda
         do  //Bucle que busca el producto, asi este haya sido ingresado incorrectamente.
        {   
        decision = obtener_entero_valido("Elija una opcion para encontrar su producto 1)Titulo o 2)ID: ");
        switch (decision)
        {
        case 1:
            printf("Selecciono la opcion 1) Busqueda por titulo\n");
            indice = buscarProducto(biblioteca, n, nombreBuscado, k, estado);
            imprimir_info(biblioteca, indice);   //Funcion que imprime toda la info de nuestro producto buscado

            break;
        case 2:
            printf("Selecciono la opcion 2) Busqueda por id\n");
            indice = buscarProductoID(biblioteca, n, idbuscado, k, estado);
            imprimir_info(biblioteca, indice);   //Funcion que imprime toda la info de nuestro producto buscado
        default:
            break;
        }
        } while (indice == -1);
        back = obtener_entero_valido("Desea volver al menu principal, donde 0) NO \t 1) Si\n");
        break;
    case 3:



     break;

    default:
        break;
    }

} while(back!=0);



    
    
    





   



   
    //printf("%i", indice);




    return 0;

}  





// Comando para ejecutar:gcc Taller_5.c val_func.c -otaller 
