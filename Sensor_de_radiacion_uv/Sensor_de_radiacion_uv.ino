/*
  Centro de Innovación e Integración de Tecnologías Avanzadas
  CIITA Unidad Veracruz
  Fecha: 17 de abril de 2023
  
  Codigo para probar el sensor de radiación UV GUVA-S12SD

  Elaborado por:
    Antonio del Angel Moreno Carbajal
    Maria Isabel Cruz Solis
*/

float Sensorvalor;
float Sensorvoltaje;
int radiacion;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Sensorvalor = analogRead(34);
  Sensorvoltaje = (Sensorvalor/1023)*5;
  radiacion = (Sensorvoltaje*11)/5;

  Serial.print("La lectura del sensor es: ");
  Serial.println(Sensorvalor);
  Serial.print("El voltaje del sensor es: ");
  Serial.println(Sensorvoltaje);
  Serial.print("El indice de radiación es: ");
  Serial.println(radiacion);
  
  if ((radiacion >=0)&&(radiacion<=2)) {
    Serial.println("Bajo nivel de radiación UV");
  }  
  else if((radiacion >=3)&&(radiacion<=5)) {
    Serial.println("Nivel UV medio, se recomiendan gafas de sol");
  }
  else if((radiacion >=6)&&(radiacion<=7)) {
    Serial.println("Nivel alto, se recomiendan cremas protectoras");
  }
   else if((radiacion >= 8)&&(radiacion <=10)) {
    Serial.println("Nivel muy alto, sombra recomendada");
  }
   else if(radiacion == 11) {
    Serial.println("Nivel extremadamente alto de radiación UV, debe limitarse la exposición al aire libre");
  }
  else {
    Serial.println("Error en la medición");
  }
  
  Serial.println();
  
  delay(2000);
}