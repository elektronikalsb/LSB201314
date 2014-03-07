#include <SPI.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Xively.h>
#include <Adafruit_BMP085.h>              //Librerias necesarias para el funcionamiento
#include <Wire.h>
#include <DHT.h>
#define DHTPIN 5                     //Definir el pin digital al que va conectado el sensor DHT22
#define DHTTYPE DHT22                //Definir que sensor DHTxx es el nuestro (DHT22)


int anemo = 2;
int i = 0;
int vueltas = 0;
int velocidad = 0;                  //Definir variables
int luz = 3;
int lluvia = 2;
int aire = 1;

DHT dht(DHTPIN, DHTTYPE);
 
Adafruit_BMP085 bmp;

char ssid[] = "lsband";      //  Nombre de la red Wifi 
char pass[] = "3955445f7168413a3d7b20533c635d68";     // Contraseña de la red Wifi, solo para WPA


int status = WL_IDLE_STATUS; 


char xivelyKey[] = "pPdLKtYcIz7o2auxMN0w0n9gVh3pWXgzhIbApt3PH46Q5vtO"; // Clave API generada por XIVELY para nuestro dispositivo


char LLUVIAId[] = "LLUVIA";
char AIREId[] = "CONTAMINACION";
char DHThumedadId[] = "HUMEDAD";
char DHTtemperaturaId[] = "TEMPERATURA";        // Definir los datos que queremos enviar
char PRESIONId[] = "PRESION";
char ANEMOMETROId[] = "VIENTO";
char LUZId[] = "LUMINOSIDAD";

const int bufferSize = 14;      //Largura del texto que queremos enviar a Xively
char bufferValue[bufferSize];     

  XivelyDatastream datastreams[] = {
  XivelyDatastream(LLUVIAId, strlen(LLUVIAId), DATASTREAM_BUFFER, bufferValue, bufferSize),
  XivelyDatastream(AIREId, strlen(AIREId), DATASTREAM_FLOAT),
  XivelyDatastream(DHThumedadId, strlen(DHThumedadId), DATASTREAM_FLOAT),
  XivelyDatastream(DHTtemperaturaId, strlen(DHTtemperaturaId), DATASTREAM_FLOAT),              //Definir el formato de los datastreams
  XivelyDatastream(PRESIONId, strlen(PRESIONId), DATASTREAM_FLOAT),
  XivelyDatastream(ANEMOMETROId, strlen(ANEMOMETROId), DATASTREAM_FLOAT),
  XivelyDatastream(LUZId, strlen(LUZId), DATASTREAM_FLOAT),
};

XivelyFeed feed(1292605205, datastreams, 7);          //Numero de Identificacion de nuestro dispositivo en Xively y el numero de datastreams que vamos a enviar

WiFiClient client;
XivelyClient xivelyclient(client);

void printWifiStatus() {

  Serial.print("Red: ");
  Serial.println(WiFi.SSID());      //Imprimir en pantalla la SSID de la red a la que nos hemos conectado

  IPAddress ip = WiFi.localIP();
  Serial.print("IP: ");              //Imprimir en pantalla la IP del Wifi Shield
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Potencia de la red (RSSI):");        //Imprimir en pantalla la fuerza de la señal wifi
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {

  Serial.begin(9600);                            //Inicializamos Arduino
  if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");          //Checkeamos si el sensor BMP esta conectado antes de inicializarlo
	while (1) {}
  }
  dht.begin();                                    //Inicializamos el sensor DHT
  Serial.println("Iniciando subida de datos a Xively...");
  Serial.println();

  while ( status != WL_CONNECTED) {                     //Intentamos conectarnos a la red especificada
    Serial.print("Intentando conectarse a la red especificada: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);          //Introducimos los datos de la red wifi a la que deseamos conectarnos
    delay(10000);
  } 
  Serial.println("Conectado a la red WIFI");
  printWifiStatus();                                //Si nos hemos conectado satisfactoriamente, nos mostrara la Ip, SSID y la fuerza de la señal
  pinMode(anemo, INPUT);                            //Definimos el anemometro como entrada de datos
  attachInterrupt(0, parpadeo, RISING);             //Definir interrupcion para resolver los problemas de lectura del anemometro
}

void loop() {
  
  Serial.println("Datos recogidos mediante los sensores de INOMET");
  
  int valorlluvia = analogRead(lluvia);            //Lectura del sensor de lluvia 
  if(valorlluvia>=950)
  {
  datastreams[0].setBuffer("NO");
  Serial.print("LLUVIA  ");                            //Si la lectura es mayor a 950, el ambiente esta seco (lluvia NO)
  Serial.println(datastreams[0].getBuffer());
  }
  
  if(valorlluvia<950)
  {
  datastreams[0].setBuffer("SI");
  Serial.print("LLUVIA  ");                            //Si la lectura es menos a 950, esta lloviendo (lluvia SI)
  Serial.println(datastreams[0].getBuffer());
  }

  int valoraire = analogRead(aire);              //Lectura del sensor de calidad de aire (MQ-135) 
  int calidad = valoraire; 
  valoraire=(valoraire/10.23);                     //Calculo para dar la calidad en porcentaje
  datastreams[1].setFloat(valoraire);
  Serial.print("AIRE  ");
  Serial.print(datastreams[1].getFloat());
  Serial.print("     ");
  Serial.println(calidad);

  int valorhumedad = dht.readHumidity();        //Lectura del sensor de humedad (DHT22)
  datastreams[2].setFloat(valorhumedad);
  Serial.print("DHT humedad  ");
  Serial.println(datastreams[2].getFloat());
    
  int valortemperatura = dht.readTemperature();        //Lectura de temperatura (DHT22) 
  datastreams[3].setFloat(valortemperatura);
  Serial.print("DHT temperatura  ");
  Serial.println(datastreams[3].getFloat());
  
  int valorpresion = (bmp.readPressure()/100);        //Lectura del sensor de presión atmosférica (BMP-085)
  datastreams[4].setFloat(valorpresion);
  Serial.print("PRESION  ");
  Serial.println(datastreams[4].getFloat());


  vueltas=i/2;                                            //Ajuste del anemómetro
  velocidad=((vueltas*0.09106)*3.6);  
  datastreams[5].setFloat(velocidad);
  Serial.print("ANEMOMETRO  ");
  Serial.println(datastreams[5].getFloat());
  i=0;
  vueltas=0;
  velocidad=0;

  int valorluz = analogRead(luz);                    //Lectura del sensor de luminosidad (Fototransistor)
  int luminosidad = valorluz;
  valorluz=(valorluz/10.23);                         //Calculo para dar la luminosidad en porcentaje
  datastreams[6].setFloat(valorluz);
  Serial.print("LUMINOSIDAD  ");
  Serial.print(datastreams[6].getFloat());
  Serial.print("     ");
  Serial.println(luminosidad);

  Serial.println("Subiendo los datos a Xyvely");
  int ret = xivelyclient.put(feed, xivelyKey);        //Checkeamos si hemos enviado correctamente los datos a Xively
  Serial.print("HTTP: ");
  Serial.println(ret);

  Serial.println();
  delay(15000);
}

void parpadeo()
{
  i++;                                          //Contador para las vueltas del anemometro
} 
