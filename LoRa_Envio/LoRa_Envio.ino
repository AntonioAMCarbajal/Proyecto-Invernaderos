//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 433E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels 32

//packet counter
int counter = 0;

Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
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
}

void loop() {
   
  Serial.print("Enviando paquete: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  //LoRa.print("DEC ");
  LoRa.print(counter);
  LoRa.endPacket();
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Paquete LoRa enviado.");
  display.setCursor(0,10);
  display.print("Contador: ");
  display.setCursor(60,10);
  display.print(counter);      
  display.display();

  counter++;
  
  delay(5000);
}