/*
 * \brief Get data from wireless remote controller asynchronously (interrupts)
 *
 * This example is using the trigger signal from the remote controller to
 * asynchronously launch a function every time a data is received.
 *
 * \author Quentin Comte-Gaz <quentin@comte-gaz.com>
 * \date 1 July June 2016
 * \license MIT License (contact me if too restrictive)
 * \copyright Copyright (c) 2016 Quentin Comte-Gaz
 * \version 1.0
 */

#include "WirelessRemoteController.h"
bool gToggles[4];
#define A_SWITCH 10
#define B_SWITCH 11


void RelayToggle(int i) {
    Serial.print("Toggle ");
    Serial.print(i);
    Serial.print("\n");
    int pin = -1;
    switch (i) {
        case 0: 
            pin = A_SWITCH;
            break;
        case 1:
            pin = B_SWITCH;
            break;
    }
    Serial.print("Turn ");
    Serial.print(gToggles[i] ? "off":"on");
    Serial.print(" relay connected to ");
    
    Serial.println(pin);
    gToggles[i] = !gToggles[i];
    digitalWrite(pin, gToggles[i] ? HIGH : LOW);
}


// Create wireless remote controller instance with D3, D4, D5, D6 pins as data pins
WirelessRemoteController remote_controller(3, 4, 5, 6);

void receiveFromRemoteController()
{
    bool current_rx[4];
    if(remote_controller.getCurrentValue(current_rx)) {
        Serial.print("Data received: ");
        char button[2];
        button[1] = 0;
        for (int i = 0; i < 4; i++) {
            if (current_rx[i] == 1) {
                RelayToggle(i);
                button[0] = 'A' + i;
                Serial.print("Button ");
                Serial.print(button);
                Serial.print(" Pressed\n");
                return;
            }
        }
    } else {
        Serial.print("No data received (!!should not happen!!)\n");
    }
}


void setup(void)
{
  for (int i=0;i<4;i++)
    gToggles[i] = false;

  pinMode(A_SWITCH, OUTPUT); 
  pinMode(B_SWITCH, OUTPUT); 
  

  Serial.begin(9600);

  // Trigger interrupt by putting D2 pin as DT remote controller pin (trigger)
  remote_controller.addTrigger(2, receiveFromRemoteController);

  Serial.print("Data sent from remote controller will be printed when received.\n");
}

void loop()
{
}