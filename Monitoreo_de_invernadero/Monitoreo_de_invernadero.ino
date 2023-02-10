/*
 * Centro de Innovación e Integración de Tecnologías Avanzadas
 * Unidad Veracruz
 * Departamento de electrónica
 * 
 * Elaborado por:
 *  Antonio del Angel Moreno Carbajal     Jefe de departamento
 *  Maria Isabel Cruz Solis               Residente del departamento
 * 
 * Fecha:
 *  09/02/2023
 * 
 * Descripción:
 *  El siguiente sistema realiza el monitoreo de un invernadero rustico con componentes de bajo costo.
 *  
 * Componentes:
 *  Tarjeta de desarroollo ESP32
 *  Sensor de humedad
 *  Sensor de radiación UV
 * 
 * Tipo de conexión:
 *  Inalambrica (Wi-FI)
 */

// Bibliotecas
#include <WiFi.h>                                                 // Biblioteca para el control de WiFi
#include <PubSubClient.h>                                         // Biblioteca para conexion MQTT

// Constastes
const int humedad;

// Datos de WiFi
const char* ssid = "Wi-Fi IPN";                                   // Aquí debes poner el nombre de tu red
const char* password = "";                                        // Aquí debes poner la contraseña de tu red

// Datos del broker MQTT
const char* mqtt_server = "148.204.143.180";                      // Si estas en una red local, coloca la IP asignada, en caso contrario, coloca la IP publica
IPAddress server(148,204,143,180);

// Objetos
WiFiClient espClient;                                             // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient);                                   // Este objeto maneja los datos de conexion al broker

// Variables
int flashLedPin = 4;                                              // Para indicar el estatus de conexión
int statusLedPin = 3;                                             // Para ser controlado por MQTT
long timeNow, timeLast;                                           // Variables de control de tiempo no bloqueante
int data = 0;                                                     // Contador
int wait = 5000;                                                  // Indica la espera cada 5 segundos para envío de mensajes MQTT
int t;

void setup() {
  pinMode(2,OUTPUT);
  //pinMode(,OUTPUT);
  //pinMode(,INPUT);
  Serial.begin(115200);

  Serial.println();
  Serial.println();
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
  // Verificar siempre que haya conexión al broker
  if (!client.connected()) {
    reconnect();                                                  // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }
  client.loop();                                                  // Esta función es muy importante, ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker
  
  timeNow = millis();                                             // Control de tiempo para esperas no bloqueantes
  if (timeNow - timeLast > wait) {
  char dataString[8];                                             // Define una arreglo de caracteres para enviarlos por MQTT, especifica la longitud del mensaje en 8 caracteres
  dtostrf(t, 1, 2, dataString);                                   // Esta es una función nativa de leguaje AVR que convierte un arreglo de caracteres en una variable String
  Serial.print("Temperatura: ");                                  // Se imprime en monitor solo para poder visualizar que el evento sucede
  Serial.println(dataString);
  client.publish("codigoIoT/G6/temp", dataString);                // Esta es la función que envía los datos por MQTT, especifica el tema y el valor

  delay (100);
  //dtostrf(h, 1, 2, dataString);
  Serial.print("Humedad: ");                                      // Se imprime en monitor solo para poder visualizar que el evento sucede
  Serial.println(dataString);
  client.publish("codigoIoT/G6/hum", dataString);
  }
}

// Funciones de usuario

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
  if (String(topic) == "codigoIoT/G6/led") {  // En caso de recibirse mensaje en el tema esp32/output
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
