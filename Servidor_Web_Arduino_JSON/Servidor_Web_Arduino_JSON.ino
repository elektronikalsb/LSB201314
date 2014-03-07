#include <SPI.h>
#include <WiFi.h>
#include <Adafruit_BMP085.h>      // Incluir las librerias que necesitemos
#include <Wire.h>
#include <DHT.h>
 
#define DHTPIN 5           //  Pin del sensor
#define DHTTYPE DHT22      //  Modelo de sensor DHT

int luz;
int sensormq;
int sensorlluvia;
int anemo = 2;
int i = 0;                //Definicion de los sensores con sus variables
int vueltas = 0;
int velocidad = 0;

DHT dht(DHTPIN, DHTTYPE);    // configuracion del DHT
 
Adafruit_BMP085 bmp;
 

char ssid[] = "lsband";      //  Nombre de la red Wifi 
char pass[] = "3955445f7168413a3d7b20533c635d68";     // Contraseña de la red Wifi, solo para WPA

int status = WL_IDLE_STATUS;    

WiFiServer server(80);   // Puerto protocolo de transferencia HTTP servidor web (por defecto 80)

void setup() {
  Serial.begin(9600);   //Inicializar la terminal
  if (!bmp.begin()) {   //Inicializar sensor BMP 085
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  dht.begin();    //Inicializar sensor DHT22
  if (WiFi.status() == WL_NO_SHIELD) {   //Chequeo de la conexion del modulo wifi al Arduino
    Serial.println("WiFi shield not present"); 
    while(true);
  } 
  
  while ( status != WL_CONNECTED) {   //Conexion del modulo wifi al SSID
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);  //Ingresa en la red wifi con el usuario y contraseña indicada
    delay(10000);
    
  } 
  server.begin();   //Inicializar el servidor web
  pinMode(anemo, INPUT);  //Declarar el anemometro como entrada
  attachInterrupt(0, parpadeo, RISING); //Funcion para la lectura de impulsos del anemometro
  printWifiStatus(); //Funcion que muestra en la terminal los datos de la conexion; IP, alcance señal, etc..
}


void loop() {
  json();
}


void printWifiStatus() {    //Bloque para mostrar los datos de la conexion Wifi
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Muestra la IP
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Muestra la potencia de la señal
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void parpadeo()  //Funcion auxiliar para el anemometro
{
  i++;
} 

void json()
{
  WiFiClient client = server.available(); 
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {    //Formato de la web a la hora de enviar los datos
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/json");
          client.println("Connnection: close");
          client.println();
          
          float h = dht.readHumidity();     //Declarar variable de humedad del DHT22
          float t = dht.readTemperature();  //Declarar variable de temperatura del DHT22
            
          client.println("{");                         // {
          client.print("\"Humedad\":");
          client.print("\"");          
          client.print(h);                             // "Humedad": "****",
          client.print("\"");
          client.println(",");         
          client.print("\"Temperatura\":");
          client.print("\"");
          client.print(t);                             //"Temperatura": "****",
          client.print("\"");
          client.println(",");                                                       //Realizamos el JSON mediante los datos que hemos recibido desde nuestros sensores
          client.print("\"Presion\":");
          client.print("\"");
          client.print(bmp.readPressure()/100);        //"Presion": "****",
          client.print("\"");
          client.println(","); 
          client.print("\"Lluvia\":"); 
          sensorlluvia = analogRead(2);  
          client.print("\"");          
          sensorlluvia = (sensorlluvia/1000);          //"Lluvia": "****",
          client.print(sensorlluvia);        
          client.print("\"");
          client.println(",");
          vueltas=i/2;
          velocidad=((vueltas*0.09106)*3.6);
          client.print("\"Viento\":");
          client.print("\"");          
          client.print(velocidad);                    //"Viento": "****",
          client.print("\"");
          client.println(",");
          i=0;
          vueltas=0;
          velocidad=0;
          client.print("\"Aire\":");   
          client.print("\"");
          sensormq = analogRead(1);
          sensormq = (sensormq/10.23);                //"Aire": "****",
          client.print(sensormq);
          client.print("\"");
          client.println(",");
          client.print("\"Luminosidad\":");   
          client.print("\"");
          luz = analogRead(3); 
          luz = (luz/10.23);                          //"Luminosidad": "****"
          client.print(luz);
          client.println("\"");
          client.println("}");                        //}
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1); //Tiempo para que el navegador web reciva los datos
      client.stop();    //Cerrar conexión
      Serial.println("client disonnected");
  }
}
