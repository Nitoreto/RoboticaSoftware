/*
 *  readMcp3008.c:
 *  read value from ADC MCP3008
 *
 * Requires: wiringPi (http://wiringpi.com)
 * Copyright (c) 2015 http://shaunsbennett.com/piblog
 ***********************************************************************
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define RANGO       100
#define Izq       0 //El 17 en el GPIO
#define MotorPaso 4//EL 23 en el GPIO
#define Der       3 //El 22 en el GPIO


void Parar(){
    //15 el medio, 16 para arriba avanzar
    softPwmWrite(Izq, 15 );

    //15 para abajo avanza
    softPwmWrite(Der, 15 );
}

void avanzar(){
    //15 el medio, 16 para arriba avanzar
    softPwmWrite(Izq, 20 );

    //15 para abajo avanza
    softPwmWrite(Der, 10 );
}

void girarDerecha(){
    //Paro rueda derecha
    //14 para abajo avanza
    softPwmWrite(Der, 15 );
}

void girarIzquierda(){
    //Paro la rueda izquierda
    //15 el medio
    softPwmWrite(Izq, 15 );
}

int main(int argc, char *argv[]) {
	
    wiringPiSetup();
    mcp3004Setup(100, 0);

    softPwmCreate (Der, 0, RANGO);//Derecho
    softPwmCreate (Izq, 0, RANGO);//Izquierdo
    while(1){
       
        //Globo derecha
        if(myAnalogRead(100) >= 300){
            Parar();
            printf ("Sensor obstaculo derecho activado, parando");
            //Torreta
        }

        //globo izquierda
        if(myAnalogRead(101) >= 300 ){
            Parar();
            printf ("Sensor obstaculo izquierdo activado, parando");
            //Torreta y algun calculo de distancia
        }

        //Sensor linea izquierda
        if(myAnalogRead(102) <= 600 ){
            girarIzquierda();
            printf ("Sensor linea izquierdo activado, girando izquierda");
        }else{
            avanzar();
        }

        //sensor linea derecha
        if(myAnalogRead(103) <= 300 ){
            printf ("Sensor linea derecha activado, girando derecha");
            girarDerecha();
        }else{
            avanzar();
        }
			
		
	}
    return 0;
	
}