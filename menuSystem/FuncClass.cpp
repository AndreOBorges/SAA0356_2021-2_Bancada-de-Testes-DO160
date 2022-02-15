  #include "Arduino.h"
#include "FuncClass.h"
#include "MMClass.h"

extern void LCDPrint();
extern bool inFunction;
extern String specificContent[4];
extern float output;
extern bool emerButtonState;
extern Vector<float> voltageReading;
extern Vector<unsigned long> timeReading;
extern String lastTest;


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
    if (firstIteration) lastTest = testNumber;
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
  unsigned long stateChangeTime; //variável que permite saber quando houve troca de estado
  int funcStage = 1;
  int lastFuncStage = 0;
  bool inTest = true;
  while(inTest && !emerButtonState) {
    if (funcStage != lastFuncStage) {
      stateChangeTime = millis();
      lastFuncStage = funcStage;
      //leitura antes da mudança de tensão
      voltageReading.push_back(analogRead(A0));
      timeReading.push_back(millis() - start);
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
      //leitura pós mudança de tensão
      for (int i = 0; i < 10; i++) {
        voltageReading.push_back(analogRead(A0));
        timeReading.push_back(millis() - start);
      }
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
      //leitura amostral
      voltageReading.push_back(analogRead(A0));
      timeReading.push_back(millis() - start);
    }

    if(millis() - stateChangeTime > 500) {
      float voltDiff;
      voltDiff = voltageReading.getElement(voltageReading.size() - 2) - voltageReading.getElement(voltageReading.size() - 1);
      if(voltDiff > 0.1 || voltDiff < -0.1) {
        voltageReading.push_back(analogRead(A0));
        timeReading.push_back(millis() - start);
      }
    }
  }
}


/*
//DESTRINCHAR ESSA FUNÇÃO EM VÁRIAS FUNÇÕES
float testPerSe(int testNumber) {
  float output;
  switch (testNumber) {

    case 2:  //Teste 16.6.1.2
      break;

    case 3: //Teste 16.6.1.3
      break;

    case 4: //Teste 16.6.1.4
      output = 28/4;
      delay(300000);
        for (int i = 0; i < 3; i++) {
          output = 47/4;
          delay(5);
          output = 40/4;
          delay(30);
          output = 28/4;
          delay(5000);
          output = 17/4;
          delay(30);
          if (i != 0) {
            output = 28/4
            delay(5);
          }
        }
      onTest = false;
      break;

    case 5: //Teste 16.6.1.5
      output = 28/4;
      delay(10000);
      output = 10/4;
      while (output < 19/4) { //Entra no loop de subida tensão de 0.3V/s
        int timer = millis();
        output += 0.3/4;
        while (timer - milis() < 1000);
      }
      output = 28/4;
      onTest = false;
      break;

    case 6: //Teste 16.6.2.1
      output = 28/4;
      delay(10000);
      output = 32.2/4;
      delay(300000);
      output = 28/4;
      delay(60000);
      output = 20.5/4;
      delay(300000);
      output = 28/4;
      delay(10000);
      onTest = false;
      break;

    case 7: //Teste 16.6.2.2
      output = 28/4;
      delay(300000);
      output = 15/4; //Tensão mínima, a definir
      delay(300000);
      output = 0;
      delay(600000);
      output = 28/4;
      delay(10000);
      onTest = false;
      break;

    case 8: //Teste 16.6.2.3
      output = 28/4;
      delay(10000);
      output = 12/4;
      delay(7000);
      output = 28/4;
      delay(10000);
      onTest = false;
      break;

    case 9: //Teste 16.6.2.4
      output = 28/4; //Para esse teste existem restrições no tempo que demora a variação da tensão. Portanto
      delay(300000); //será necessário medir e verificar se a variação normal da fonte já é o suficiente
        for (int i = 0; i < 3; i++) {
          output = 60/4;
          delay(100);
          output = 40/4;
          delay(1000);
          if (i != 0) {
            output = 28/4
            delay(10000);
          }
        }
      onTest = false;
      break;
  }
}

*/
