#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <softPwm.h>

#define RANGO       100
#define PIN17       0 //El 17 en el GPIO
#define PIN22       3 //El 22 en el GPIO



void avanzar(double t){
    //15 el medio, 16 para arriba avanzar
    softPwmCreate (PIN17, 0, RANGO);//ziquierda
    softPwmWrite(PIN17, 20 );

    //15 para abajo avanza
    softPwmCreate (PIN22, 0, RANGO);//Derecho
    softPwmWrite(PIN22, 10 );

    delay(t * 1000);

}

void girarDerecha(){

    //15 el medio, 16 para arriba avanzar
    softPwmCreate (PIN17, 0, RANGO);//Izquierdo
    softPwmWrite(PIN17, 20 );

    //14 para abajo avanza
    softPwmCreate (PIN22, 0, RANGO);//Derecho
    softPwmWrite(PIN22, 15 );
    delay(1723);
}

void cuadrado(double d1, double d2){
    double t1 = (d1/20.42)/0.77;
    double t2 = (d2/20.42)/0.77;
    avanzar(t1);
    girarDerecha();
    avanzar(t2);
    girarDerecha();
    avanzar(t1);
    girarDerecha();
    avanzar(t2);
    girarDerecha();
}

int main ()
{
    printf ("PWM Basico\n") ;
    wiringPiSetup () ;

    cuadrado(120, 40);

    softPwmWrite (PIN17, 0);
    softPwmWrite (PIN22, 0);

    return 0;

}
