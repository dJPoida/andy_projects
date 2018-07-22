//THIS PROJECT REQUIRES THE KEYPAD LIBRARY FOR ARDUINO BY MARK STANLEY AND ALEXANDER BREVIG
//(available through the Arduino Library Manager "Sketch->Include Library->Manage Libraries")
#include <Key.h>
#include <Keypad.h>

//Button Pins
#define PIN_BUTTON_ROW_1  5   //Button Row 1
#define PIN_BUTTON_ROW_2  6   //Button Row 2
#define PIN_BUTTON_ROW_3  7   //Button Row 3
#define PIN_BUTTON_ROW_4  8   //Button Row 4
#define PIN_BUTTON_COL_1  9   //Button Col 1
#define PIN_BUTTON_COL_2  10  //Button Col 2
#define PIN_BUTTON_COL_3  11  //Button Col 3
#define PIN_BUTTON_COL_4  12  //Button Col 4

//all of the and columns in lists for use later
byte button_row_pins[] = {PIN_BUTTON_ROW_1, PIN_BUTTON_ROW_2, PIN_BUTTON_ROW_3, PIN_BUTTON_ROW_4};
byte button_col_pins[] = {PIN_BUTTON_COL_1, PIN_BUTTON_COL_2, PIN_BUTTON_COL_3, PIN_BUTTON_COL_4};

//Buzzer Pins
#define PIN_BUZZER        3   //Buzzer

//LDR (Light sensor) Pins
#define PIN_LDR           A0  //LDR (Light Detecting Resistor) (Light Sensor)

//LDR Calibration
#define LDR_CALIBRATION   1.5 //Multiply the LDR reading by this value to make it more gooda

// Notes (Hz for the buzzer)
#define  NOTE_C1        261
#define  NOTE_D1        294
#define  NOTE_E1        329
#define  NOTE_F1        349
#define  NOTE_G1        392
#define  NOTE_A1        440
#define  NOTE_B1        493
#define  NOTE_C2        523
#define  NOTE_D2        587
#define  NOTE_E2        659
#define  NOTE_F2        698
#define  NOTE_G2        784
#define  NOTE_A2        880
#define  NOTE_B2        988
#define  NOTE_C3        1047
#define  NOTE_D3        1175
#define  NOTE_REST      0

//All notes in an array/list for access later
int notes[] = {
  NOTE_REST,
  NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1,
  NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2,
  NOTE_C3, NOTE_D3
};

//Keypad button bindings (which note to play when a button is pressed)
byte keyNoteMap[4][4] = {
  {1,  5,  8,  12}, 
  {2,  6,  9,  13},
  {3,  7,  10, 14},  
  {4,  8,  11, 15}
};

//Keypad Setup
Keypad keypad = Keypad(makeKeymap(keyNoteMap), button_row_pins, button_col_pins, 4, 4);

//Program Variables
byte currentKey = 0;          //Current key being depressed
int currentNote = NOTE_REST;  //Current note being played
int initialLDRValue = 0;      //Initial LDR Value
int currentLDRValue = 0;      //Current LDR Value
int currentPitchBend = 0;     //Pitch Bend Value (0-100)

/**
 * Initial Setup
 */
void setup() {
  //Spin up a serial connection for debugging
  Serial.begin(9600);
  Serial.println("Ready.");
  
  //Buzzer Pins
  pinMode(PIN_BUZZER, OUTPUT);

  //LDR Pins
  pinMode(PIN_LDR, INPUT);

  //Read the inital (ambient) light value - this helps improve the accuracy of the subsequent reads
  initialLDRValue = analogRead(PIN_LDR);
}

/**
 * Main Program Loop
 */
void loop() {
  //Read the Light value
  currentLDRValue = analogRead(PIN_LDR);
  currentPitchBend = constrain(((float(initialLDRValue - currentLDRValue) / float(initialLDRValue)) * LDR_CALIBRATION * 100), 0, 100);

  //Look for keypad presses  
  if (checkCurrentKey()) {
    currentNote = notes[currentKey];
    Serial.println(currentNote);
  }

  //Play the current sound
  if (currentNote != NOTE_REST) {
    if (currentPitchBend > 0) {
      int bentNote = (currentNote + map(currentPitchBend, 0, 100, 0, (notes[currentKey + 1] - currentNote)));
      //Serial.println(bentNote);
      tone(PIN_BUZZER, bentNote);
    } else {
      tone(PIN_BUZZER, currentNote);
    }
  } else {
    noTone(PIN_BUZZER);
  }
}

/**
 * Read the keypad and look for any changes
 */
bool checkCurrentKey () {
  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (keypad.getKeys())
  {
    String msg;

    for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
    {
      if (keypad.key[i].stateChanged )   // Only find keys that have changed state.
      {
        byte keyChar = keypad.key[i].kchar;
        switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            currentKey = keyChar;
            return true;
          //case HOLD:
          //  break;
          case RELEASED:
            if (currentKey == keyChar) {
              currentKey = 0;
              return true;
            }
            break;
          //case IDLE:
          //  break;
        }
      }
    }
  }
  return false;
}

