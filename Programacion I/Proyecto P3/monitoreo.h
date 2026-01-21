#ifndef MONITOREO_H
#define MONITOREO_H

#define MAX_ZONAS 5
#define DIAS_HISTORICO 30
#define ARCHIVO_DATOS "historico_contaminacion.txt"
#define ARCHIVO_REPORTE "reporte_calidad_aire.txt"

// Estructuras
typedef struct {
    float velocidadViento;
    float temperatura;
    float humedad;
    float presion;
} Meteorologia;

typedef struct {
    float co2, so2, no2, pm25;
} Contaminantes;

typedef struct {
    char nombreZona[50];
    Meteorologia clima;
    Contaminantes actual;
    Contaminantes historico[DIAS_HISTORICO];
} ZonaUrbana;

// Prototipos de funciones
void inicializarZonas(ZonaUrbana *zonas);
void cargarHistorico(ZonaUrbana *zonas);
void guardarHistorico(ZonaUrbana *zonas);
void ingresarDatosActuales(ZonaUrbana *zonas);
void predecir24Horas(ZonaUrbana *z, Contaminantes *pred);
void generarAlertas(ZonaUrbana z, Contaminantes pred);
void generarReporte(ZonaUrbana *zonas);
void calcularPromedioZona(ZonaUrbana *z, Contaminantes *prom);


#endif