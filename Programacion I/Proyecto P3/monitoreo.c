#include <stdio.h>
#include <string.h>
#include "monitoreo.h"

// Definición de límites OMS
const float L_SO2 = 40.0;
const float L_NO2 = 25.0;
const float L_PM25 = 15.0;

void inicializarZonas(ZonaUrbana *zonas) {
    char *nombres[] = {"Norte", "Sur", "Centro", "Este", "Oeste"};

    for(int i = 0; i < MAX_ZONAS; i++) {
        strcpy(zonas[i].nombreZona, nombres[i]);

        memset(&zonas[i].actual, 0, sizeof(Contaminantes));
        memset(&zonas[i].clima, 0, sizeof(Meteorologia));
        memset(zonas[i].historico, 0, sizeof(zonas[i].historico));
    }
}


void predecir24Horas(ZonaUrbana *z, Contaminantes *pred) {
    float sumPM = 0, pesoTotal = 0;
    for(int j = 0; j < DIAS_HISTORICO; j++) {
        float peso = (float)(DIAS_HISTORICO - j);
        sumPM += z->historico[j].pm25 * peso;
        pesoTotal += peso;
    }
    // Lógica: Más viento = menos contaminación; Más humedad = más PM2.5
    float factor = (z->clima.velocidadViento > 15) ? 0.75 : 1.1;
    pred->pm25 = (sumPM / pesoTotal) * factor;
    if (pred->pm25 < 0)     //Validacion contaminacion no puede ser negativa
    pred->pm25 = 0;
}

void cargarHistorico(ZonaUrbana *zonas) {
    int archivoVacio = 1;
    FILE *f = fopen(ARCHIVO_DATOS, "r");
    if(!f) return;

    for(int i = 0; i < MAX_ZONAS; i++) {
        for(int j = 0; j < DIAS_HISTORICO; j++) {
            if (fscanf(f, "%f %f %f %f",
                &zonas[i].historico[j].co2,
                &zonas[i].historico[j].so2,
                &zonas[i].historico[j].no2,
                &zonas[i].historico[j].pm25) == 4) {

                archivoVacio = 0;
            }
        }
    }
    fclose(f);

    // Si el archivo estaba vacío o incompleto, NO sobrescribir datos válidos
    if (archivoVacio) {
        for(int i = 0; i < MAX_ZONAS; i++)
            memset(zonas[i].historico, 0, sizeof(zonas[i].historico));
    }
}


void guardarHistorico(ZonaUrbana *zonas) {
    FILE *f = fopen(ARCHIVO_DATOS, "w");
    if(!f) return;

    for(int i = 0; i < MAX_ZONAS; i++) {
        for(int j = 0; j < DIAS_HISTORICO; j++) {
            fprintf(f, "%.2f %.2f %.2f %.2f ", zonas[i].historico[j].co2, zonas[i].historico[j].so2,
                                       zonas[i].historico[j].no2, zonas[i].historico[j].pm25);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}




void ingresarDatosActuales(ZonaUrbana *zonas) {
    for(int i = 0; i < MAX_ZONAS; i++) {
        printf("\n--- Datos para Zona: %s ---\n", zonas[i].nombreZona);
        printf("Temperatura (C) y Humedad (%%): ");
        scanf("%f %f", &zonas[i].clima.temperatura, &zonas[i].clima.humedad);
        printf("Vel. Viento (km/h): ");
        scanf("%f", &zonas[i].clima.velocidadViento);
        printf("Niveles CO2, SO2, NO2, PM2.5: ");
        scanf("%f %f %f %f", &zonas[i].actual.co2, &zonas[i].actual.so2, 
                             &zonas[i].actual.no2, &zonas[i].actual.pm25);
        
        // Desplazar histórico y agregar actual
        for(int j = DIAS_HISTORICO - 1; j > 0; j--) {
            zonas[i].historico[j] = zonas[i].historico[j-1];
        }
        zonas[i].historico[0] = zonas[i].actual;
    }
}
void generarAlertas(ZonaUrbana z, Contaminantes pred) {
    printf("\n>>> ANALISIS DE RIESGO: ZONA %s <<<\n", z.nombreZona);
    int alerta = 0;

    if(pred.pm25 > L_PM25) {
        printf("[ALERTA] PM2.5 previsto (%.2f) excede limite OMS (%.2f)\n", pred.pm25, L_PM25);
        alerta = 1;
    }
    if(pred.no2 > L_NO2) {
        printf("[ALERTA] NO2 previsto (%.2f) excede limite OMS (%.2f)\n", pred.no2, L_NO2);
        alerta = 1;
    }

    if(alerta) {
        printf("RECOMENDACIONES:\n");
        printf("- Reduccion de trafico vehicular en el centro.\n");
        printf("- Suspender actividades fisicas al aire libre.\n");
        printf("- Uso obligatorio de mascarillas N95 en grupos de riesgo.\n");
    } else {
        printf("Calidad del aire dentro de rangos aceptables.\n");
    }
}
void generarReporte(ZonaUrbana *zonas) {
    FILE *rep = fopen(ARCHIVO_REPORTE, "w");
    if(!rep) return;

    fprintf(rep, "REPORTE INTEGRAL DE CALIDAD DEL AIRE\n");
    fprintf(rep, "====================================\n\n");

    for(int i = 0; i < MAX_ZONAS; i++) {
        Contaminantes pred;
        predecir24Horas(&zonas[i], &pred);

        fprintf(rep, "ZONA: %s\n", zonas[i].nombreZona);
        fprintf(rep, "Nivel Actual PM2.5: %.2f ug/m3\n", zonas[i].actual.pm25);
        fprintf(rep, "Prediccion 24h PM2.5: %.2f ug/m3\n", pred.pm25);
        
        Contaminantes promedio;
        calcularPromedioZona(&zonas[i], &promedio);
        fprintf(rep, "Promedio mensual PM2.5 (30 dias): %.2f ug/m3\n", promedio.pm25);

        
        if(pred.pm25 > L_PM25) {
            fprintf(rep, "ESTADO: CRITICO - Se requiere mitigacion inmediata.\n");
        } else {
            fprintf(rep, "ESTADO: NORMAL\n");
        }
        fprintf(rep, "------------------------------------\n");
    }



    fclose(rep);
    printf("\nReporte generado exitosamente en '%s'\n", ARCHIVO_REPORTE);
}

void calcularPromedioZona(ZonaUrbana *z, Contaminantes *prom) {
    float sumCO2 = 0, sumSO2 = 0, sumNO2 = 0, sumPM25 = 0;
    int diasValidos = 0;

    for(int i = 0; i < DIAS_HISTORICO; i++) {
        // Ignorar días sin datos
        if (z->historico[i].pm25 > 0 ||
            z->historico[i].co2  > 0 ||
            z->historico[i].so2  > 0 ||
            z->historico[i].no2  > 0) {

            sumCO2  += z->historico[i].co2;
            sumSO2  += z->historico[i].so2;
            sumNO2  += z->historico[i].no2;
            sumPM25 += z->historico[i].pm25;
            diasValidos++;
        }
    }

    if (diasValidos > 0) {
        prom->co2  = sumCO2  / diasValidos;
        prom->so2  = sumSO2  / diasValidos;
        prom->no2  = sumNO2  / diasValidos;
        prom->pm25 = sumPM25 / diasValidos;
    } else {
        prom->co2 = prom->so2 = prom->no2 = prom->pm25 = 0;
    }
}
