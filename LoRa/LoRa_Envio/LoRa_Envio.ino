#define DHTPIN 32                                                   // Conexión al pin 32 del esp32
#define DHTTYPE DHT22                                               // Sensor de temperatura y humedad DHT22
#define sensor_hum 35                                               // Sensor de humedad capacitivo
#define sensor_temp 33                                              // Sensor de temperatura DS18B20
#define sensor_radiacion 34                                         // Sensor de radiación UV
#define LED 2
#define BAND 433E6                                                  // 433E6 for Asia, 866E6 for Europe, 915E6 for North America
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128                                            // OLED display width, in pixels
#define SCREEN_HEIGHT 32                                            // OLED display height, in pixels 32
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"                                                    // Biblioteca para usar el sensor DHT22
#include <OneWire.h>                                                // Biblioteca para usar el DS18B20
#include <DallasTemperature.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Variables del sensor de radiacion
float Sensorvalor = 0.0;
float Sensorvoltaje = 0.0;
int indice_radiacion;

// Variables del sensor de humedad
const int aire_sensor = 4095;
const int agua_sensor = 2981;
int sensor;
int hum_capacitivo;

// Variable de sensor de temperatura DS18B20
float temp_ds18b20 = 0.0;

// Variables del sensor DHT22
float hum_dht22 = 0.0; 
float temp_dht22 = 0.0;

// Variables del programa con valor definido
int flashLedPin = 4;                                                // Para indicar el estatus de conexión
int statusLedPin = 3;                                               // Para ser controlado por MQTT
long timeNow, timeLast;                                             // Variables de control de tiempo no bloqueante
int data = 0;                                                       // Contador
int wait = 5000;                                                 // Indica la espera cada hora para envío de mensajes MQTT
int t;

// Prototipo de funciones
void funcion_humedad();
void funcion_radiacion();
void funcion_dht22();
void funcion_temperaturawire();

// Variable a enviar a Node-RED
String json;                                                        // Variable que almacena la cadena de caracteres en formato JSON

Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
DHT dht(DHTPIN,DHTTYPE);                                            // Este objeto maneja el sensor DHT22
OneWire oneWireObjeto(sensor_temp);                                 // Este objeto maneja el sensor DS18B20
DallasTemperature sensorDS18B20(&oneWireObjeto);                    // Este objeto maneja el sensor DS18B20

void setup() {
  Serial.begin(115200);
  pinMode(sensor_hum,INPUT);                                        // Sensor de humedad capacitivo
  pinMode(sensor_temp,INPUT);                                       // Sensor de temperatura ds18b20
  
  sensorDS18B20.begin();                                            // Iniciando el sensor de temperatura DS18B20
  dht.begin();                                                      // Iniciando el DHT22
  Serial.println("Prueba de envio LoRa");
  
  //initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Inicializacion OK!");
  display.setCursor(0,10);
  display.print("Inicializacion OK!");
  display.display();
  delay(2000);

  timeLast = millis ();                                           // Inicia el control de tiempo
}

void loop() {
  timeNow = millis();                                             // Control de tiempo para esperas no bloqueantes
  
  if (timeNow - timeLast > wait) {
    timeLast = timeNow;                                           // Actualización de seguimiento de tiempo
    // Llamando a las funciones que controlan a los sensores del sistema
    funcion_humedad();
    funcion_radiacion();
    funcion_dht22();
    funcion_temperaturawire();

    // Se construye el string con formato JSON para enviar la información por protocolo MQTT
    json = "{\"temperatura_ds18b20\":\""+String(0)+"\""+
           ",\"humedad_2\":\""+String(1)+"\""+
           ",\"radiacionUV\":\""+String(2)+"\""+
           ",\"humedad_DHT22\":\""+String(3)+"\""+
           ",\"temperatura_DHT22\":\""+String(4)+"\"}";
    
    /*json = "{\"temperatura_ds18b20\":\""+String(temp_ds18b20)+"\""+
           ",\"humedad_2\":\""+String(hum_capacitivo)+"\""+
           ",\"radiacionUV\":\""+String(indice_radiacion)+"\""+
           ",\"humedad_DHT22\":\""+String(hum_dht22)+"\""+
           ",\"temperatura_DHT22\":\""+String(temp_dht22)+"\"}";*/

    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print(json);
    LoRa.endPacket();
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(json);      
  display.display();
}

/*
* Funciones del programa que controlan los sensores.
* En esta sección puede agregar las funciones que necesite para ampliar el numero de sensores 
* o actuadores involucrados en el sistema
*/

// Función para leer la humedad en la cama de cultivo de tomate
void funcion_humedad() {
  sensor = analogRead(sensor_hum);                                // Lectura del sensor
  
  if ((sensor >= agua_sensor) && (sensor <= aire_sensor)){ 
    hum_capacitivo = map(sensor,aire_sensor,agua_sensor,0,100);   // Mapear la lectura del sensor
  }
}

// Función para detectar la radiación solar
void funcion_radiacion() {
  Sensorvalor = analogRead(sensor_radiacion);                     // Lectura del sensor
  Sensorvoltaje = (Sensorvalor/1023)*5;                           // Conversión a voltaje
  indice_radiacion = (Sensorvoltaje*11)/5;                        // Conversión a escala del 0 al 11
}

// Función para leer la temperatura y humedad al interior del invernadero
void funcion_dht22() {
  hum_dht22 = dht.readHumidity();                                 // Obtener lectura de humedad 
  temp_dht22 = dht.readTemperature();                             // Obtener lectura de temperatura
}

// Función para obtener la temperatura en la cama de cultivo de tomate
void funcion_temperaturawire() {
  sensorDS18B20.requestTemperatures();                            // Solicitud al sensor
  temp_ds18b20 = sensorDS18B20.getTempCByIndex(0);                // Obtener temperatura
}