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
#include "mcp3004.h"
#include <softPwm.h>

#define RANGO       100
#define Izq       0 //El 17 en el GPIO
#define MotorPaso 4//EL 23 en el GPIO
#define Der       3 //El 22 en el GPIO



void Parar(){
    //15 el medio, 16 para arriba avanzar
    softPwmWrite(Izq, 0 );

    //15 para abajo avanza
    softPwmWrite(Der, 0 );
    }

void avanzar(){
    //15 el medio, 16 para arriba avanzar
    softPwmWrite(Izq, 20 );

    //15 para abajo avanza
    softPwmWrite(Der, 10 );
}

void girarDerecha(){
  //Paro la rueda izquierda
    //15 el medio
	softPwmWrite(Izq, 20 );
    softPwmWrite(Der, 20 );
	delay(10);
   
}

void girarIzquierda(){
    //Paro la rueda izquierda
    //15 el medio
	softPwmWrite(Izq, 10 );
    softPwmWrite(Der, 10 );
	delay(10);

   
}

int main(int argc, char *argv[]) {

    int parado = 0;
	
    wiringPiSetup();
    mcp3004Setup(100, 0);

    softPwmCreate (Der, 0, RANGO);//Derecho
    softPwmCreate (Izq, 0, RANGO);//Izquierdo
    while(1){
        int ch1 = analogRead(100);
        int ch2 = analogRead(101);
        int ch3 = analogRead(102);
        int ch4 = analogRead(103);

        if (parado == 1){
            if(ch1 < 300 && ch2 < 300 ){
            avanzar();
            parado = 0;
            }
            else {
                if (ch1 >= 300 && ch2 < 300){
                    //--------------------
                    //Explotar globo derecha
                    //--------------------
                }
                if (ch2 >= 300 && ch1 < 300){
                    //--------------------
                    //Explotar globo izquierda
                    //--------------------
                }
            }
            
        }

        else{

            //Sensor linea izquierda
            if(ch3 <= 600 ){
                girarIzquierda();
                printf ("Sensor linea izquierdo activado, girando izquierda");
            }
            
            else if(ch4 <= 300 ){
		//sensor linea derecha
                printf ("Sensor linea derecha activado, girando derecha");
                girarDerecha();
            }else{
                avanzar();
            }

            if (ch3 < 600 && ch4 < 300){
                Parar();
                parado = 1;
            }
            
            //Globo derecha
            if(ch1 >= 300){
                Parar();
                parado = 1;
                printf ("Sensor obstaculo derecho activado, parando");
                //Torreta
            }


            //globo izquierda
            if(ch2 >= 300 ){
                Parar();
                parado = 1;
                printf ("Sensor obstaculo izquierdo activado, parando");
                //Torreta y algun calculo de distancia
            }
        

            
        }
    }
    return 0;
	
}