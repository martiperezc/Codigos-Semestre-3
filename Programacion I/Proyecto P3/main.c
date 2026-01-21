#include <stdio.h>
#include "monitoreo.h"

int main() {
    ZonaUrbana zonas[MAX_ZONAS];
    Contaminantes contaminante;
    inicializarZonas(zonas);
    cargarHistorico(zonas);
    //predecir24Horas(zonas, contaminante);
    int opcion;
   do {
        printf("\n--- SISTEMA DE MONITOREO AMBIENTAL URBANO ---\n");
        printf("1. Ingresar datos actuales por zona\n");
        printf("2. Ver predicciones y alertas (24h)\n");
        printf("3. Generar reporte general en archivo\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                ingresarDatosActuales(zonas);
                break;
            case 2:
                for(int i = 0; i < MAX_ZONAS; i++) {
                    Contaminantes pred;
                    predecir24Horas(&zonas[i], &pred);
                    generarAlertas(zonas[i], pred);
                }
                break;
            case 3:
                guardarHistorico(zonas);
                generarReporte(zonas);
                break;
            case 4:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while(opcion != 4);

    return 0;
}

//Codigo para ejecutar: gcc main.c monitoreo.c -omain