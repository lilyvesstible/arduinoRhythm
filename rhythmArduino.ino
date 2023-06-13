#include <Adafruit_CircuitPlayground.h>

//Button pins
int leftButt = 4;
int rightButt = 5;

//Checks when pressing the button will reward you a point
bool leftPress = 0;
bool rightPress = 0;

//Sound requirement for sound sensor portion
int soundReq = 75;
float sound = 0;

int score = 0;
int scoreMax = 0;

int levelCount = 0;
int levelMax = 3;
bool levelCheck = 1;
bool levelStart = 0;

bool tutorial = 1;

//My program is acting weird, and it's outputting the first instance of the sound sensor as 52 every time.
//For now, this is a quick fix to waste the first instance on nothing.
float garbage = 0;

void setup() {
  CircuitPlayground.begin();
  pinMode(leftButt, INPUT_PULLDOWN);
  pinMode(rightButt, INPUT_PULLDOWN);
  Serial.begin(9600); 

  attachInterrupt(digitalPinToInterrupt(leftButt), leftBeat, FALLING);
  attachInterrupt(digitalPinToInterrupt(rightButt), rightBeat, FALLING);

    
  while(!Serial){

  }
}

void loop() {  
  if(tutorial){
    Serial.println("Welcome to Rhythm Arduino! Here are the rules: The left or right (or sometimes both) side of the arduino will countdown through it's LEDs and will make a beeping sound.");
    Serial.println("When no LEDs are lit, that's when you press the button that's on the same side as the LED that was just lit.");
    Serial.println("If the LED ticks yellow, you need to yell into the arduino as loud as possible during the right time instead.");
    Serial.println("After the first stage, each others are designed to match with a song. Try to guess the song!");
    Serial.println("To select a stage, press the right button to cycle through the stages, and press the left button to start the stage. A green countdown will begin to let you get ready.");
    tutorial = 0;
  }
  levelSelect();
}

void beatCycle(int leftUI, int rightUI, int leftCycleUI, int rightCycleUI, float bpmUI){
  //Number of beats in a cycle
  int leftNum = leftUI - 2;
  int rightNum = rightUI - 2;
  //Number of cycles in a full beat cycle
  int leftCycle = leftCycleUI;
  int rightCycle = rightCycleUI;
  //A counter for the number of beats that can be modified
  int leftCounter = leftNum;
  int rightCounter = rightNum;
  //Time in between beats
  float time = 60000/bpmUI;
  // Signals the last beat (for the player to hit)
  bool leftReady = 0;
  bool rightReady = 0;

  if(leftNum < 0){
    leftNum = 0;
  }
  
  if(rightNum < 0){
    rightNum = 0;
  }

  //if(leftCycle == 0){
    //leftReady = 1;
  //}
  
  //if(rightCycle == 0){
    //rightReady = 1;
  //}

  while(((leftCycle != 0) || (rightCycle != 0)) || ((leftCycle != 0) && (rightCycle != 0))){
    //If it isn't the last beat, run through beats. Otherwise, signel that the player can hit the last beat
    if(leftCycle != 0){
      if(leftReady == 0){
        CircuitPlayground.setPixelColor(leftCounter, 20, 0, 0);
        if(rightCycle == 0){
          CircuitPlayground.playTone(659.26, 50);
        }
        else if(rightReady == 1){
          CircuitPlayground.playTone(493.88, 50);
        }
        //If the beat counter is up, signel last beat. Else, decrement counter
        if(leftCounter == 0){
          leftReady = 1;
        }
        else{
          leftCounter--;
        }
      }
      else if(leftReady){    
        leftPress = 1;
      }
    }
    //Repeat for right side
    if(rightCycle != 0){
      if(rightReady == 0){        
        CircuitPlayground.setPixelColor(9-rightCounter, 20, 0, 0);
        if(leftCycle == 0){
          CircuitPlayground.playTone(329.62, 50);
        }
        else{
          CircuitPlayground.playTone(493.88, 50);
        }
        //If the beat counter is up, signel last beat. Else, decrement counter
        if(rightCounter == 0){
          rightReady = 1;
        }
        else{
          rightCounter--;
          
        }
      }
      else if(rightReady){    
        rightPress = 1;
      }
    }
    delay(time-50);        
    //If this is at the end of the last beat, reset everything and decrement cycle
    if(leftPress){
      leftPress = 0;
      leftReady = 0;
      leftCounter = leftNum;
      leftCycle--;
    }
    if(rightPress){
      rightPress = 0;
      rightReady = 0;
      rightCounter = rightNum;
      rightCycle--;
    }
    CircuitPlayground.clearPixels();
  }
}

void leftBeat(){
  if(leftPress){
    //Serial.println("Congrats! +1 point");
    score++;
    Serial.println(score); 
    leftPress = 0;
  }
  else{
    Serial.println("Skill issue.");
  }

  if(levelCheck){
    levelStart = 1;
  }
}

void rightBeat(){
  if(rightPress){
    //Serial.println("Congrats! +1 point");   
    rightPress = 0;
    score++;
    Serial.println(score); 
  }
  else{
    Serial.println("Skill issue.");
  }

  if(levelCheck){
    levelCount++;
    if(levelCount == levelMax){
      levelCount = 0;
    }
    CircuitPlayground.clearPixels();    
  }
}

void countdown(int bpmUI){
  int time = 60000/bpmUI;

  garbage = CircuitPlayground.mic.soundPressureLevel(0);
  levelStart = 0;
  levelCheck = 0;  
  score = 0;
  scoreMax = 0;
  for(int i=0; i < 3; i++){
    CircuitPlayground.setPixelColor(2-i, 0, 20, 0);
    CircuitPlayground.setPixelColor(7+i, 0, 20, 0);
    delay(time);
    CircuitPlayground.clearPixels();
  }
  delay(time);
}

void yellBeat(int beatUI, int bpm){
  int beatNum = beatUI - 2;
  int time = 60000/bpm;

  for(int i=0; i <= beatNum; i++){
    sound = CircuitPlayground.mic.soundPressureLevel(1);
    if(sound >= soundReq){
      Serial.println("Skill issue."); 
    }
    sound = 0;

    CircuitPlayground.setPixelColor(beatNum-i, 20, 20, 0);
    CircuitPlayground.setPixelColor((9-beatNum)+i, 20, 20, 0);
    CircuitPlayground.playTone(164.61, 50);
    delay(time-50);
    CircuitPlayground.clearPixels();
  }
  sound = CircuitPlayground.mic.soundPressureLevel(50);
  if(sound >= soundReq){
    //Serial.println("Big voice! +2 points");
    score = score + 2;
    Serial.println(score); 
  }
  else{
    Serial.println("Get Gud.");    
  }
  sound = 0;
  delay(time-50);
}

void scoreCalc(){
  Serial.println(score);
  Serial.println(scoreMax);
  score = map(score, 0, scoreMax, 0, 100);
  Serial.println(score);
  for(int i = 0; i<10; i++){
    if(score >= (i+1)*10){
      CircuitPlayground.setPixelColor(i, 0, 20, 0);
      delay(50);      
    }
  }
  delay(10000);
  CircuitPlayground.clearPixels();
  levelCheck = 1;
}

void levelSelect(){
  CircuitPlayground.setPixelColor(levelCount, 20, 0, 0);

  if(levelStart){
    if(levelCount == 0){
      CircuitPlayground.clearPixels();
      levelZero();
    }
    else if(levelCount == 1){
      CircuitPlayground.clearPixels();
      levelOne();
    }
    else if(levelCount == 2){
      CircuitPlayground.clearPixels();
      levelTwo();      
    }
  }
}

void beatlessWait(int beatUI, int bpmUI){
  delay((60000/bpmUI)*beatUI);  
}

//Tutorial
void levelZero(){
  countdown(120);
  scoreMax = 46;
  beatCycle(4, 0, 4, 0, 120);
  beatCycle(0, 4, 0, 4, 120);
  beatCycle(4, 4, 4, 4, 120);
  beatCycle(4, 2, 4, 8, 120);  
  beatCycle(2, 4, 8, 4, 120);
  beatCycle(4, 0, 2, 0, 240);
  beatCycle(0, 4, 0, 2, 240);
  yellBeat(5, 120);
  scoreCalc();
}

//Flamingo - Kero Kero Bonito
void levelOne(){
  countdown(89);
  scoreMax = 28;  
  beatCycle(4, 0, 2, 0, 178);
  beatCycle(0, 4, 0, 2, 178);
  beatCycle(4, 0, 2, 0, 178);
  beatCycle(0, 5, 0, 1, 178);
  beatlessWait(3, 178);
  beatCycle(4, 0, 4, 0, 89);
  beatCycle(0, 4, 0, 4, 89);
  beatCycle(4, 0, 3, 0, 89);
  beatCycle(2, 0, 1, 0, 89);  
  beatCycle(0, 4, 0, 1, 178);
  beatCycle(0, 4, 0, 3, 89);
  beatCycle(4, 4, 1, 1, 178);
  yellBeat(3, 178);
  scoreCalc();
}

//What you won't do for love - Bobby Caldwell
void levelTwo(){
  countdown(84);
  scoreMax = 27;
  beatCycle(2, 2, 1, 1, 84);
  beatCycle(4, 0, 3, 0, 84);
  beatCycle(6, 0, 1, 0, 336);
  beatCycle(0, 2, 0, 1, 336);
  beatlessWait(2, 84);
  beatCycle(0, 4, 0, 4, 84);
  beatCycle(4, 0, 4, 0, 84);
  beatCycle(4, 4, 4, 4, 84);  
  beatCycle(4, 0, 1, 0, 84);
  beatCycle(0, 4, 0, 1, 84);
  beatCycle(4, 4, 1, 1, 84);
  beatlessWait(2, 84);
  beatCycle(3, 3, 1, 1, 84);
  scoreCalc();  
}
