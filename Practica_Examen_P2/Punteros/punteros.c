#include <stdio.h>

/*Verificar que efectivamente si yo modifico el valor de mi puntero, el valor de la variable original al que 
estuvo apuntando cambiara*/

int main (){
    int a=1;
    int *pa=&a;

    
    printf("El valor de la variable a es: %i\n", a);

    printf("El valor del puntero es: %i\n", *pa);

    //Modificamos el valor de nuestro puntero *pa, esto deberia modificar el valor de a;

    *pa=123;

    printf("El valor nuevo de a es: %i\n", a);
    printf("El valor nuevo del puntero es: %i\n", *pa);


    return 0;
}