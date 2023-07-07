/*
 *Home Assiatant LED Switch
 *Using an Arduino WIZnet Ethernet shield.
 *
 * created 07 July 2023 by Scarlet
 */

#include <SPI.h>
#include <Ethernet.h>
#include <MQTTPubSubClient.h>
#include <ArduinoHA.h>

#define BROKER_ADDR         IPAddress(192, 168, 11, 103)
#define BROKER_USERNAME     "USERNAME"
#define BROKER_PASSWORD     "PASSWORD" 

#define DEBOUNCE_TIME       100

#define LED_PIN             7
#define BUTTON_PIN          10

#define ETH_SPI_CS_PIN      17
#define ETH_RESET_PIN       20

byte mac[] = { 0x00, 0x00, 0x00, 0xEF, 0xFE, 0xED };

EthernetClient client;

HADevice device(mac, sizeof(mac));
HAMqtt HA_mqtt(client, device);
HASwitch led("my_pico_led");


bool ledState = LOW;
int buttonState = 0;         
int lastButtonState = 0;    

unsigned long lastDebounceTime = 0;

// Handle switch command from Home Assistant
void onSwitchCommand(bool state, HASwitch* sender)
{
  digitalWrite(LED_PIN, (state ? HIGH : LOW));
  sender->setState(state); // Report state back to Home Assistant
  //Serial.printf("Switch command: %s\r\n", state ? "ON" : "OFF");
}

// Set LED state based on button state
void setLedStateButton(bool state)
{
  if (state) {
    led.turnOn();
  }
  else {
    led.turnOff();
  }
  //Serial.printf("LED state: %s\r\n", state ? "ON" : "OFF");
}

void readButtonState(void){
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH && lastButtonState == LOW) {

    ledState = !ledState;
    setLedStateButton(ledState);
  }
  lastButtonState = buttonState; 
}

void EthernetInit (void){
  Ethernet.init(ETH_SPI_CS_PIN);  

  pinMode(ETH_RESET_PIN, OUTPUT);
  digitalWrite(ETH_RESET_PIN, HIGH);
  delay(100);
  
  Ethernet.begin(mac);
  
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  Serial.print("Device IP address: ");
  Serial.println(Ethernet.localIP());

}
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  EthernetInit();

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  led.onCommand(onSwitchCommand);
  led.setName("My Pico LED");

  HA_mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
  
  lastDebounceTime = millis();
}

void loop() {
  HA_mqtt.loop();

  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME){
    readButtonState();
    lastDebounceTime = millis();
  }
 
}
