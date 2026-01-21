#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "val_func.h"

typedef struct {
    int id;
    char marca[30];
    char modelo[30];
    char tipo[20];   // camioneta, sedan, etc.
    float precio;
    int usado;       // 1 = sí, 0 = no
    int disponible;  // 1 = sí, 0 = vendido
} Vehiculo;

typedef struct {
    int id;
    char nombre[30];
    int edad;
    char prefMarca[30];
    char prefTipo[20];
    float presupuesto;
    int prefiereUsado; // 1 = sí, 0 = no
} Cliente;

void guardarVehiculo(Vehiculo v) {
    FILE *f = fopen("vehiculos.txt", "a");
    if (!f) { printf("Error al abrir archivo de vehiculos\n"); return; }
    fprintf(f, "%i,%s,%s,%s,%.2f,%i,%i\n", v.id, v.marca, v.modelo, v.tipo, v.precio, v.usado, v.disponible);
    fclose(f);
}

void guardarCliente(Cliente c) {
    FILE *f = fopen("clientes.txt", "a");
    if (!f) { printf("Error al abrir archivo de clientes\n"); return; }
    fprintf(f, "%i,%s,%i,%s,%s,%.2f,%i\n", c.id, c.nombre, c.edad, c.prefMarca, c.prefTipo, c.presupuesto, c.prefiereUsado);
    fclose(f);
}

void registrarVenta(int idCliente, int idVehiculo, float precio) {
    FILE *f = fopen("ventas.txt", "a");
    if (!f) { printf("Error al abrir archivo de ventas\n"); return; }
    fprintf(f, "%i,%i,%.2f\n", idCliente, idVehiculo, precio);
    fclose(f);
    printf("Venta registrada correctamente!\n");
}

void buscarVehiculos(char *marca, char *tipo, float presupuesto, int usado) {
    FILE *f = fopen("vehiculos.txt", "r");
    if (!f) { printf("No hay vehiculos registrados.\n"); return; }
    Vehiculo v;
    char linea[200];
    printf("Resultados de busqueda\n");
    while (fgets(linea, sizeof(linea), f)) {
        sscanf(linea, "%i,%[^,],%[^,],%[^,],%f,%d,%d", &v.id, v.marca, v.modelo, v.tipo, &v.precio, &v.usado, &v.disponible);
        if (v.disponible == 1 &&
            strcmp(v.marca, marca) == 0 &&
            strcmp(v.tipo, tipo) == 0 &&
            v.precio <= presupuesto &&
            v.usado == usado) {
            printf("ID:%i | %s %s %s | Precio: %.2f\n", v.id, v.marca, v.modelo, v.tipo, v.precio);
        }
    }
    fclose(f);
}

int main() {
    int opcion;
    do {
        printf("\n=== SGIC ===\n");
        printf("1. Registrar vehiculo\n");
        printf("2. Registrar cliente\n");
        printf("3. Buscar vehiculos para cliente\n");
        printf("4. Registrar venta\n");
        printf("0. Salir\n> ");
        scanf("%i", &opcion);
        getchar();

        if (opcion == 1) {
            Vehiculo v;
            v.id = obtener_entero_valido("ID: \n");
            validar_strings(v.marca,30, "Marca: \n");
            //while (getchar() != '\n');    //Funcion que captura el salto de linea
            printf("Modelo: "); fgets(v.modelo, 30, stdin); 
            v.modelo[strcspn(v.modelo, "\n")] = 0;          //Funcion strcspn elimina el salto de linea
            printf("Tipo: "); fgets(v.tipo, 20, stdin);
             v.tipo[strcspn(v.tipo, "\n")] = 0;
            printf("Precio: \n"); 
            scanf("%f", &v.precio);
            while (getchar() != '\n');
            v.usado = obtener_entero_valido("Usado (1=si,0=no): ");
            //while (getchar() != '\n');
            v.disponible = 1;
            guardarVehiculo(v);
            
        }
        else if (opcion == 2) {
            Cliente c;
            c.id = obtener_entero_valido("ID: \n");
            validar_strings(c.nombre,30, "Nombre: \n");
            c.edad = obtener_entero_valido("Edad: \n");
            validar_strings(c.prefMarca,30,"Pref Marca: \n");

            printf("Tipo: "); fgets(c.prefTipo, 20, stdin); 
            c.prefTipo[strcspn(c.prefTipo, "\n")] = 0;
            //while (getchar() != '\n');
            printf("Presupuesto: "); 
            scanf("%f", &c.presupuesto);
            while (getchar() != '\n');
            c.prefiereUsado = obtener_entero_valido("Prefiere usado (1=si,0=no): ");
            guardarCliente(c);
        }
        else if (opcion == 3) {
            char marca[30], tipo[20];
            float presupuesto;
            int usado;
            validar_strings(marca,30,"Marca: \n");

            printf("Tipo: "); fgets(tipo, 20, stdin); 
            tipo[strcspn(tipo, "\n")] = 0;
            //while (getchar() != '\n');
            printf("Presupuesto: \n"); 
            scanf("%f", &presupuesto);
            while (getchar() != '\n');
            usado = obtener_entero_valido("Usado (1=si,0=no): ");
            buscarVehiculos(marca, tipo, presupuesto, usado);
        }
        else if (opcion == 4) {
            int idCliente, idVehiculo;
            float precio;
            idCliente = obtener_entero_valido("ID Cliente: \n");
            idVehiculo = obtener_entero_valido("ID Vehiculo: \n");
            //while (getchar() != '\n');
            printf("Precio: "); 
            scanf("%f", &precio);
            registrarVenta(idCliente, idVehiculo, precio);
        }
    } while (opcion != 0);

    return 0;
}

//Comando para ejecutar: gcc Taller_3.c val_func.c -otaller13