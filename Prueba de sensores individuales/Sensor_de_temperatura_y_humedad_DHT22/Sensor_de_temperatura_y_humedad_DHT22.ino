#include "DHT.h"
 
#define DHTPIN 33                                         // Definimos el pin digital donde se conecta el sensor
#define DHTTYPE DHT22                                     // Tipo de sensor

DHT dht(DHTPIN, DHTTYPE);                                 // Inicializamos el sensor DHT22

void setup() {
  Serial.begin(115200);                                   // Inicializamos comunicación serie
  dht.begin();                                            // Comenzamos el sensor DHT
}

void loop() {
  delay(5000);
  float h = dht.readHumidity();                           // Obtención de la humedad relativa
  float t = dht.readTemperature();                        // Obtención de la temperatura en grados centígrados

  // Mostrar datos por puerto serie
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C ");
}
