//*Elevador de 5 plantas y una planta baja realizado por LAR S.L.*//
//***********Progrmado en lenguaje C Con Compilador CCS********** //
//*******Autores: Erik Lekuona, Mikel Altuna, Jon Rodriguez*******//
//***************Ciclo: Mantenimiento Electrónico*****************//

#include <16f887.h>					//Pic a utilizar                   
#fuses INTRC_IO,NOWDT,NOPUT,NOPROTECT,NOCPD,NOLVP					//Configuracion inicial del pic.	    
#use delay(clock=4000000)				//Frecuencia del oscilador 4Mhz
#use RS232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)					//Configuracion del puerto serie; Velocidad 9600 baudios; Pin_C6 transmision y Pin_C7 recepcion.
#byte osccon=0x8f					//Registro Osccon en la posición 0x8f

#use fast_io(B)
#use fast_io(D)

#bit detect0=0x06.0					//detect0=Puerto B Pin 0.
#bit detect1=0x06.1					//detect1=Puerto B Pin 1.				
#bit detect2=0x06.2					//detect2=Puerto B Pin 2.
#bit detect3=0x06.3					//detect3=Puerto B Pin 3.
#bit detect4=0x06.4					//detect4=Puerto B Pin 4.
#bit detect5=0x06.5					//detect5=Puerto B Pin 5. 		

#bit puls0=0x05.0					//puls0=Puerto A Pin 0.
#bit puls1=0x05.1					//puls1=Puerto A Pin 1.
#bit puls2=0x05.2					//puls2=Puerto A Pin 2.
#bit puls3=0x05.3					//puls3=Puerto A Pin 3.
#bit puls4=0x05.4					//puls4=Puerto A Pin 4.
#bit puls5=0x05.5					//puls5=Puerto A Pin 5.

int dato_serie=7;					//Inicializamos dato_serie con un valor desconocido.		


void subir (void);					//Declaración Funcion Subir.
void bajar (void);					//Declaración Función Bajar.																
void parar (void);					//Declaración Función Parar.

#INT_RDA											//Interrupción para cuando llegue algo por el Puerto Serie.
elevador()
{
	switch(getc())									//Lee lo que haya por el Puerto Serie
	{
		case '0': dato_serie=0; break;				//Si llega por el puerto serie un '0' que lo guarde en dato_serie=0.
		case '1': dato_serie=1; break;				//Si llega por el puerto serie un '1' que lo guarde en dato_serie=1.
		case '2': dato_serie=2; break;				//Si llega por el puerto serie un '2' que lo guarde en dato_serie=2.
		case '3': dato_serie=3; break;				//Si llega por el puerto serie un '3' que lo guarde en dato_serie=3.
		case '4': dato_serie=4; break;				//Si llega por el puerto serie un '4' que lo guarde en dato_serie=4.
		case '5': dato_serie=5; break;				//Si llega por el puerto serie un '5' que lo guarde en dato_serie=5.
	}
}
void main(void)										//Función Principal.
{   
	enable_interrupts(GLOBAL);						//Habilitar la interrupcion.
	enable_interrupts(INT_RDA);						//Habilitar Interrupción Puerto Serie.
	set_tris_A(0xff);								//Declaramos el Puerto A como entradas.
	set_tris_B(0xff);								//Declaramos el Puerto B como entradas.
	set_tris_D(0x00);								//Declaramos el Puerto D como salidas.
	set_tris_C(0x00);								//Declaramos el puerto C como salidas.
	output_B(0);									//Inicializamos el puerto B a 0.
	output_C(0);									//Inicializamos el Puerto C a 0.
	output_A(0);									//Inicializamos el puerto A a 0.
	parar();										//Inicializamos el Puerto D a 0.
	osccon=0x61;									//Utilizamos el oscilador interno a 4Mhz

	while(1)
	{
		if (detect0==1)										//Si esta detectando el 0.
		{
			parar();										//Que cuando detecte se pare.
			delay_ms(200);
			printf("Piso 0.\n");							//Envía por el Puerto Serie "Piso 0."
			delay_ms(100);
			printf(" ");
			if ((puls0==1)||(dato_serie==0))				//Si pulso el 0 o dato_serie=0.
			{
				delay_ms(200);
				output_bit(PIN_C0,1);						//Led del Pulsador 0 a 1.
				parar();									//Parar.
				dato_serie=7;								//Le damos a dato_serie un valor cualquiera.
			}
			if ((puls1==1)||(dato_serie==1))				//Si pulso el 1 o dato_serie=1.
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C1,1);						//Led del Pulsador 1 a 1.
				subir();									//Subir.
				while (detect1==0)							//Mientras que no detecte el 1.
				{
					printf("SUBIENDO AL P1.\n");			//Envía por el puerto Serie "SUBIENDO AL P1."
					delay_ms(100);	
					printf(" ");							//Que escriba y limpie continuamente
				}
			}
			if ((puls2==1)||(dato_serie==2))				//Si pulso el 2 o dato_serie=2
			{
				delay_ms(200);
				printf(" ");								//Limpia lo que haya en el Puerto Serie.
				output_bit(PIN_C2,1);						//Led del Pulsador 2 a 1.
				subir();									//Subir.
				while (detect2==0)							//Mientras que no detecte el 2.
				{
					printf("SUBIENDO AL P2.\n");			//Envía por el puerto Serie "SUBIENDO AL P2."
					delay_ms(100);					
					printf(" ");							//Que escriba y limpie continuamente.
				}
			}
			if ((puls3==1)||(dato_serie==3))				//Si pulso el 3 o dato_serie=3
			{
				delay_ms(200);								
				printf(" ");								//Limpia lo que haya en el Puerto Serie.
				output_bit(PIN_C3,1);
				subir();									//Subir.
				while (detect3==0)							//Mientras que no detecte el 3.
				{
					printf("SUBIENDO AL P3.\n");			//Envía por el Puerto Serie "SUBIENDO AL P3."
					delay_ms(100);
					printf(" ");							//Que escriba y limpie continuamente
				}
			}
			if ((puls4==1)||(dato_serie==4))				//Si pulso el 4 o dato_serie=4
			{
				delay_ms(200);
				printf(" ");								//Limpia lo que haya en el Puerto Serie
				output_bit(PIN_C4,1);
				subir();									//Subir.
				while (detect4==0)							//Mientras que no detecte el 4.
				{
					printf("SUBIENDO AL P4.\n");			//Envía por el Puerto Serie "SUBIENDO AL P4."
					delay_ms(100);
					printf(" ");							//Que escriba y limpie continuamente.
				}
			}
			if ((puls5==1)||(dato_serie==5))				//Si pulso el 5 o dato_serie=5
			{
				delay_ms(200);
				printf(" ");								//Limpia lo que haya en el puerto serie.
				output_bit(PIN_C5,1);
				subir();									//Subir
				
				while (detect5==0)							//Mientras que no detecte el 5.
				{
					printf("SUBIENDO AL P5.\n");			//Envía por el Puerto Serie "SUBIENDO AL P5.
					delay_ms(100);
					printf(" ");							//Que escriba y limpie continuamente.
				}		
			}
			output_c(0);									//Los leds del Puerto C a 0.
			parar();										//Parar.
			dato_serie=7;									//Darle valor desconocido a dato_serie.
			printf(" ");									//Limpiar el label del Puerto Serie.
		}


		if (detect1==1)										//Si esta detectando el 1.
		{
			parar();
			delay_ms(200);
			printf("Piso 1.\n");
			delay_ms(100);
			printf(" ");
			if ((puls0==1)||(dato_serie==0))									
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C0,1);
				bajar();
				while (detect0==0)
				{
					printf("BAJANDO AL P0.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls1==1)||(dato_serie==1))				
			{
				delay_ms(200);
				output_bit(PIN_C1,1);
				parar();
				dato_serie=7;
			}
			if ((puls2==1)||(dato_serie==2))				
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C2,1);
				subir();
				while (detect2==0)
				{
					printf("SUBIENDO AL P2.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls3==1)||(dato_serie==3))			
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C3,1);
				subir();
				while (detect3==0)
				{
					printf("SUBIENDO AL P3.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls4==1)||(dato_serie==4))				
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C4,1);
				subir();
				while (detect4==0)
				{
					printf("SUBIENDO AL P4.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls5==1)||(dato_serie==5))				
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C5,1);
				subir();
				while (detect5==0)
				{
					printf("SUBIENDO AL P5.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			output_C(0);
			parar();
			dato_serie=7;
			printf(" ");
		}


		if (detect2==1)										//Si esta detectando el 2.
		{
			parar();
			delay_ms(200);
			printf("Piso 2.\n");
			delay_ms(100);
			printf(" ");
	
			if ((puls0==1)||(dato_serie==0))				
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C0,1);
				bajar();
				while (detect0==0)
				{
					printf("BAJANDO AL P0\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls1==1)||(dato_serie==1))				
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C1,1);
				bajar();
				while(detect1==0)
				{
					printf("BAJANDO AL P1.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls2==1)||(dato_serie==2))			
			{
				delay_ms(200);
				output_bit(PIN_C2,1);
				parar();
				dato_serie=7;
			}
			if ((puls3==1)||(dato_serie==3))				
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C3,1);
				subir();
				while (detect3==0)
				{
					printf("SUBIENDO AL P3.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls4==1)||(dato_serie==4))				
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C4,1);
				subir();
				while (detect4==0)
				{
					printf("SUBIENDO AL P4.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls5==1)||(dato_serie==5))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C5,1);
				subir();
				while (detect5==0)
				{
					printf("SUBIENDO AL P5.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			output_C(0);
			parar();
			dato_serie=7;
			printf(" ");
		}
		
		
		if (detect3==1)								//Si está detectando el 3.
		{
			parar();
			delay_ms(200);
			printf("Piso 3.\n");
			delay_ms(100);
			printf(" ");
			
			if ((puls0==1)||(dato_serie==0))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C0,1);
				bajar();
				while (detect0==0)
				{
					printf("BAJANDO AL P0.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls1==1)||(dato_serie==1))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C1,1);
				bajar();
				while(detect1==0)
				{
					printf("BAJANDO AL P1.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls2==1)||(dato_serie==2))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C2,1);
				bajar();
				while(detect2==0)
				{
					printf("BAJANDO AL P2\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls3==1)||(dato_serie==3))
			{
				delay_ms(200);
				output_bit(PIN_C3,1);
				parar();
				dato_serie=7;
			}
			if ((puls4==1)||(dato_serie==4))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C4,1);
				subir();
				while (detect4==0)
				{
					printf("SUBIENDO AL P4.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls5==1)||(dato_serie==5))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C5,1);
				subir();
				while (detect5==0)
				{
					printf("SUBIENDO AL P5.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			output_C(0);
			parar();
			dato_serie=7;
			printf(" ");
		}


		if (detect4==1)									//Si está detectando el 4.
		{
			parar();
			delay_ms(200);
			printf("Piso 4.\n");
			delay_ms(100);
			printf(" ");
	
			if ((puls0==1)||(dato_serie==0))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C0,1);
				bajar();
				while (detect0==0)
				{
					printf("BAJANDO AL P0.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls1==1)||(dato_serie==1))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C1,1);
				bajar();
				while(detect1==0)
				{
					printf("BAJANDO AL P1.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls2==1)||(dato_serie==2))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C2,1);
				bajar();
				while(detect2==0)
				{
					printf("BAJANDO AL P2./n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls3==1)||(dato_serie==3))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C3,1);
				bajar();
				while(detect3==0)
				{
					printf("BAJANDO AL P3.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls4==1)||(dato_serie==4))
			{
				delay_ms(200);
				output_bit(PIN_C4,1);
				parar();
				dato_serie=7;
			}
			if ((puls5==1)||(dato_serie==5))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C5,1);
				subir();
				while (detect5==0)
				{
					printf("SUBIENDO AL P5.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			output_C(0);
			parar();
			dato_serie=7;
			printf(" ");
		}


		if (detect5==1)									//Si está detectando el 5.
		{
			parar();
			delay_ms(200);
			printf("Piso 5.\n");
			delay_ms(100);
			printf(" ");

			if ((puls0==1)||(dato_serie==0))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C0,1);
				bajar();
				while (detect0==0)
				{
					printf("BAJANDO AL P0.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls1==1)||(dato_serie==1))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C1,1);
				bajar();
				while(detect1==0)
				{
					printf("BAJANDO AL P1.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls2==1)||(dato_serie==2))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C2,1);
				bajar();
				while(detect2==0)
				{
					printf("BAJANDO AL P2.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls3==1)||(dato_serie==3))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C3,1);
				bajar();
				while(detect3==0)
				{
					printf("BAJANDO AL P3.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls4==1)||(dato_serie==4))
			{
				delay_ms(200);
				printf(" ");
				output_bit(PIN_C4,1);
				bajar();
				while(detect4==0)
				{
					printf("BAJANDO AL P4.\n");
					delay_ms(100);
					printf(" ");
				}
			}
			if ((puls5==1)||(dato_serie==5))
			{
				delay_ms(200);
				output_bit(PIN_C5,1);
				parar();
				dato_serie=7;
			}
			output_C(0);
			parar();
			dato_serie=7;
			printf(" ");
		}
		
	}			
}										


void subir (void)
{
	output_bit(PIN_D0, 1);  	       
	output_bit(PIN_D1, 0);

	return;
}

void parar (void)
{
	output_bit(PIN_D0, 0);  	       
	output_bit(PIN_D1, 0);
	
	return;
}
void bajar (void)
{
	output_bit(PIN_D0, 0);  	       
	output_bit(PIN_D1, 1);
	
	return;
}