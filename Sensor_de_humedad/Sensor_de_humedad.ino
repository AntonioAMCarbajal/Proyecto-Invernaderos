/*
  Centro de Innovación e Integración de Tecnologías Avanzadas
  CIITA Unidad Veracruz
  Fecha: 17 de abril de 2023
  
  Codigo para probar el sensor de Humedad Capacitivo Anticorrosivo

  Elaborado por:
    Antonio del Angel Moreno Carbajal
    Maria Isabel Cruz Solis
*/

const int aire_sensor = 1023;
const int agua_sensor = 345;
int sensor;
int salida;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor = analogRead(35);
  Serial.print("Valor detectado: ");
  Serial.println(sensor);

  if ((sensor >= agua_sensor) && (sensor <= aire_sensor)){
    salida = map(sensor,aire_sensor,agua_sensor,0,100);
  }
  else {
    //Serial.println("Error en la lectura.");
  }

  Serial.print("Porcentaje de Humedad: ");
  Serial.println(salida);
  Serial.println("");
  delay(1000);
}
