/*
  Centro de Innovación e Integración de Tecnologías Avanzadas
  CIITA Unidad Veracruz
  Fecha: 17 de abril de 2023
  
  Codigo para probar el sensor de Temperatura DS18B20

  Elaborado por:
    Antonio del Angel Moreno Carbajal
    Maria Isabel Cruz Solis
*/

#include <OneWire.h>
#include <DallasTemperature.h>
 
// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 32;
 
// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);
 
void setup() {
    // Iniciamos la comunicación serie
    Serial.begin(9600);
    // Iniciamos el bus 1-Wire
    sensorDS18B20.begin(); 
}
 
void loop() {
    // Mandamos comandos para toma de temperatura a los sensores
    sensorDS18B20.requestTemperatures();
 
    // Leemos y mostramos los datos de los sensores DS18B20
    Serial.print("Temperatura sensor 0: ");
    Serial.print(sensorDS18B20.getTempCByIndex(0));
    Serial.println(" C");
    
    delay(1000); 
}
