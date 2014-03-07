
int sensor_der =9;  // variable sensor SRF05 derecho 
int sensor_recto = 10;  // variable sensor SRF05 delantero
int sensor_izq = 11;  // variable sensor SRF05 izquierdo
int cm_sens1;  // variable para guardar la medida recibida por el sensor derecho
int cm_sens2; // variable para guardar la medida recibida por el sensor delantero
int cm_sens3;  // varaible para guardar la medida recibida por el sensor izquierdo
int lecturaSensor1;  // variable para leer si esta en cierta medida que haga algo en concreto, sensor derecha
int lecturaSensor2;  // variable para leer si esta en cierta medida que haga algo en concreto, sensor delantero
int lecturaSensor3;  // variable para leer si esta en cierta medida que haga algo en concreto, sensor izquierda
char btdato;  
char btdato1; 

int aut_manu=2; // variable para elegir automatico o manual

int motor1pin1=8; //motor 1 adelante
int motor1pin2=12; //motor 1 atras
int enable1pin=13; // dar permiso al motor1 para que se mueva
int motor2pin1=7; // motor 2 adelante
int motor2pin2=4; // motor 2 atras
int enable2pin=5; // dar permiso al motor2 para que se mueva

long duracion;  // definir variables

void setup()  // definir variable como entradas o salidas
{
Serial.begin(9600);
 pinMode(motor1pin1, OUTPUT); // motor1 adelante, como salida
  pinMode(motor1pin2, OUTPUT);  // motor1 atras, como salida
  pinMode(enable1pin, OUTPUT);  // permiso de motor1, como salida
  pinMode(motor2pin1, OUTPUT);  // motor2 adelante, como salida
  pinMode(motor2pin2, OUTPUT);  // motor2 atras, como salida
  pinMode(enable2pin, OUTPUT);  // permiso de motor2, como salida
  digitalWrite(enable1pin, HIGH); //poner los permisos de movimiento en HIGH para que se puedan poner en marcha
  digitalWrite(enable2pin, HIGH); 
}
//--------------------------------------------------FUNCION ULTRASONIDOS---------------------------------------------------------------------
int cmsensor1(){  // medir distancia sensor SRF05 derecho
  pinMode(sensor_der, OUTPUT);  // definir como salida
  digitalWrite(sensor_der, LOW);  // apagar
  delayMicroseconds(5);  // esperar 5 segundos
  digitalWrite(sensor_der, HIGH);  // encender
  delayMicroseconds(15);  // esperar 15 segundos
  digitalWrite(sensor_der, LOW);  // apagar
  pinMode(sensor_der, INPUT);  // definir como entrada
  duracion = pulseIn(sensor_der, HIGH);  //encender y calcular lo que a tardado desde que a mandado la señal hasta que la a recibido
  cm_sens1 = msCentimetros(duracion);  // poner el valor en centimetros
  return cm_sens1;  // devolver el dato al programa principal, en este caso el de la distancia
  }
int cmsensor2(){  // medir distancia sensor SRF05 delantero
  pinMode(sensor_recto, OUTPUT);  // definir como salida
  digitalWrite(sensor_recto, LOW);  // apagar
  delayMicroseconds(5);  // esperar 5 segundos
  digitalWrite(sensor_recto, HIGH);  // encender
  delayMicroseconds(15);  // esperar 15 segundos
  digitalWrite(sensor_recto, LOW);  // apagar
  pinMode(sensor_recto, INPUT);  // definir como entrada
  duracion = pulseIn(sensor_recto, HIGH);  // encender y calcular lo que a tardado desde que a mandado la señal hasta que la a recibido
  cm_sens2 = msCentimetros(duracion);  // poner el valor en centimetros
  return cm_sens2;  // devolver el dato al programa principal, en este caso el de la distancia
  }
int cmsensor3(){  // medir distancia sensor SRF05 izquierdo
  pinMode(sensor_izq, OUTPUT);  // definir como salida
  digitalWrite(sensor_izq, LOW);  // esperar 5 segundos
  delayMicroseconds(5);  // esperar 5 segundos
  digitalWrite(sensor_izq, HIGH);  // encender
  delayMicroseconds(15);  // esperar 15 segundos
  digitalWrite(sensor_izq, LOW);  //apagar
  pinMode(sensor_izq, INPUT);  // definir como entrada
  duracion = pulseIn(sensor_izq, HIGH);  // encender y calcular lo que a tardado desde que a mandado la señal hasta que la a recibido
  cm_sens3 = msCentimetros(duracion);  // poner el valor en centimetros
  return cm_sens3;  // devolver el dato al programa principal, en este caso el de la distancia
}
long msCentimetros(long microsegundos)  // calculo que hace falta hacer para que el resultado lo de en centimetros
{
return microsegundos / 29 / 2;
}
//----------------------------------------------------FUNCION MOTORES-----------------------------------------------------------------------  
void adelante(){ // funcion para mover motor, sentido: adelante
    digitalWrite(motor1pin1,HIGH);
    digitalWrite(motor1pin2,LOW);
    digitalWrite(motor2pin1,LOW);
    digitalWrite(motor2pin2,HIGH); 
}
void izquierda(){  // funcion para mover el motor, sentido: giro a la izquierda
    digitalWrite(motor1pin1,HIGH);
    digitalWrite(motor1pin2,LOW);
    digitalWrite(motor2pin1,LOW);
    digitalWrite(motor2pin2,LOW); 
}
void derecha(){  // funcion para mover el motor, sentido: giro a la derecha
    digitalWrite(motor1pin1,LOW);
    digitalWrite(motor1pin2,LOW);
    digitalWrite(motor2pin1,LOW);
    digitalWrite(motor2pin2,HIGH); 
}
void paro(){  // funcion para mover el motor, sentido: parar
    digitalWrite(motor1pin1,LOW);
    digitalWrite(motor1pin2,LOW);
    digitalWrite(motor2pin1,LOW);
    digitalWrite(motor2pin2,LOW); 
}
void atras(){  // funcion para mover el motor, sentido: hacia atras
    digitalWrite(motor1pin1,LOW);
    digitalWrite(motor1pin2,HIGH);
    digitalWrite(motor2pin1,HIGH);
    digitalWrite(motor2pin2,LOW); 
}
//----------------------------------------------------------------MANUAL/AUTOMATICO------------------------------------------------------------
  
//...............bluetooth...............
 void manual()
 {
   if (Serial.available()>0)
   {
   btdato1=Serial.read();
   switch (btdato1)
   {
          case 'P': //paro    //si le doy al boton de STOP, al programa le llega una 'P' y para los motores
          {
              paro();
              break;
          } 
          case 'A': //adelante    // si le doy a la flecha de adelante, al programa le llega una 'A' y gira los motores para que el robot vaya haci adelante
          {  
            adelante();
            break;
          } 
          case 'D': //derecha    // si le doy a la flecha de la derecha, al programa le llega una 'D', se para un motor y el otro gira para que el robot gire a la derecha
          {
            derecha();
            break;
          }   
          case 'I': //izquierda    // si le doy a la flecha de la izquierda, al programa le llega una 'I', se para un motor y el otro girar para que el robot gire a la izquierda
          {
            izquierda();
            break;
          }
          case 'R': //atras    // si le doy a la flecha de atras, al programa le llega una 'R' y los motores giran en sentido contrario al de adelante
          {
            atras();
            break;
          } 
   }
   }
    }
   
//...............ultrasonidos............... 
 
 void automatico()
 {
 
  lecturaSensor1=cmsensor1(); // el valor que tiene la variable 'cmsensor1' es el mismo valor que el de 'lecturaSensor1'
  lecturaSensor2=cmsensor2();
  lecturaSensor3=cmsensor3();
    
        adelante();  // cuando pulse automatico, los motores van a empezar a girar
       if (lecturaSensor2<=10) // si el sensor delantero detecta que está a menos de 10cm de un obstaculo
       {
         paro();  // se paran los motores
         if((lecturaSensor1<=10) && (lecturaSensor3>10)) // mientras que el sensor delantero sigue a menor de 10cm y el sensor derecho es menor de 10cm pero el izquierdo es mayor
         {
           izquierda();  // girar a la izquierda
         }
         if ((lecturaSensor1>10) && (lecturaSensor3<=10))  // mientras que el sensor delantero sigue siendo menor de 10cm y el sensor izquierdo es menor de 10 cm pero el derecho es mayor
         {
           derecha();  // girar a la derecha
         }
         if ((lecturaSensor1<=10) && (lecturaSensor3<=10))  // mientras que el sensor delantero sigue siendo menor de 10cm y el sensor izquierdo y el derecho son menores de 10cm 
         {
           derecha();  // que gire a la derecha durante 3 segundos, para dar una vuelta de 180º
           delay(1000);
         }
       }
       
 }
 //-----------------------------------------------------------------INTERRUPCION----------------------------------------------------------------
void serialEvent() 
{
   if (Serial.available()>0)
   {
   btdato=Serial.read();
   switch (btdato)
   {
     case 'U':
     {
       aut_manu=1;
       break;
    }
     case 'M':
     {
      aut_manu=2;
      break;
     }
   }
 }
}
  
//----------------------------------------------------------------FUNCION PRINCIPAL------------------------------------------------------------
void loop()  
{  
  lecturaSensor1=cmsensor1();  // el mismo valor que tiene la variable 'cmsensor1' es la que tendra el 'lecturaSensor1'
  lecturaSensor2=cmsensor2();
  lecturaSensor3=cmsensor3();  
 if (Serial.available()>0)
   {
   btdato=Serial.read();
   switch (btdato)
   {
     case 'U': // cuando pulse el boton de automatico, el programa recibe una 'U' 
     {
       aut_manu=1; // la variable se pone a '1'
       break;
    }
     case 'M': // cuando pulse el boton de manual, el programa recibe una 'M'
    {
      aut_manu=2; // la variable se pone a '2'
      break;
    }
   }
  while(aut_manu==1) // mientras que la variable sea '1'
  {
    automatico(); // hacer la funcion de 'automatico'
  }
   while(aut_manu==2) // mientras que la variable sea '2'
  {
    manual();  // hacer la funcion de 'manual'
  }
   }
}
