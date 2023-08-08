/* Descomentar para usar la pantalla integrada al LILYGO
* #define OLED_SDA 4
* #define OLED_SCL 15 
* #define OLED_RST 16
* #define SCREEN_WIDTH 128                                          // Ancho del display en pixels
* #define SCREEN_HEIGHT 32                                          // Alto del distlay en pixels
*/
#define SCK 5                                                       // Definicion de los pines usados por LoRa
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define BAND 433E6                                                  // 433E6 for Asia, 866E6 for Europe, 915E6 for North America

/* Descomentar para usar la pantalla integrada al LILYGO
* #include <Adafruit_GFX.h>
* #include <Adafruit_SSD1306.h>
*/
#include <SPI.h>                                                    // Librerias para LoRa
#include <LoRa.h>                                                   // Librerias para LoRa
#include <WiFi.h>                                                   // Biblioteca para el control de WiFi
#include <PubSubClient.h>                                           // Biblioteca para conexion MQTT 

// Datos de WiFi a conectar
const char* ssid = "INFINITUM5DFA";                                 // Aquí se debe poner el nombre de la red
const char* password = "AxF3sQtnzL";                                // Aquí se debe poner la contraseña de la red

// Datos de la conexión a broker MQTT
const char* mqtt_server = "192.168.1.67";                           // Colocar IP del servidor (computadora) ejemplo: "192.168.137.1" 
IPAddress server (192,168,1,67);

/* Descomentar para usar la pantalla integrada al LILYGO
* Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
*/
WiFiClient espClient;                                               // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient);                                     // Este objeto maneja los datos de conexion al broker
String LoRaData, json;

void setup() { 
  Serial.begin(115200);                                             // Iniciando el puerto serial de la ESP32
  Serial.println("LoRa Receiver Test");
  
  /* Descomentar para iniciar el display del LILYGO
  * display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  * display.clearDisplay();
  * display.setTextColor(WHITE);
  * display.setTextSize(1);
  * display.setCursor(0,0);
  * display.print("LORA RECEIVER ");
  * display.display();
  */

  SPI.begin(SCK, MISO, MOSI, SS);                                           // Pines SPI del LoRa
  LoRa.setPins(SS, RST, DIO0);                                              // Configuración del transmisor LoRa

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  /* Descomentar para escribir un mensaje en pantalla del módulo LILYGO
  * display.setCursor(0,10);
  * display.println("LoRa Inicializacion OK!");
  * display.display();
  */
  Serial.println();                                                 // Imprimiendo en puerto serial la conexión al WiFi
  Serial.println();                                                  
  Serial.print("Conectar a ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);                                       // Esta es la función que realiza la conexión a WiFi

  while (WiFi.status() != WL_CONNECTED) {                           // Este bucle espera a que se realice la conexión
    Serial.print(".");  
    delay (5);
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  
  delay (1000);                                                   // Esta espera es solo una formalidad antes de iniciar la comunicación con el broker

  // Conexión con el broker MQTT
  client.setServer(server, 1883);                                 // Conectarse a la IP del broker en el puerto indicado
  client.setCallback(callback);                                   // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);                                                    // Esta espera es preventiva, espera a la conexión para no perder información  
}

void loop() {  
  if (!client.connected()) {                                      // Verificar siempre que haya conexión al broker
    reconnect();                                                  // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }
  
  client.loop();                                                  // Esta función es muy importante, ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker
  
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
    }
    /* Descomentar para escribir un mensaje en pantalla del módulo LILYGO
    * display.clearDisplay();
    * display.setCursor(0,0);
    * display.print("Mensaje recibido:");
    * display.setCursor(0,10);
    * display.print(LoRaData);
    * display.display();    
    */
    json = LoRaData;
    Serial.println(json);                                         // Se imprime en monitor solo para poder visualizar que el string esta correctamente formado
    int str_len = json.length() + 1;                              // Se calcula la longitud del string
      char char_array[str_len];                                     // Se crea un arreglo de caracteres de dicha longitud
      json.toCharArray(char_array, str_len);                        // Se convierte el string a char array    
      client.publish("CIITA/VERACRUZ/MONITOREO", char_array);       // Esta es la función que envía los datos por MQTT, especifica el tema y el valor
      delay (100);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  // Indicar por serial que llegó un mensaje
  Serial.print("Llegó un mensaje en el tema: ");
  Serial.print(topic);

  // Concatenar los mensajes recibidos para conformarlos como una varialbe String
  String messageTemp;                                             // Se declara la variable en la cual se generará el mensaje completo  
  for (int i = 0; i < length; i++) {                              // Se imprime y concatena el mensaje
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);

/* 
* En esta parte el personal del CIITA puede agregar las funciones que requieran
* para actuar segun lo necesiten al recibir un mensaje por MQTT
*/
   
  // El ESP32 está suscrito al tema CIITA/VERACRUZ/CONTROL 
  if (String(topic) == "CIITA/VERACRUZ/CONTROL") { 
    if(messageTemp == "true"){
      Serial.println("Led encendido");
    }
  else if(messageTemp == "false"){
     Serial.println("Led apagado");
    }
  }
}

// Función para reconectarse al tema de MQTT en caso de desconexión
void reconnect() {
  // Bucle hasta lograr conexión
  while (!client.connected()) {                                   // Pregunta si hay conexión
    Serial.print("Tratando de contectarse...");
    
    // Intentar reconexión
    if (client.connect("ESP32CAMClient")) {                       // Pregunta por el resultado del intento de conexión
      Serial.println("Conectado");
      client.subscribe("CIITA/VERACRUZ/CONTROL");                 // Esta función realiza la suscripción al tema
    }
    else {  
      Serial.print("Conexion fallida, Error rc=");
      Serial.print(client.state());                               // Muestra el codigo de error
      Serial.println(" Volviendo a intentar en 5 segundos");
      
      delay(5000);
      Serial.println (client.connected());                        // Muestra estatus de conexión
    }
  }
}