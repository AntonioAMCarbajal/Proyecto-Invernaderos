const int aire_sensor = 1023;
const int agua_sensor = 345;
int sensor;
int salida;

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
}

void loop() {
  sensor = analogRead(A0);
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
