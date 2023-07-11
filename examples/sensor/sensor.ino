#include <Ethernet.h>
#include <MQTTPubSubClient.h>
#include <ArduinoHA.h>

#define BROKER_ADDR         IPAddress(192,168,11,103)
#define BROKER_USERNAME     "USERNAME"
#define BROKER_PASSWORD     "PASSWORD"

#define POTENTIOMETER_PIN 26

byte mac[] = { 0x00, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;
HADevice device(mac, sizeof(mac));
HAMqtt HA_mqtt(client, device);

unsigned long lastUpdateAt = 0;
HASensor valve("pico_sensor");

void setup() {
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Ethernet.init(17);
  pinMode(20,OUTPUT);
  digitalWrite(20,LOW);
  delay(100);
  digitalWrite(20,HIGH);
  delay(100);

  Ethernet.begin(mac);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  Serial.print("Device IP address:");
  Serial.println(Ethernet.localIP());


  pinMode (POTENTIOMETER_PIN,INPUT);//initialize digital pin 26 as an input

  HA_mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
}

unsigned long previousMillis = 0;
const long interval = 10;

void loop() {
  char c_val[10];
  int pot_val= 0;
  HA_mqtt.loop();

  if ((millis() - lastUpdateAt) > 1000) {
    pot_val = analogRead(POTENTIOMETER_PIN);
    itoa(pot_val, c_val, 10);
    Serial.printf("Pot value is: %s \r\n", c_val);

    valve.setValue(c_val);
    lastUpdateAt = millis();
  }
}
