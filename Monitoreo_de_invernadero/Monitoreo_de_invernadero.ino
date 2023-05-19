/*
 * Centro de Innovación e Integración de Tecnologías Avanzadas
 * Unidad Veracruz
 * Departamento de electrónica
 * 
 * Elaborado por:
 *  Maria Isabel Cruz Solis               Residente del departamento
 * 
 * Fecha:
 *  19/05/2023
 * 
 * Descripción:
 *  El siguiente sistema realiza el monitoreo de un tunel del invernadero tecnificado ubicado en el CIITA Veracruz 
 *  
 * Componentes:
 *  Tarjeta de desarroollo ESP32
 *  Sensor de humedad capacitivo
 *  Sensor de radiación UV
 *  Sensor de temperatura DHT22
 *  Sensor de temperatura DS18B20
 *
 * Tipo de conexión:
 *  Inalambrica (Wi-FI)
 */

// Constastes
#define DHTPIN 33                                                   // Conexión al pin 25 del esp32
#define DHTTYPE DHT22                                               // Sensor de temperatura y humedad DHT22
#define sensor_hum 35                                               // Sensor de humedad capacitivo
#define sensor_temp 32                                              // Sensor de temperatura DS18B20
#define sensor_radiacion 34                                         // Sensor de radiación UV
#define LED 2                                                       // LED de la esp32

// Bibliotecas a utilizar
#include <WiFi.h>                                                   // Biblioteca para el control de WiFi
#include <PubSubClient.h>                                           // Biblioteca para conexion MQTT
#include "DHT.h"                                                    // Biblioteca para usar el sensor DHT22
#include <OneWire.h>                                                // Biblioteca para usar el DS18B20
#include <DallasTemperature.h>                                      // Biblioteca para usar el DS18B20

// Objetos del programa
WiFiClient espClient;                                               // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient);                                     // Este objeto maneja los datos de conexion al broker
DHT dht(DHTPIN,DHTTYPE);                                            // Este objeto maneja el sensor DHT22
OneWire oneWireObjeto(sensor_temp);                                 // Este objeto maneja el sensor DS18B20
DallasTemperature sensorDS18B20(&oneWireObjeto);                    // Este objeto maneja el sensor DS18B20

// Datos de WiFi a conectar
const char* ssid = "";                                              // Aquí se debe poner el nombre de la red
const char* password = "";                                          // Aquí se debe poner la contraseña de la red

// Datos de la conexión a broker MQTT
const char* mqtt_server = "";                                       // Colocar IP del servidor (computadora) ejemplo: "192.168.137.1" 
IPAddress server ();                                                // Colocar IP del servidor (computadora) ejemplo: (192,168,137,1)

// Variables del sensor de radiacion
float Sensorvalor;
float Sensorvoltaje;
int indice_radiacion;

// Variables del sensor de humedad
const int aire_sensor = 4095;
const int agua_sensor = 2981;
int sensor;
int hum_capacitivo;

// Variable de sensor de temperatura DS18B20
float temp_ds18b20;

// Variables del sensor DHT22
float hum_dht22; 
float temp_dht22;

// Variables del programa con valor definido
int flashLedPin = 4;                                                // Para indicar el estatus de conexión
int statusLedPin = 3;                                               // Para ser controlado por MQTT
long timeNow, timeLast;                                             // Variables de control de tiempo no bloqueante
int data = 0;                                                       // Contador
int wait = 5000;                                                    // Indica la espera cada 5 segundos para envío de mensajes MQTT
int t;

// Variable a enviar a Node-RED
String json;                                                        // Variable que almacena la cadena de caracteres en formato JSON

// Prototipo de funciones
void funcion_humedad();
void funcion_radiacion();
void funcion_dht22();
void funcion_temperaturawire();

void setup() {
  // Definiendo entradas y salidas del microcontrolador
  pinMode(LED,OUTPUT);                                            // LED Parpadeante de la placa
  pinMode(sensor_hum,INPUT);                                     // Sensor de humedad capacitivo
  pinMode(sensor_temp,INPUT);                                     // Sensor de temperatura ds18b20

  dht.begin();                                                    // Iniciando el DHT22
  Serial.begin(115200);                                           // Iniciando el puerto serial de la ESP32
  sensorDS18B20.begin();
  
  Serial.println();                                               // Imprimiendo en puerto serial
  Serial.println();                                               // la conexión al WiFi
  Serial.print("Conectar a ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);                                     // Esta es la función que realiza la conexión a WiFi

  while (WiFi.status() != WL_CONNECTED) {                         // Este bucle espera a que se realice la conexión
    digitalWrite (statusLedPin, HIGH);
    delay(500); 
    digitalWrite (statusLedPin, LOW);
    Serial.print(".");  
    delay (5);
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());

  if (WiFi.status () > 0){
    digitalWrite (statusLedPin, LOW);
  }
  
  delay (1000);                                                   // Esta espera es solo una formalidad antes de iniciar la comunicación con el broker

  // Conexión con el broker MQTT
  client.setServer(server, 1883);                                 // Conectarse a la IP del broker en el puerto indicado
  client.setCallback(callback);                                   // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);                                                    // Esta espera es preventiva, espera a la conexión para no perder información

  timeLast = millis ();                                           // Inicia el control de tiempo
}

void loop(){
  if (!client.connected()) {                                      // Verificar siempre que haya conexión al broker
    reconnect();                                                  // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }
  
  client.loop();                                                  // Esta función es muy importante, ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker
  
  timeNow = millis();                                             // Control de tiempo para esperas no bloqueantes
  
  if (timeNow - timeLast > wait) {
    timeLast = timeNow;                                           // Actualización de seguimiento de tiempo
    
    // Llamando a las funciones de los sensores
    funcion_humedad();
    funcion_radiacion();
    funcion_dht22();
    funcion_temperaturawire();

    // Se construye el string en formato JSON
    json = "{\"temperatura_ds18b20\":\""+String(temp_ds18b20)+"\""+
           ",\"humedad_2\":\""+String(hum_capacitivo)+"\""+
           ",\"radiacionUV\":\""+String(indice_radiacion)+"\""+
           ",\"humedad_DHT22\":\""+String(hum_dht22)+"\""+
           ",\"temperatura_DHT22\":\""+String(temp_dht22)+"\"}";

    Serial.println(json);                                         // Se imprime en monitor solo para poder visualizar que el string esta correctamente creado
    int str_len = json.length() + 1;                              // Se calcula la longitud del string
    char char_array[str_len];                                     // Se crea un arreglo de caracteres de dicha longitud
    json.toCharArray(char_array, str_len);                        // Se convierte el string a char array    
    client.publish("CIITA/VERACRUZ/MONITOREO", char_array);       // Esta es la función que envía los datos por MQTT, especifica el tema y el valor
    delay (100);
  }
}

// Esta función permite tomar acciones en caso de que se reciba un mensaje correspondiente a un tema al cual se hará una suscripción
void callback(char* topic, byte* message, unsigned int length) {
  // Indicar por serial que llegó un mensaje
  Serial.print("Llegó un mensaje en el tema: ");
  Serial.print(topic);

  // Concatenar los mensajes recibidos para conformarlos como una varialbe String
  String messageTemp; // Se declara la variable en la cual se generará el mensaje completo  
  for (int i = 0; i < length; i++) {  // Se imprime y concatena el mensaje
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);

  // En esta parte puedes agregar las funciones que requieras para actuar segun lo necesites al recibir un mensaje MQTT
  
  // Ejemplo, en caso de recibir el mensaje true - false, se cambiará el estado del led soldado en la placa.
  // El ESP323CAM está suscrito al tema esp/output
  if (String(topic) == "CIITA/VERACRUZ") {  // En caso de recibirse mensaje en el tema esp32/output
    if(messageTemp == "true"){
      Serial.println("Led encendido");
      digitalWrite(flashLedPin, HIGH);
  }
  else if(messageTemp == "false"){
     Serial.println("Led apagado");
     digitalWrite(flashLedPin, LOW);
    }
  }
}

// Función para reconectarse
void reconnect() {
  // Bucle hasta lograr conexión
  while (!client.connected()) { // Pregunta si hay conexión
    Serial.print("Tratando de contectarse...");
    // Intentar reconexión
    if (client.connect("ESP32CAMClient")) { //Pregunta por el resultado del intento de conexión
      Serial.println("Conectado");
      client.subscribe("codigoIoT/G6/led"); // Esta función realiza la suscripción al tema
    }
    else {  //en caso de que la conexión no se logre
      Serial.print("Conexion fallida, Error rc=");
      Serial.print(client.state()); // Muestra el codigo de error
      Serial.println(" Volviendo a intentar en 5 segundos");
      // Espera de 5 segundos bloqueante
      delay(5000);
      Serial.println (client.connected ()); // Muestra estatus de conexión
    }
  }
}

void funcion_humedad() {
  sensor = analogRead(sensor_hum);
  
  if ((sensor >= agua_sensor) && (sensor <= aire_sensor)){
    hum_capacitivo = map(sensor,aire_sensor,agua_sensor,0,100);
  }
}

void funcion_radiacion() {
  Sensorvalor = analogRead(sensor_radiacion);
  Sensorvoltaje = (Sensorvalor/1023)*5;
  indice_radiacion = (Sensorvoltaje*11)/5;
}

void funcion_dht22() {
  hum_dht22 = dht.readHumidity(); 
  temp_dht22 = dht.readTemperature();
}

void funcion_temperaturawire() {
  sensorDS18B20.requestTemperatures();
  temp_ds18b20 = sensorDS18B20.getTempCByIndex(0);
}