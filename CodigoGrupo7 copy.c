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



#define	TRUE	            (1==1)
#define	FALSE	            (!TRUE)
#define CHAN_CONFIG_SINGLE  8
#define CHAN_CONFIG_DIFF    0

static int myFd;

char *usage = "Usage: mcp3008 all|analogChannel[1-8] [-l] [-ce1] [-d]";
// -l   = load SPI driver,  default: do not load
// -ce1  = spi analogChannel 1, default:  0
// -d   = differential analogChannel input, default: single ended

void loadSpiDriver() {
	
    if (system("gpio load spi") == -1) {
		
        fprintf(stderr, "Can't load the SPI driver: %s\n", strerror (errno));
		
        exit(EXIT_FAILURE);
		
    }
	
}

void spiSetup(int spiChannel) {
	
    if ((myFd = wiringPiSPISetup(spiChannel, 1000000)) < 0) {
		
        fprintf(stderr, "Can't open the SPI bus: %s\n", strerror(errno));
		
        exit(EXIT_FAILURE);
		
    }
	
}

int myAnalogRead(int spiChannel, int channelConfig, int analogChannel) {
	
    if(analogChannel < 0 || analogChannel > 7)
        return -1;
		
    unsigned char buffer[3] = {1}; // start bit
	
    buffer[1] = (channelConfig + analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
	
    return ((buffer[1] & 3) << 8) + buffer[2]; // get last 10 bits
	
}
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
    //Paro rueda izquierda
    //14 para abajo avanza
    softPwmWrite(Der, 15 );
}

void girarIzquierda(){
    //Paro la rueda izquierda
    //15 el medio
    softPwmWrite(Izq, 15 );
}

int main(int argc, char *argv[]) {
	
    int loadSpi = FALSE;
    int analogChannel = 0;
    int spiChannel = 0;
    int channelConfig = CHAN_CONFIG_SINGLE;
	
    if (argc < 2) {
		
        fprintf(stderr, "%s\n", usage);
		
        return 1;
		
    }
	
    if((strcasecmp(argv[1], "all") == 0))
		argv[1] = "0";
		
    if ((sscanf(argv[1], "%i", &analogChannel) != 1) || analogChannel < 0 || analogChannel > 8) {
		
        printf ("%s\n", usage);
		
        return 1;
		
    }
	
    int i;
	
    for(i = 2; i < argc; i++) {
		
        if (strcasecmp(argv[i], "-l") == 0 || strcasecmp(argv[i], "-load") == 0)
            loadSpi = TRUE;
			
        else if (strcasecmp(argv[i], "-ce1") == 0)
            spiChannel = 1;
			
        else if (strcasecmp(argv[i], "-d") == 0 || strcasecmp(argv[i], "-diff") == 0)
            channelConfig = CHAN_CONFIG_DIFF;
			
    }
	
    if(loadSpi == TRUE)
        loadSpiDriver();
	
    wiringPiSetup();
    spiSetup(spiChannel);

    softPwmCreate (Der, 0, RANGO);//Derecho
    softPwmCreate (Izq, 0, RANGO);//Izquierdo
    while(1){
       
        //Globo derecha
        if(myAnalogRead(spiChannel, channelConfig, 0) >= 300){
            Parar();
            printf ("Sensor obstaculo derecho activado, parando");
            //Torreta
        }

        //globo izquierda
        if(myAnalogRead(spiChannel, channelConfig, 1) >= 300 ){
            Parar();
            printf ("Sensor obstaculo izquierdo activado, parando");
            //Torreta y algun calculo de distancia
        }

        //Sensor linea izquierda
        if(myAnalogRead(spiChannel, channelConfig, 2) <= 600 ){
            girarIzquierda();
            printf ("Sensor linea izquierdo activado, girando izquierda");
        }else{
            avanzar();
        }

        //sensor linea derecha
        if(myAnalogRead(spiChannel, channelConfig, 3) <= 300 ){
            printf ("Sensor linea derecha activado, girando derecha");
            girarDerecha();
        }else{
            avanzar();
        }
			
		
	}
	
    close(myFd);
	
    return 0;
	
}