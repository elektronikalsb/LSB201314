#include<16f88.h>

#fuses INTRC_IO, NOWDT, NOLVP, NOCPD, NOWRT, NODEBUG, NOPROTECT, NOFCMEN, NOIESO, NOMCLR, NOBROWNOUT, CCPB3 

#device ADC=10

#use delay(clock=4000000)
#use fast_io(b)
#use fast_io(a)

#byte trisa=0x85
#byte porta=0x05
#byte trisb=0x86
#byte portb=0x06

void main(void)
{

	int16 sensorder1;  
	int16 sensorder2;  
	int16 sensorizq1;  
	int16 sensorizq2; 
	trisa=0xff;				//Entradas Port A
	trisb=0x00;				//Salidas Port B
	
	setup_oscillator(OSC_4MHZ|OSC_INTRC); //oscialdor interno a 4MHZ
   
    setup_adc(ADC_CLOCK_INTERNAL);   //Configura el CAD
	setup_adc_ports(sAN0);			 //A0
	setup_adc_ports(sAN1);			 //A1
 	setup_adc_ports(sAN2);			 //A2
	setup_adc_ports(sAN3);			 //A3
    set_adc_channel(0);              //Especifica el canal a utilizar 0
	set_adc_channel(1);              //Especifica el canal a utilizar 1
	set_adc_channel(2);              //Especifica el canal a utilizar 2 
	set_adc_channel(3);              //Especifica el canal a utilizar 3
	delay_us(20);


	while(1)
	{
	set_adc_channel(0);
	delay_us(20);
	sensorizq1 = read_adc();

	set_adc_channel(1); 
	delay_us(20);
	sensorizq2 = read_adc();

	set_adc_channel(2); 
	delay_us(20);
	sensorder1 = read_adc();

	set_adc_channel(3); 
	delay_us(20);
	sensorder2 = read_adc();
	

	if((sensorizq2<=400)||(sensorder1<=400))     
	{
	output_high(PIN_B0);													
	output_low(PIN_B1);
	output_low(PIN_B4);
	output_high(PIN_B5);
	}
	else
	{
		if((sensorizq1<=400)&&(sensorizq2<=400)&&(sensorder1>=900)&&(sensorder2>=900))	 
		{
		output_low(PIN_B0);															
		output_low(PIN_B1);
		output_low(PIN_B4);
		output_high(PIN_B5);
		}		
		if((sensorizq1>=900)&&(sensorizq2>=900)&&(sensorder1<=400)&&(sensorder2<=400))	 
		{
		output_high(PIN_B0);															
		output_low(PIN_B1);
		output_low(PIN_B4);
		output_low(PIN_B5);
		}
		if((sensorizq1<=400)&&(sensorizq2>=900)&&(sensorder1>=900)&&(sensorder2>=900))	 
		{
		output_low(PIN_B0);															
		output_high(PIN_B1);
		output_low(PIN_B4);
		output_high(PIN_B5);
		}
		if((sensorizq1>=900)&&(sensorizq2>=900)&&(sensorder1>=900)&&(sensorder2<=400))	  
		{
		output_high(PIN_B0);																	
		output_low(PIN_B1);
		output_high(PIN_B4);
		output_low(PIN_B5);
		}
	}
	if((sensorizq1>=900)&&(sensorizq2>=900)&&(sensorder1>=900)&&(sensorder2>=900))      
	{
	output_low(PIN_B0);
	output_low(PIN_B1);
	output_low(PIN_B4);
	output_low(PIN_B5);
	}
	}
}