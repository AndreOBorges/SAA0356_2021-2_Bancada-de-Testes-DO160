#include "Arduino.h"
#include "FuncClass.h"
//#include "MenuClass.h"
#include "MMClass.h"

extern void LCDPrint();
extern bool inFunction;
extern String specificContent[4];

    Function::Function(String testNumber, String testTitle, String totalTime, MM* menuManager) 
      :testNumber(testNumber), testTitle(testTitle), totalTime(totalTime), menuManager(menuManager)
    {
      isTest = true;
      firstIteration = true;
      funcStage = 0;
    }
    
    Function::Function(String testTitle, MM* menuManager)
      :testTitle(testTitle), menuManager(menuManager)
    {
      isTest = false;
      firstIteration = true;
    }
    
    void Function::funcManager() {
      
      menuManager->zeroInputs();
      
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
      specificContent[0] = "T. " + testNumber + "in exec.";
      specificContent[1] = "CVT:      CVM:      ";
      specificContent[2] = "Progress(%): ";
      specificContent[3] = " PRESS LEFT TO STOP";      
    }

//    static String* Function::getSpecificContent() {
//      return specificContent;
//    }
