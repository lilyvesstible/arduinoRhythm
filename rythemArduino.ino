#include <Adafruit_CircuitPlayground.h>

int leftButt = 4;
int rightButt = 5;

bool leftPress = 0;
bool rightPress = 0;

void setup() {
  CircuitPlayground.begin();
  pinMode(leftButt, INPUT_PULLDOWN);
  pinMode(rightButt, INPUT_PULLDOWN);
  Serial.begin(9600); 

  attachInterrupt(digitalPinToInterrupt(leftButt), leftBeat, FALLING);
  attachInterrupt(digitalPinToInterrupt(rightButt), rightBeat, FALLING);
}

void loop() {
  Serial.println("boop");
  beatCycle(2, 4, 0, 2, 1000);
  delay(100000);

}

void beatCycle(int leftUI, int rightUI, int leftCycleUI, int rightCycleUI, int timeUI){
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
  int time = timeUI;
  // Signels the last beat (for the player to hit)
  bool leftReady = 0;
  bool rightReady = 0;

  if(leftCycle == 0){
    leftReady = 1;
  }
  
  if(rightCycle == 0){
    rightReady = 1;
  }

  Serial.println(rightCycle);
//add right cycle too
  while(leftCycle != 0 && rightCycle != 0){
    //If it isn't the last beat, run through beats. Otherwise, signel that the player can hit the last beat
    if(leftReady == 0){
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(leftCounter, 255, 0, 0);
      //If the beat counter is up, signel last beat. Else, decrement counter
      if(leftCounter == 0){
        leftReady = 1;
      }
      else{
        leftCounter--;
      }
    }
    else if(leftReady){
      CircuitPlayground.clearPixels();      
      leftPress = 1;
    }

    if(rightReady == 0){
      CircuitPlayground.clearPixels();
      Serial.println(9-rightCounter);
      CircuitPlayground.setPixelColor(9-rightCounter, 255, 0, 0);
      //If the beat counter is up, signel last beat. Else, decrement counter
      if(rightCounter == 0){
        rightReady = 1;
      }
      else{
        rightCounter--;
      }
    }
    else if(rightReady){
      CircuitPlayground.clearPixels();      
      rightPress = 1;
    }

    delay(time);
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
  }
}

void leftBeat(){
  if(leftPress){
    Serial.println("Congrats! +1 point");
  }
  else{
    Serial.println("Skill issue. -1 point");
  }
}

void rightBeat(){
  if(rightPress){
    Serial.println("Congrats! +1 point");
  }
  else{
    Serial.println("Skill issue. -1 point");
  }
}