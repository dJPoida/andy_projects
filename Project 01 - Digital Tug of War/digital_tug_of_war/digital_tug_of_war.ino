//Game Config
#define WIN_OFFSET 10       //When a player edges the score away from the start position by this value, the game is over (max 128)

//Button Phases
//These ensure the player cycles through a complete motion of pressing and depressing each button in sequence
#define BUTTON_PHASE_NONE 0
#define BUTTON_PHASE_LEFT_BUTTON_DOWN 1
#define BUTTON_PHASE_LEFT_BUTTON_UP 2
#define BUTTON_PHASE_RIGHT_BUTTON_DOWN 3
#define BUTTON_PHASE_RIGHT_BUTTON_UP 4

//Player 1 Pins
#define PIN_P1_LED_1    2   //Player 1 LED 1
#define PIN_P1_LED_2    3   //Player 1 LED 2
#define PIN_P1_BUTTON_1 4   //Player 1 Button 1
#define PIN_P1_BUTTON_2 5   //Player 1 Button 2

//Player 2 Pins
#define PIN_P2_LED_1    12  //Player 1 LED 1
#define PIN_P2_LED_2    13  //Player 1 LED 2
#define PIN_P2_BUTTON_1 7   //Player 1 Button 1
#define PIN_P2_BUTTON_2 8   //Player 1 Button 2

//RGB LED Pins
#define PIN_RGB_R       11  //RGB Red
#define PIN_RGB_G       10  //RGB Green
#define PIN_RGB_B       9   //RGB Blue

//Game Variables
bool player_1_toggle = false;                     //Player 1 Left / Right Toggle
byte player_1_button_phase = BUTTON_PHASE_NONE;   //This phase increments in each stage of the button press and releas cycle
unsigned long player_1_score = 0;                 //Player 1 current score

bool player_2_toggle = false;                     //Player 2 Left / Right Toggle
byte player_2_button_phase = BUTTON_PHASE_NONE;   //This phase increments in each stage of the button press and releas cycle
unsigned long player_2_score = 0;                 //Player 2 current score

int knot_position = 0;      //Were the knot is currently positioned over the centre line

void setup() {
  Serial.begin(9600);
  
  //Player 1 Pins
  pinMode(PIN_P1_LED_1, OUTPUT);
  pinMode(PIN_P1_LED_2, OUTPUT);
  pinMode(PIN_P1_BUTTON_1, INPUT);
  pinMode(PIN_P1_BUTTON_2, INPUT);

  //Player 2 Pins
  pinMode(PIN_P2_LED_1, OUTPUT);
  pinMode(PIN_P2_LED_2, OUTPUT);
  pinMode(PIN_P2_BUTTON_1, INPUT);
  pinMode(PIN_P2_BUTTON_2, INPUT);

  //RGB LEDs  
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);
  
  //play the game intro
  play_intro();
}

void loop() {
  //Read the Buttons for Player 1
  switch(player_1_button_phase) {
    case BUTTON_PHASE_NONE:
    case BUTTON_PHASE_LEFT_BUTTON_UP:
      //Player has pressed button 2
      if (digitalRead(PIN_P1_BUTTON_2)) {
        player_1_button_phase = BUTTON_PHASE_RIGHT_BUTTON_DOWN;
      }
      break;
      
    case BUTTON_PHASE_RIGHT_BUTTON_DOWN:
      //Player has released button 2
      if (!digitalRead(PIN_P1_BUTTON_2)) {
        player_1_button_phase = BUTTON_PHASE_RIGHT_BUTTON_UP;
        player_1_toggle = true;
      }
      break;
      
    case BUTTON_PHASE_RIGHT_BUTTON_UP:
      //Player has pressed button 1
      if (digitalRead(PIN_P1_BUTTON_1)) {
        player_1_button_phase = BUTTON_PHASE_LEFT_BUTTON_DOWN;
      }
      break;

    case BUTTON_PHASE_LEFT_BUTTON_DOWN:
      //Player has released button 1
      if (!digitalRead(PIN_P1_BUTTON_1)) {
        player_1_button_phase = BUTTON_PHASE_LEFT_BUTTON_UP;
        player_1_toggle = false;
        player_1_score++;
      }
      break;
  }
  
  //Read the Buttons for Player 2
  switch(player_2_button_phase) {
    case BUTTON_PHASE_NONE:
    case BUTTON_PHASE_LEFT_BUTTON_UP:
      //Player has pressed button 2
      if (digitalRead(PIN_P2_BUTTON_2)) {
        player_2_button_phase = BUTTON_PHASE_RIGHT_BUTTON_DOWN;
      }
      break;
      
    case BUTTON_PHASE_RIGHT_BUTTON_DOWN:
      //Player has released button 2
      if (!digitalRead(PIN_P2_BUTTON_2)) {
        player_2_button_phase = BUTTON_PHASE_RIGHT_BUTTON_UP;
        player_2_toggle = true;
      }
      break;
      
    case BUTTON_PHASE_RIGHT_BUTTON_UP:
      //Player has pressed button 1
      if (digitalRead(PIN_P2_BUTTON_1)) {
        player_2_button_phase = BUTTON_PHASE_LEFT_BUTTON_DOWN;
      }
      break;

    case BUTTON_PHASE_LEFT_BUTTON_DOWN:
      //Player has released button 1
      if (!digitalRead(PIN_P2_BUTTON_1)) {
        player_2_button_phase = BUTTON_PHASE_LEFT_BUTTON_UP;
        player_2_toggle = false;
        player_2_score++;
      }
      break;
  }
  
  //Update the player LEDs
  digitalWrite(PIN_P1_LED_1, !player_1_toggle);
  digitalWrite(PIN_P1_LED_2, player_1_toggle);
  digitalWrite(PIN_P2_LED_1, !player_2_toggle);
  digitalWrite(PIN_P2_LED_2, player_2_toggle);

  //Calculate how close one of the players is to winning
  int new_knot_position = player_2_score - player_1_score;
  if (new_knot_position != knot_position) {
    knot_position = new_knot_position;
    
    //If knot_position <= -WIN_OFFSET then player 1 wins. If bias >= WIN_OFFSET then player 2 wins
    if (knot_position <= -WIN_OFFSET) {
      play_winner(1);
    } else if (knot_position >= WIN_OFFSET) {
      play_winner(2);
    }

    //update the RGB LED
    if (knot_position < 0) {
      //Player 1 is winning. Make the RGB LED YELLOW and make the brightness reflect the degree of winnage
      analogWrite(PIN_RGB_R, map(WIN_OFFSET + knot_position, WIN_OFFSET, 0, 0, 255));
      analogWrite(PIN_RGB_G, map(WIN_OFFSET + knot_position, WIN_OFFSET, 0, 0, 64));
      analogWrite(PIN_RGB_B, 0);
    } else if (knot_position > 0) {
      //Player 2 is winning. Make the RGB LED RED and make the brightness reflect the degree of winnage
      analogWrite(PIN_RGB_R, map(WIN_OFFSET - knot_position, WIN_OFFSET, 0, 0, 255));
      analogWrite(PIN_RGB_G, 0);
      analogWrite(PIN_RGB_B, 0);
    } else {
      analogWrite(PIN_RGB_R, 0);
      analogWrite(PIN_RGB_G, 0);
      analogWrite(PIN_RGB_B, 0);
    }
    
    Serial.print("Knot Pos: ");
    Serial.println(knot_position);
  }

  //Give the CPU a rest for 5ms!
  delay(5);
}

/**
 * Play the Intro sequence
 */
void play_intro() {
  //Turn off the RGB LED
  analogWrite(PIN_RGB_R, 0);
  analogWrite(PIN_RGB_G, 0);
  analogWrite(PIN_RGB_B, 0);

  //Flash the RGB LED Blue 3 times
  for (byte i = 0; i < 3; i++) {
    analogWrite(PIN_RGB_B, 255);
    delay(500);
    analogWrite(PIN_RGB_B, 0);
    delay(500);
  }

  //Turn the RGB LED Green for 1 second
  analogWrite(PIN_RGB_G, 255);
  delay(1000);

  //Turn off the RGB LED
  analogWrite(PIN_RGB_R, 0);
  analogWrite(PIN_RGB_G, 0);
  analogWrite(PIN_RGB_B, 0);
}

/**
 * Play the Winner sequence
 */
void play_winner(byte winning_player) {
  for (byte i = 0; i < 5; i++) {
    //Turn everything off
    digitalWrite(PIN_P1_LED_1, 0);
    digitalWrite(PIN_P1_LED_2, 0);
    digitalWrite(PIN_P2_LED_1, 0);
    digitalWrite(PIN_P2_LED_2, 0);
    analogWrite(PIN_RGB_R, 0);
    analogWrite(PIN_RGB_G, 0);
    analogWrite(PIN_RGB_B, 0);

    delay(500);

    //Turn on just the winner's LEDs
    digitalWrite(PIN_P1_LED_1, (winning_player == 1));
    digitalWrite(PIN_P1_LED_2, (winning_player == 1));
    digitalWrite(PIN_P2_LED_1, (winning_player == 2));
    digitalWrite(PIN_P2_LED_2, (winning_player == 2));

    if (winning_player == 1) {
      analogWrite(PIN_RGB_R, 255);
      analogWrite(PIN_RGB_G, 64);
    } else if (winning_player == 2) {
      analogWrite(PIN_RGB_R, 255);
    }

    delay(500);
  }

  //Loop infinately (force the user to reset the arduino)
  while (1) {
    delay(250);
  }
}

