#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    char nombre[50];
    char pais[30];
    float dinero;
    
}  Registro;




void registro_data(Registro A){
    FILE *archivo = fopen("base_datos.txt", "a"); //Modo apendice para agregar datos al final de la ultima linea.
    if(!archivo){
    printf("No se pudo abrir el archivo.txt\n");
    return;
    }
    fprintf(archivo, "%s %s %.2f\n", A.nombre, A.pais, A.dinero);
    fclose(archivo);

}


void busqueda(char *cliente_buscado){
    int encontrado =0;
    Registro temporal;



    FILE *archivo = fopen("base_datos.txt", "r");  //Modo lectura de nuestro archivo txt
    if(!archivo){
        printf("No se pudo abrir el archivo.txt\n");
        return;
    }
    while(fscanf(archivo,"%s %s %f", temporal.nombre, temporal.pais, &temporal.dinero)!= EOF){
        if(strcmp(temporal.nombre, cliente_buscado)==0){
            printf("Se encontro al usuario\n");
            printf("Nombre: %s\t Pais: %s\t Dinero: %.2f\n", temporal.nombre, temporal.pais, temporal.dinero);
            encontrado =1;
        }
    }
    if (!encontrado) {
    printf("No se encontro ningun registro con el nombre %s \n", cliente_buscado);
    }
    fclose(archivo);
}

int main(){
    int opcion;
    Registro cliente;
    char cliente_buscado[50];


    do {
        printf("1. Agregar Cliente\n 2. Buscar Cliente\n 3. Salir \nOpcion: \n");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese nombre: \n");
                scanf("%s", cliente.nombre);
                printf("Ingrese el pais del usuario: %s \n", cliente.nombre);
                scanf("%s", cliente.pais);
                printf("Ingrese cuanto dinero dispone: \n");
                scanf("%f", &cliente.dinero);
                registro_data(cliente);  //Guardamos nuestros datos ingresados dentro de archivo txt.
                break;
            case 2:
                printf("Ingrese nombre a buscar: ");
                scanf("%s", cliente_buscado);
                busqueda(cliente_buscado);
                break;
            default:
                printf("Esa opcion no es valida\n");
            
        }
    } while (opcion != 3);

    return 0;
}