#include "Arduino.h"
#include "FuncClass.h"
#include "MMClass.h"

extern void LCDPrint();
extern bool inFunction;
extern String specificContent[4];
extern float output;
extern bool emerButtonState;

Function::Function(void (*function)(), String testNumber, String testTitle, String totalTime, MM* menuManager) 
  :function(function), testNumber(testNumber), testTitle(testTitle), totalTime(totalTime), menuManager(menuManager)
{
  isTest = true;
  firstIteration = true;
  funcStage = 0;
}

Function::Function(void (*function)(), String testTitle, MM* menuManager)
  :function(function), testTitle(testTitle), menuManager(menuManager)
{
  isTest = false;
  firstIteration = true;
}

bool Function::getFirstIteration() {return firstIteration;}

void Function::setFirstIteration(bool setParameter) {firstIteration = setParameter;}

void Function::funcManager() {
  
  menuManager->zeroInputs();

  if(!isTest) {
    function();
  }
  else {
    switch(funcStage) {
      case 0:
        stageZero();
        break;
      case 1:
        stageOne();
        break;
      case 2:
        stageTwo();
        break;
    }
    
    if (menuManager->getHaveInput() || firstIteration) {
      LCDPrint();
      firstIteration = false;
    }
    
    menuManager->userInput();
    
    if (menuManager->checkLeft() == 1) {
      menuManager->setHaveInput(true);
      if (funcStage == 0 || funcStage == 2) {
        funcStage = 0;
        inFunction = false;
        firstIteration = true;
        menuManager->prevMenu();     
        LCDPrint();
      }
      else {
        funcStage--;
      }
    }
    else if (menuManager->checkRight() == 1) {
      if (funcStage != 2) {
        menuManager->setHaveInput(true);
        funcStage++;    
      }
    }
  }
}

void Function::stageZero() {
  specificContent[0] = "Test" + testNumber;
  specificContent[1] = testTitle;
  specificContent[2] = "Total time: " + totalTime;
  specificContent[3] = "<-Back     To Test->";
}

void Function::stageOne() {
  specificContent[0] = "Only emergency can ";
  specificContent[1] = "stop the test. Click";
  specificContent[2] = "   right to begin";
  specificContent[3] = "<-Back       Begin->";      
}

void Function::stageTwo() {
  function();
  funcStage = 0;
  specificContent[0] = "Test" + testNumber;
  specificContent[1] = testTitle;
  specificContent[2] = "Total time: " + totalTime;
  specificContent[3] = "<-Back     To Test->";
  LCDPrint();
}

void testOneFunc()
{

  int percentage = 0;  
  specificContent[0] = "T. 16.6.1.1 in exec.";
  specificContent[1] = "CVT: " + String(output/53) + " CVM:      ";
  specificContent[2] = "Progress(%): " + String(percentage) +"%";
  specificContent[3] = " PRESS EMER TO STOP";

  emerButtonState = false;
  unsigned long start = millis();
  unsigned long checkTime = millis(); //variável que permite checagens intervaladas
  int funcStage = 1;
  int lastFuncStage = 0;
  bool inTest = true;
  while(inTest && !emerButtonState) {
    if (funcStage != lastFuncStage) {
      lastFuncStage = funcStage;
      switch(funcStage) {
        case 1:
          output = 73;
          break;
        case 2:
          output = 75; //Considerando 255 = 4.92V, 75 é 1.5V, que é 30.3V/20 (30.3V é a primeira tensão do teste, e 20 o fator de multiplicação)
          break;
        case 3:
          output = 73;
          break;
        case 4:
          output = 57;
          break;
        case 5:
          output = 47;
          break;
        case 6:
          output = 0;
          inTest = false;
          break;
      }
      analogWrite(6, output);
      specificContent[1] = "CVT: " + String(output/53) + " CVM:      ";
      LCDPrint();
    }
    if(millis() - start < 1000) funcStage = 1;
    else if(millis() - start < 4000) funcStage = 2;
    else if(millis() - start < 10000) funcStage = 3;
    else if(millis() - start < 13000) funcStage = 4;
    else if(millis() - start < 16000) funcStage = 5;
    else funcStage = 6;

    if(millis() - checkTime > 1600) {
      percentage += 10;
      checkTime = millis();
      specificContent[2] = "Progress(%): " + String(percentage) +"%";
      LCDPrint();
    }
  }
}
