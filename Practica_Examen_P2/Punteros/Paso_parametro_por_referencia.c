#include <stdio.h>
//Codigo que comprueba el concepto "Pasos de parametro por referencia"
// Importante: Con el paso de parametro por referencia, la variable original es modificada, mediante apuntadores.

void agregar(int *a){       // Aqui usamos el * para apuntar a la direccion de la variable que llega a la funcion.
    *a+=10;         // Importante seguir usando el * para modificar el valor del apuntador.


}

int main (){
     int num;

     printf("Ingresa el valor de tu numero:\n");
     scanf("%i", &num);

     printf("El valor antes de someter el numero a la funcion es de: %i\n", num);

     agregar(&num); // Usamos & para tomar el valor que esta en la direccion de nuestra variable num

     printf("El valor despues de someter el numero a la funcion es de: %i\n", num);

    return 0;
}


