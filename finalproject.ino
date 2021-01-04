/* Matthew Michelson */

/*Digital Electronics Final Project */

  /* M I D I  C L A P  */



/*init for the display */

#include <Arduino.h>
#include <TM1637Display.h>

/* Module connection pins (Digital Pins) */

#define CLK 2
#define DIO 3

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

TM1637Display display(CLK, DIO);




/*These are all the numbered variables*/

int ledPins[16] = {5, 6, 7, 8, 9, 10, 11, 12, 24, 25, 26, 27, 28, 29, 30, 31};
int buttonPins [4] = {36, 35, 34, 37};
int switchPin = 38;
int prevChannelButtonPin = 39;
int nextChannelButtonPin = 14;
int channelDisplayed = 1;
int midiNotes[16] = { 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66};
int currentStep = 0;
unsigned long lastStepTime = 0;
unsigned long lastRecordTime = 0;
unsigned long lastPressTime = 0;
int tempo = 0;

/*Booleans*/

boolean lastButtonStates [3] = {LOW};
boolean buttonStates [3] = {LOW};
boolean prevChannelButtonState = LOW;
boolean lastPrevChannelButtonState = LOW;
boolean nextChannelButtonState = LOW;
boolean lastNextChannelButtonState = LOW;
boolean recordOn = false;
boolean on[16][16] = {
  { LOW },
  { LOW },
  { LOW }
};

void setup() {
  /*pinModes */
  
  Serial.begin(9600);
  for (int i = 0; i < 16; i++) {
    pinMode(ledPins [i], OUTPUT);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins [i], INPUT);
  }
  pinMode(switchPin, INPUT);
  pinMode(prevChannelButtonPin, INPUT);
  pinMode(nextChannelButtonPin, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  //for the display
  display.setBrightness(0x0f);
  if (channelDisplayed == 1) {
    display.showNumberHexEx(0xc1);
  }


  /* Checks to see the button(s) selected */
  /* Changes the channel */
  
  checkButtons();
  
  /* Turns on or off the leds */
  
  if (recordOn == true) {
    //plainSequence();
    sequenceWithSelectedStep();
    digitalWrite(13, HIGH);
  }

  if (recordOn == false) {
    midiSequence();
    digitalWrite(13, LOW);
  }
}

void checkButtons() {

  /*Checks through the 8-step sequencer buttons to see if they are pressed
     modifies the 3 channels depending on which channel is displayed
     If they are pressed, that step will turn on until pressed again
  */

  for (int i = 0; i < 4; i++) {
    lastButtonStates[i] = buttonStates [i];
    buttonStates [i] = digitalRead(buttonPins [i]);
  }

  /*record button */
  
  if (buttonStates [2] == HIGH && lastButtonStates [2] == LOW) {
    if (recordOn == false) {
      recordOn = true;

    } else if (recordOn == true) {
      recordOn = false;
    }
  }

  /* Channel Reset Button */
  
  for (int i = 0; i < 16; i++) {
    if (buttonStates [1] == HIGH && lastButtonStates [1] == LOW) {
      on[channelDisplayed - 1][i] = false;
    }
  }
  
  /* Master Reset Button */
  
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 8; j++) {
      if (buttonStates [3] == HIGH && lastButtonStates [3] == LOW) {
        on[j][i] = false;
      }
    }
  }


  /* Switches through the 3 channels while checking button states */
  
  if (millis() > lastPressTime + 200) {
    lastPrevChannelButtonState = prevChannelButtonState;
    prevChannelButtonState = digitalRead(prevChannelButtonPin);

    if (prevChannelButtonState == HIGH && lastPrevChannelButtonState == LOW) {
      //Serial.println("Current channel is:" channelDisplayed);

      --channelDisplayed;
      if (channelDisplayed < 1) {
        channelDisplayed = 16;
      }

      if (channelDisplayed == 1) {
        display.showNumberHexEx(0xc1);
      }

      if (channelDisplayed == 2) {
        display.showNumberHexEx(0xc2);
      }
      if (channelDisplayed == 3) {
        display.showNumberHexEx(0xc3);
      }
      if (channelDisplayed == 4) {
        display.showNumberHexEx(0xc4);
      }

      if (channelDisplayed == 5) {
        display.showNumberHexEx(0xc5);
      }
      if (channelDisplayed == 6) {
        display.showNumberHexEx(0xc6);
      }
      if (channelDisplayed == 7) {
        display.showNumberHexEx(0xc7);
      }

      if (channelDisplayed == 8) {
        display.showNumberHexEx(0xc8);
      }
      if (channelDisplayed == 9) {
        display.showNumberHexEx(0xc9);
      }
      if (channelDisplayed == 10) {
        display.showNumberHexEx(0xc10);
      }

      if (channelDisplayed == 11) {
        display.showNumberHexEx(0xc11);
      }
      if (channelDisplayed == 12) {
        display.showNumberHexEx(0xc12);
      }
      if (channelDisplayed == 13) {
        display.showNumberHexEx(0xc13);
      }

      if (channelDisplayed == 14) {
        display.showNumberHexEx(0xc14);
      }
      if (channelDisplayed == 15) {
        display.showNumberHexEx(0xc15);
      }
      if (channelDisplayed == 16) {
        display.showNumberHexEx(0xc16);
      }
      lastPressTime = millis();
    }
  }


  if (millis() > lastPressTime + 200) {
    lastNextChannelButtonState = nextChannelButtonState;
    nextChannelButtonState = digitalRead(nextChannelButtonPin);

    if (nextChannelButtonState == HIGH && lastNextChannelButtonState == LOW) {
      channelDisplayed++;
      if (channelDisplayed > 16) {
        channelDisplayed = 1;
      }
      //Serial.println("Current channel is:"channelDisplayed);

      if (channelDisplayed == 1) {
        display.showNumberHexEx(0xc1);
      }
      if (channelDisplayed == 2) {
        display.showNumberHexEx(0xc2);
      }
      if (channelDisplayed == 3) {
        display.showNumberHexEx(0xc3);
      }
      if (channelDisplayed == 4) {
        display.showNumberHexEx(0xc4);
      }
      if (channelDisplayed == 5) {
        display.showNumberHexEx(0xc5);
      }
      if (channelDisplayed == 6) {
        display.showNumberHexEx(0xc6);
      }
      if (channelDisplayed == 7) {
        display.showNumberHexEx(0xc7);
      }
      if (channelDisplayed == 8) {
        display.showNumberHexEx(0xc8);
      }
      if (channelDisplayed == 9) {
        display.showNumberHexEx(0xc9);
      }
      if (channelDisplayed == 10) {
        display.showNumberHexEx(0xc10);
      }
      if (channelDisplayed == 11) {
        display.showNumberHexEx(0xc11);
      }
      if (channelDisplayed == 12) {
        display.showNumberHexEx(0xc12);
      }
      if (channelDisplayed == 13) {
        display.showNumberHexEx(0xc13);
      }
      if (channelDisplayed == 14) {
        display.showNumberHexEx(0xc14);
      }
      if (channelDisplayed == 15) {
        display.showNumberHexEx(0xc15);
      }
      if (channelDisplayed == 16) {
        display.showNumberHexEx(0xc16);
      }
      lastPressTime = millis();
    }
  }
}

void midiSequence() {

  setLeds();
  tempo = analogRead(A14);

  if (digitalRead(switchPin) == HIGH) {

    if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...

      for (int i = 0; i < 16; i++) {
        usbMIDI.sendNoteOff(midiNotes[i], 0, 1); //send midi note off
      }

      currentStep = currentStep + 1;         //increment to the next step
      Serial.println(currentStep);
      if (currentStep > 15) {
        currentStep = 0;
      }

      for (int i = 0; i < 16; i++) {
        if (on[i][currentStep] == true) {
          usbMIDI.sendNoteOn(midiNotes[i], 127, 1); //send midi note on
          usbMIDI.sendNoteOff(midiNotes[i], 0, 1); //send midi note off
        }
      }

      lastStepTime = millis();               //set lastStepTime to the current time
    }
  }



  if (digitalRead(switchPin) == LOW) {

    /* Sends the midi notes to teensy 3.5 when stepping backwards and the current step is selected */
    
    if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
      //digitalWrite(ledPins[currentStep], LOW);  //turn off the current led

      for (int i = 0; i < 16; i++) {
        usbMIDI.sendNoteOff(midiNotes[i], 0, 1); //send Midi note off
      }

      currentStep = currentStep - 1;         //increment to the next step
      if (currentStep < 0) {
        currentStep = 15;
      }

      for (int i = 0; i < 16 ; i++) {
        if (on[i][currentStep] == true) {
          usbMIDI.sendNoteOn(midiNotes[i], 127, 1); //send midi note on
          usbMIDI.sendNoteOff(midiNotes[i], 0, 1);//send midi note off
        }

      }
      lastStepTime = millis();               //set lastStepTime to the current time
    }
  }

}

void setLeds() {
  for (int i = 0; i < 16; i++) {
    //for  the current channel, if the step is selected turn that LED on
    if (on [channelDisplayed - 1][i] == true || i == currentStep) {
      digitalWrite(ledPins [i], HIGH);
      //Serial.println(ledPins [i]);
    }

    //for  the current channel, if the step is no longer selected, turn that LED off
    else if (on [channelDisplayed - 1][i] == false || i == currentStep) {
      digitalWrite(ledPins [i], LOW);
    }
  }
}

void sequenceWithSelectedStep() {

  if (millis() > lastRecordTime + 50) {   //if its time to go to the next step...
    
    /* Mic Button */
    
    if (buttonStates [0] == HIGH && lastButtonStates [0] == LOW) {
      if (on[channelDisplayed - 1][currentStep] == false) {
        on[channelDisplayed - 1][currentStep] = true;
        lastRecordTime = millis();
      }
    }
  }

  setLeds();
  tempo = analogRead(A14);

  if (digitalRead(switchPin) == HIGH) {

    if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...

      currentStep = currentStep + 1;         //increment to the next step
      Serial.println(currentStep);
      if (currentStep > 15) {
        currentStep = 0;
      }

      lastStepTime = millis();               //set lastStepTime to the current time
    }
  }



  if (digitalRead(switchPin) == LOW) {

    /* Sends the midi notes to the teensy when stepping backwards and the current step is selected */
    
    if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
      //digitalWrite(ledPins[currentStep], LOW);  //turn off the current led

      currentStep = currentStep - 1;         //decrement to the next step
      if (currentStep < 0) {
        currentStep = 15;
      }

      lastStepTime = millis();               //set lastStepTime to the current time
    }
  }
}
