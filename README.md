# **Pico Smart Home**
**Fusing Home Assistant and Raspberry Pi Pico for Smart Living**


# **1. Introduction**
I plan to create a smart hub using Pico and Home Assistant to automate my home and integrate with other HA devices using Zigbee, Bluetooth, IR, and other functionalities.

<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/device_thumbnail.png"></p>

This time, I would like to introduce the basic aspects of setting up the environment.

<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/device_info.png"></p>
  
I will be using Arduino as the development tool and Raspberry Pi Pico board as the hardware. Additionally, I will utilize the Pico Bricks development kit to integrate various types of devices.
Pico Bricks allows for convenient usage without the need to individually purchase and connect components. It provides an easy way to add and integrate other devices as well.

Both Pico Bricks and Home Assistant RP2040 utilize the Pico W, which is a Wi-Fi-based board. However, if you wish to use wired Ethernet instead, you can change it to the ***W5100S-EVB-Pico board(WIZnet)***. This board provides support for wired Ethernet connectivity.

-[**W5100S-EVB-PICO**][link-w5100s-evb-pico]

-[**Pico Bricks**][link-picobrick]

-[**Arduino**][link-picobrick]


# 2. **Home Assistant setting**

 ### - Install Home Assistant
Since Linux is the preferred operating system, if you want to set up the server on a Windows PC, you will need to use a virtual machine. The official Home Assistant website provides the necessary files and manuals, allowing you to configure it according to your specific environment.
- [**Home Assistant**][link-HomeAssistant]

I am using VMware for virtualization.

For VMware, there is a detailed tutorial with images available on the following page. Please refer to it for guidance.
- [**Install VMware**][link-set_vmware]

Once the installation is complete and you see the prompt, you have successfully installed it!

**The IP address displayed on the screen will be set as the MQTT Broker IP.*
<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/home_assistant_init.png"></p>

Let's try accessing the page by entering *http://homeassistant.local:8123/* in your browser.

### - Adding a Users
Now, let's proceed with adding a user account. The user ID and password set here will be used directly for MQTT communication.

***[Settings>People>Users>ADD USER]***   
http://homeassistant.local:8123/config/users   

<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/add_users.png"></p>

If you don't see the User tab, please go to the Profile Settings, and select 'Enable Advanced Mode

***[profile> Advanced Mode >Enable]***   
http://homeassistant.local:8123/profile
<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/setting_advanced_mode.png"></p>


### - MQTT Setting

Next, go to the ADD-ON STORE, search for MQTT, and add the Mosquitto broker.

***[Add-ones>ADD-ON STORE]***  
http://homeassistant.local:8123/hassio/store
<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/download_mqtt.png"></p>


# 3. **Arduino Setting**

### - Arduino for W5100S-EVB-Pico

Set up the environment for W5100S-EVB-Pico. Please follow the instructions on this website
- [**W5100S-EVB-Pico with the Arduino IDE**][link-RP2040_Arduino]

Let's install the necessary libraries using the Arduino Library Manager.
In this step, we will install the Home Assistant and MQTT libraries.

***[Sketch> include library > manage libraries> Home-assistant-integration]***   
***[Sketch> include library > manage libraries> MQTTPubSubClient]***

<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/install_arduino_lib.png"></p>

Once you have completed these steps, you should have the basic functionality in place. As you continue with your development, make sure to install any additional libraries you may need along the way.


# 4. **LED Switch Control**

Let's proceed with a simple basic functionality test using this example.   
***https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/examples/led_switch/led_switch.ino***

Now that the basic setup is complete, let's verify the basic functionality. You can refer to the Arduino examples on the following page for guidance.

W5100-EVB-Pico: https://github.com/WIZnet-ArduinoEthernet/Ethernet   
Home Assistant: https://github.com/dawidchyrzynski/arduino-home-assistant 

### - Setting MQTT information

Enter the necessary information for Home Assistant MQTT communication.


```cpp
#define BROKER_ADDR         IPAddress(192, 168, 11, 103)
#define BROKER_USERNAME     "scarlet_pico" 
#define BROKER_PASSWORD     "1234"  
```

### - Ethernet information

Also, please provide the control pin information for W5100S.

* *Note that Pin 20 of Pico Bricks is used for the Buzzer function, so if you want to use the Buzzer feature, please modify the pin settings and hardware accordingly*


```cpp
#define ETH_SPI_CS_PIN    17
#define ETH_RESET_PIN     20
```

### - HW information

Please also configure the LED and Button pins
```cpp
#define LED_PIN             7
#define BUTTON_PIN          10
```

If you want to change the button check time, please modify the 'define' statement below. The current button check time is set to 0.1 seconds.
```cpp
#define DEBOUNCE_TIME       100
```

### - Home Assistant Device information

Let's register the device in Home Assistant. Once you have completed the coding and the system is functioning correctly, it will be automatically registered on the Home Assistant page.
Here, the parameter for HASwitch (in this example, scalet_pico_led) is the unique ID.

```cpp
HAMqtt HA_mqtt(client, device);
HASwitch led("scalet_pico_led");
```

Register a callback function for the command to be executed whenever an MQTT message is subscribed to.

```cpp
led.onCommand(onSwitchCommand);
led.setName("Scarlet Pico LED");
```

Compile and upload the firmware from Arduino and check if the device is successfully registered on the Home Assistant page. On the device information page, you should see the configured name and unique ID of the device.

***[Settings > devices & Service > entities]***   
http://homeassistant.local:8123/config/entities

<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/regiater_device.png"></p>

When you click on the icon, the switch screen will be displayed, and when you move the switch, the LED will activate.

<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/led_switch.png"></p>

That covers the basic functionality.
Actually, I have completed the development up to the point where I can control the LED based on button inputs and integrate it with Home Assistant. However, there seems to be an issue with the additional configuration, as the device registration on Home Assistant is not working properly.
Let me provide an overview based on the remaining information. I'll explain it again in the next project.

### - Controlling LEDs with Buttons

Configure the LED to blink based on button actions in the automation page

***[Settings > Automations & Scenes > CREATE AUTOMATION]***   
: http://homeassistant.local:8123/config/automation/dashboard 

In the Triggers configuration, select [Device], then choose the configured button switch turned on.   
In the Actions configuration, select [Device], then choose the configured LED switch turned on.

<p align="center"><img src="https://github.com/wiznetmaker/RP2040-HAT-HOMEASSISTANT-C/blob/main/static/images/setting_automation.png"></p>

Here is the code to use the push button on Pico as a toggle button
```cpp
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH && lastButtonState == LOW) {

    ledState =! ledState;
    setLedStateButton(ledState);
  }
  lastButtonState = buttonState; 
```

The LED status is sent via MQTT Publish.
LED control is done exclusively through Home Assistant switches.
The button is used solely as an input device.
This approach ensures that there are no conflicts in control status even when additional devices are added









[link-Arduino]: https://www.arduino.cc/en/software 
[link-w5100s-evb-pico]: https://docs.wiznet.io/Product/iEthernet/W5100S/w5100s-evb-pico 
[link-picobrick]: https://picobricks.com
[link-HomeAssistant]: https://www.home-assistant.io/installation/windows
[link-set_vmware]: https://smarthomepursuits.com/how-to-migrate-home-assistant-from-raspberry-pi-to-vmware-workstation/ 
[link-RP2040_Arduino]: https://www.hackster.io/loveivyou/w5100s-evb-pico-with-the-arduino-ide-fe6315 