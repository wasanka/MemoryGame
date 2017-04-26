#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#endif

byte level = 1;
byte leds = 4;
byte ledsForLevel = 1;
byte stepsForLevel = 1;

byte ledPins[] = {3, 4, 5, 6};
byte buttonPins[] = {8, 9, 10, 11};
byte buzzer = 12;

const int tones[] = {1915, 1700, 1519, 1432, 1300, 2700};

void setup() {
  // put your setup code here, to run once:

  randomSeed(analogRead(0));

  Serial.begin(9600);

  for (int i = 0; i < leds; i++) {

    pinMode(ledPins[i], OUTPUT);
    //pinMode(buttonPins[i], INPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(buzzer, OUTPUT);

}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(tone);
  }
}

void loop() {


  // put your main code here, to run repeatedly:

  initializeLevel();
  DEBUG_PRINT("complete");

  // trying to find random leds for this level. Same led can not repeat again.
  int randomLeds[ledsForLevel * stepsForLevel];

  for (int step = 1, index = 0; step <= stepsForLevel; step++) {

    //int values[ledsForLevel];
    int* values = new int[ledsForLevel];

    DEBUG_PRINT("Finding some ramdom leds " + String(ledsForLevel));
    DEBUG_PRINT("Finding some ramdom leds " + String(sizeof(values) / sizeof(int)));
    
    getRandomLEDs(values);
    
    
    for (int i = 0; i < ledsForLevel; i++) {

      randomLeds[index] = values[i];
      index++;
    }
  }

  int sizeOfRamdomLEDs = sizeof(randomLeds) / sizeof(int);

  DEBUG_PRINT("Size of random leds " + String(sizeof(randomLeds)));
  for (int i = 0; i < sizeOfRamdomLEDs; i++) {

    DEBUG_PRINT("Random leds for this level " + String(randomLeds[i]));
  }

  // Now we have random leds for this level. Lets show them to the player

  DEBUG_PRINT("Showing to player");
  for (int i = 0; i < sizeOfRamdomLEDs; i++) {
    digitalWrite(ledPins[randomLeds[i]], HIGH); // set the LED on
    playTone(tones[i], 500);
    //delay(1000); // wait for a second

    digitalWrite(ledPins[randomLeds[i]], LOW); // set the LED on
    delay(500); // wait for a second
  }

  delay(1000);
  digitalWrite(ledPins[0], HIGH); // set the LED on
  digitalWrite(ledPins[1], HIGH); // set the LED on
  digitalWrite(ledPins[2], HIGH); // set the LED on
  digitalWrite(ledPins[3], HIGH); // set the LED on
  delay(1000);
  digitalWrite(ledPins[0], LOW); // set the LED on
  digitalWrite(ledPins[1], LOW); // set the LED on
  digitalWrite(ledPins[2], LOW); // set the LED on
  digitalWrite(ledPins[3], LOW);
  //delay(1000);
  DEBUG_PRINT("Ready to acceppt players moves");

  // now lets waiting for player input
  int playerButtons[sizeOfRamdomLEDs];
  int playerButtonCount = 0;
  while (playerButtonCount < sizeOfRamdomLEDs) {

    for (int button = 0; button < leds; button++) {

      //                System.out.print(digitalRead(buttonPins[button]));

      //DEBUG_PRINT("Checking button " + String(button));

      if (digitalRead(buttonPins[button]) == LOW) {

        buttonPress(button);

        playerButtons[playerButtonCount] = button;
        playerButtonCount++;

        DEBUG_PRINT("Button pressed " + String(button));

        //delay(500);
        break;
      }
    }

    //            System.out.println();
  }

  boolean win = true;
  for (int i = 0; i < sizeOfRamdomLEDs; i++) {

    if(randomLeds[i] != playerButtons[i]){
      win = false;
      break;
    }
  }
  
  // Check if player got it correct
  if (win) {
    playTone(tones[5], 1000);
    displayWin();
    level++;
  } else {
    playTone(tones[4], 1000);
  }
}

void buttonPress(int button){

  digitalWrite(ledPins[button], HIGH);
  playTone(tones[button], 300);
  //delay(300);
  digitalWrite(ledPins[button], LOW);
}

boolean isWin(int leds[], int buttons[]) {

  DEBUG_PRINT("Checking if player won");
  DEBUG_PRINT("Length " + String(sizeof(leds) / sizeof(int)));

  for (int i = 0; i < sizeof(leds) / sizeof(int); i++) {

    DEBUG_PRINT("LED " + String(leds[i] + " BUTTON " + String(buttons[i])));

    if (leds[i] != buttons[i]) {

      return false;
    }
  }

  return true;
}

void reset() {

  level = 1;
  ledsForLevel = 1;
  stepsForLevel = 1;
}

void initializeLevel() {

  DEBUG_PRINT("");
  DEBUG_PRINT("initializing level " + String(level));

  switch (level) {
    case 1:
    case 2:
    case 3:
    case 4:
      ledsForLevel = level;
      break;
    case 5:
    case 6:
    case 7:
    case 8:
      ledsForLevel = (byte) (level - 4);
      stepsForLevel = 2;
      break;
    case 9:
    case 10:
    case 11:
    case 12:
      ledsForLevel = (byte) (level - 8);
      stepsForLevel = 3;
      break;
    case 13:
    case 14:
    case 15:
    case 16:
      ledsForLevel = (byte) (level - 12);
      stepsForLevel = 4;
      break;
    case 17:
    case 18:
    case 19:
    case 20:
      ledsForLevel = (byte) (level - 16);
      stepsForLevel = 5;
      break;
    case 21:
    case 22:
    case 23:
    case 24:
      ledsForLevel = (byte) (level - 20);
      stepsForLevel = 5;
      break;
    case 25:
      displayWin();
      break;
  }

  DEBUG_PRINT("ledsForLevel "+String(ledsForLevel));
  DEBUG_PRINT("stepsForLevel "+String(stepsForLevel));
  DEBUG_PRINT("");
}

void displayWin() {

  Serial.println("player won this level");
  //TODO  Winining melody

  for (int i = 0; i < 10; i++) {

    delay(100);
    digitalWrite(ledPins[0], HIGH); // set the LED on
    digitalWrite(ledPins[1], HIGH); // set the LED on
    digitalWrite(ledPins[2], HIGH); // set the LED on
    digitalWrite(ledPins[3], HIGH); // set the LED on
    delay(100);
    digitalWrite(ledPins[0], LOW); // set the LED on
    digitalWrite(ledPins[1], LOW); // set the LED on
    digitalWrite(ledPins[2], LOW); // set the LED on
    digitalWrite(ledPins[3], LOW);
    delay(100);
  }

  //reset();
}

void getRandomLEDs(int randomLeds[]) {

  DEBUG_PRINT("Random leds to find " + String(sizeof(randomLeds) / sizeof(int)));
  String randomValues = "";
  //int randomLeds[ledsForLevel];
  DEBUG_PRINT("ledsForLevel " + String(ledsForLevel));

  for (int i = 0; i < ledsForLevel; i++) {

    int randomLed = random(leds);
    DEBUG_PRINT("random " + String(randomLed));

    while (true) {

      DEBUG_PRINT("inside loop... '" + randomValues + "'");

      if (randomValues.indexOf(String(randomLed)) == -1) {
        randomValues = randomValues + String(randomLed) + " ";

        randomLeds[i] = randomLed;
        DEBUG_PRINT("Found value " + String(randomLed));
        
        break;
      }

      randomLed = random(leds);
    }

    DEBUG_PRINT("Random leds found " + String(sizeof(randomLeds) / sizeof(int)));
    //randomLeds[i] = random(leds);
  }
}
