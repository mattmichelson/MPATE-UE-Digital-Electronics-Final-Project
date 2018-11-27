//Matthew Michelson
//Digital Electronics final project
///these are all the numbered variables
int ledPins[8] = {12, 11, 10, 9, 8, 7, 6, 5};
int buttonPins [2] = {30, 31};
int switchPin = 28;
int prevChannelButtonPin = 27;
int nextChannelButtonPin = 26;
int channelDisplayed = 0;
int midiNotes[3] = { 60, 62, 64};
int currentStep = 0;
unsigned long lastStepTime = 0;
int tempo = 0;

//booleans
boolean lastButtonStates [2] = {LOW};
boolean buttonStates [2] = {LOW};
boolean prevChannelButtonState = LOW;
boolean lastPrevChannelButtonState = LOW;
boolean nextChannelButtonState = LOW;
boolean lastNextChannelButtonState = LOW;
boolean on[3][8] = {
  { LOW },
  { LOW },
  { LOW }
};

void setup() {
  //pinModes
  Serial.begin(9600);
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins [i], OUTPUT);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(buttonPins [i], INPUT);
  }
  pinMode(switchPin, INPUT);
  pinMode(prevChannelButtonPin, INPUT);
  pinMode(nextChannelButtonPin, INPUT);
}

void loop() {
  //checks to see the button(s) selected
  //changes the channel
  checkButtons();
  //turns on or off the leds
  setLeds();
  //sends the midi note(s) in the selected order
  sequence();

}

void sequence() {
  //gives tempo control to that pin
  tempo = analogRead(A14);

  //switch
  if (digitalRead(switchPin) == HIGH) {
    forwardStep(tempo);
  }

  if (digitalRead(switchPin) == LOW) {
    backwardStep(tempo);
  }
}

void checkButtons() {

  /*Checks through the 8-step sequencer buttons to see if they are pressed
     modifies the 3 channels depending on which channel is displayed
     If they are pressed, that step will turn on until pressed again
  */

  //mic button
  for (int i = 0; i < 2; i++) {
    lastButtonStates[i] = buttonStates [i];
    buttonStates [i] = digitalRead(buttonPins [i]);
  }

  if (buttonStates [0] == HIGH && lastButtonStates [0] == LOW) {
    if (on[channelDisplayed][currentStep] == false) {
      on[channelDisplayed][currentStep] = true;
    }
  }
  
  //reset button
  for (int i = 0; i < 8; i++) {
    if (buttonStates [1] == HIGH && lastButtonStates [1] == LOW) {
      on[channelDisplayed][i] = false;
    }
  }

  //this code switches through the 3 channels
  lastPrevChannelButtonState = prevChannelButtonState;
  prevChannelButtonState = digitalRead(prevChannelButtonPin);

  if (prevChannelButtonState == HIGH && lastPrevChannelButtonState == LOW) {
    //Serial.println("Current channel is:" channelDisplayed);
    //displayNumberAndText(channelDisplayed);
    --channelDisplayed;
  }

  if (channelDisplayed < 0) {
    channelDisplayed = 2;
  }

  lastNextChannelButtonState = nextChannelButtonState;
  nextChannelButtonState = digitalRead(nextChannelButtonPin);

  if (nextChannelButtonState == HIGH && lastNextChannelButtonState == LOW) {
    channelDisplayed++;
    //Serial.println("Current channel is:"channelDisplayed);
    //displayNumberAndText(channelDisplayed);
  }

  if (channelDisplayed > 2) {
    channelDisplayed = 0;
  }
}

void setLeds() {
  for (int i = 0; i < 8; i++) {
    //for  the current channel, if the step is selected turn that LED on
    if (on [channelDisplayed][i] == true || i == currentStep) {
      digitalWrite(ledPins [i], HIGH);
      //Serial.println(ledPins [i]);
    }

    //for  the current channel, if the step is no longer selected, turn that LED off
    else if (on [channelDisplayed][i] == false || i == currentStep) {
      digitalWrite(ledPins [i], LOW);
    }
  }
}


void forwardStep(int potentiometer) {
  //sends the midi notes when stepping forwards and the current step is selected

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...

    for (int i = 0; i < 3; i++) {
      usbMIDI.sendNoteOff(midiNotes[i], 0, 1); //send midi note off
    }

    currentStep = currentStep + 1;         //increment to the next step
    Serial.println(currentStep);
    if (currentStep > 7) {
      currentStep = 0;
    }

    for (int i = 0; i < 7; i++) {
      if (on[i][currentStep] == true) {
        usbMIDI.sendNoteOn(midiNotes[i], 127, 1); //send midi note on
        usbMIDI.sendNoteOff(midiNotes[i], 0, 1); //send midi note off
      }
    }

    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void backwardStep(int potentiometer) {
  //sends the midi notes when stepping backwards and the current step is selected
  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(ledPins[currentStep], LOW);  //turn off the current led

    for (int i = 0; i < 2; i++) {
      usbMIDI.sendNoteOff(midiNotes[i], 0, 1); //send Midi note off
    }

    currentStep = currentStep - 1;         //increment to the next step
    if (currentStep < 0) {
      currentStep = 7;
    }

    for (int i = 0; i < 2; i++) {
      if (on[i][currentStep] == true) {
        usbMIDI.sendNoteOn(midiNotes[i], 127, 1); //send midi note on
        usbMIDI.sendNoteOff(midiNotes[i], 0, 1);//send midi note off
      }

    }
    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void displayNumberAndText(int number) {
  char text[] = "channel:";
  clearDisplay();
  Serial1.print(text);
  char displayFormattedNumber[4];
  sprintf(displayFormattedNumber, "%4d" , number);
  clearDisplay();
  Serial.println(displayFormattedNumber);
}

void clearDisplay() {
  Serial1.write(0x76); //clear display command
}