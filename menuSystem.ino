#include "MenuClass.h"
#include "MMClass.h"
#include "VectorClass.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
bool left, right, up, down; //bool dos botões, quando recebe input ele vira 1
bool haveInput = false; //quando algum dos botóes da positivo, esse bool vai pra positivo, permitindo que uma nova impressão de menus seja feita, já com a atualização do input
bool printContent = false; //dentro das funções, o conteúdo apresentado vai ser diferente dos menus. Avaliar se não é o caso de usar inFunction ao invés de printContent
bool inFunction = false; //variável que vai para true assim que uma função começa a ser executada. É responsabilidade da função receber o input "left" e colocar o inFunction em false
String specificContent[4]; //quando entra em uma função, o testo printado não são os menus, e por isso é necessário que a função preencha o que deve ser printado na tela

void LCDPrint();
void userInput();
//class Function;

/*
//classe função
class Function {
  private:
    int funcStage; //para funções de vários stages
    bool firstIteration; //bool que começa true e fica false a partir da primeira iteração
    bool isTest; //talvez seja necessária essa bool, pra indicar que trata-se de um teste
    String testNumber;
    String testTitle;
    String totalTime;
    MM* menuManager;
    
  public:
    Function(String testNumber, String testTitlele, String totalTime) 
      :testNumber(testNumber), testTitlele(testTitlele), totalTime(totalTime)
    {
      isTest = true;
      firstIteration = true;
      funcStage = 0;
    }

    void funcManager() {
      up = 0;
      down = 0;
      left = 0;     
      right = 0;    
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
      
      if (haveInput) {
        LCDPrint();
      }
      
      userInput();
      
      if (left == 1) {
        if (funcStage == 0 || funcStage == 2) {
          funcStage = 0;
          menuManager->prevMenu();     
          inFunction = false;
          LCDPrint();
        }
        else {
          funcStage--;
        }
      }
      else if (right == 1) {
        if (funcStage != 2) {
          funcStage++;    
        }
      }
    }
    
    void stageZero() {
      specificContent[0] = "Test" + testNumber;
      specificContent[1] = testTitle;
      specificContent[2] = "Total time: " + totalTime;
      specificContent[3] = "<-Back     To Test->";
    }
    
    void stageOne() {
      specificContent[0] = "Only emergency can ";
      specificContent[1] = "stop the test. Click";
      specificContent[2] = "   right to begin";
      specificContent[3] = "<-Back       Begin->";      
    }

    void stageTwo() {
      specificContent[0] = "T. " + testNumber + "in exec.";
      specificContent[1] = "CVT:      CVM:      ";
      specificContent[2] = "Progress(%): ";
      specificContent[3] = " PRESS LEFT TO STOP";      
    }
};
*/
Menu mainMenu("Main menu");
MM mm(&mainMenu); 
Menu tests("Tests"), test1("16.6.1.1"), test2("16.6.1.2"), test3("16.6.1.3"), test4("16.6.1.4"), test5("16.6.1.5"), test6("16.6.2.1"), test7("16.6.2.2"), test8("16.6.2.3"), test9("16.6.2.4"), retData("Retrieve Data"),  info("Info"/*, infoDisplay*/);
//Function testOne("16.6.1.1", "Voltage (avr value)", "91");

void setup() {
  mainMenu.addSubMenu(&tests);
  mainMenu.addSubMenu(&retData);
  mainMenu.addSubMenu(&info);
  tests.addSubMenu(&test1);
  tests.addSubMenu(&test2);
  tests.addSubMenu(&test3);
  tests.addSubMenu(&test4);
  tests.addSubMenu(&test5);
  tests.addSubMenu(&test6);
  tests.addSubMenu(&test7);
  tests.addSubMenu(&test8);
  tests.addSubMenu(&test9);
  info.addFunction(infoDisplay);
  test2.addFunction(test16611);
  
  /*pciSetup(8);
  pciSetup(9);
  pciSetup(10);
  pciSetup(11);*/
  
  lcd.init();// initialize the lcd 
  lcd.backlight();
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  LCDPrint();
}

void loop() {
  if (!inFunction) {
    userInput();  
    if(haveInput)
    {
      LCDPrint();
    }
  }
  else if (inFunction){
    mm.getCurrentMenu()->executeFunction();
  }
}

void LCDPrint() {
  if (!inFunction) {
    mm.displayOrganizer();  
     lcd.clear();
    for (int i = 0; i < 4; i++) {
      if (i == mm.getCurrentLine()){
        lcd.setCursor(1, i);
        lcd.print(mm.getCurrentDisplay(i));
        lcd.setCursor(0, mm.getCurrentLine());
        lcd.print(">");
      }
      else {
        lcd.setCursor(1, i);
        lcd.print(mm.getCurrentDisplay(i));
      }
    }
  }
  else {
    mm.displayOrganizer(inFunction, specificContent);
    lcd.clear();
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(0, i);
      lcd.print(mm.getCurrentDisplay(i));
    }
  }
  haveInput = false;
  delay(500);
}


void infoDisplay() {
  
  if (mm.getCurrentMenu()->getProgramCounter() == 0) {
    delay(500);
    mm.getCurrentMenu()->addProgramCounter();
    up = 0;
    down = 0;
    left = 0;     
    right = 0;
    }
  
  if (!left) {
    specificContent[0] = "";
    specificContent[1] = "  Information menu";
    specificContent[2] = "     V1.0  beta";
    specificContent[3] = "";
  }
  else {
    mm.prevMenu();
    inFunction = false; 
    mm.getCurrentMenu()->setProgramCounter(0);
    LCDPrint();    
  }
  if (haveInput) {
    LCDPrint();
    LCDPrint();
  }
  userInput();
}
  

void test16611() {
  if (mm.getCurrentMenu()->getProgramCounter() == 0) {
    mm.getCurrentMenu()->setFuncStage(0);
      specificContent[0] = "Test 16.6.1.1";
      specificContent[1] = "Voltage (avr value)";
      specificContent[2] = "Total time: 91min";
      specificContent[3] = "<-Back     To Test->";    
  }

  if(mm.getCurrentMenu()->getProgramCounter() != 0) {
  switch(mm.getCurrentMenu()->getFuncStage()) {
    case 0:
      specificContent[0] = "Test 16.6.1.1";
      specificContent[1] = "Voltage (avr value)";
      specificContent[2] = "Total time: 91min";
      specificContent[3] = "<-Back     To Test->";
    break;

    case 1:
      specificContent[0] = "Only emergency can ";
      specificContent[1] = "stop the test. Click";
      specificContent[2] = "   right to begin";
      specificContent[3] = "<-Back       Begin->";
    break;
    
    case 2:
      specificContent[0] = "T. 16.6.1.1 in exec.";
      specificContent[1] = "CVT:      CVM:      ";
      specificContent[2] = "Progress(%): ";
      specificContent[3] = " PRESS LEFT TO STOP";
  }
  }
  if (haveInput) {
    LCDPrint();
  }
  /*
  if (programCounter == 0) {
    delay(500);
    programCounter++;
    }*/
  if (mm.getCurrentMenu()->getProgramCounter() == 0) {
    delay(500);
    mm.getCurrentMenu()->addProgramCounter();
    mm.getCurrentMenu()->setFuncStage(0);
   
    up = 0;
    down = 0;
    left = 0;     
    right = 0;
    }  
    
  userInput();
  
  if (left == 1) {
    if (mm.getCurrentMenu()->getFuncStage() == 0 /*|| mm.getCurrentMenu()->getFuncStage() == 2*/) {
      mm.getCurrentMenu()->subFuncStage();
      mm.getCurrentMenu()->setProgramCounter(0);      
      mm.prevMenu();     
      inFunction = false;
      LCDPrint();
    }
    else {
      mm.getCurrentMenu()->subFuncStage();
    }
  }
  else if (right == 1) {
    if (mm.getCurrentMenu()->getFuncStage() != 2) {
      mm.getCurrentMenu()->addFuncStage();
    }
  }
}  

void userInput() {
  left = digitalRead(8);
  right = digitalRead(9);
  up = digitalRead(10);
  down = digitalRead(11);

  if (left == 1) {
    haveInput = true;
    if (!inFunction) {
      mm.prevMenu();
    }
  }
  else if (right == 1) {
    haveInput = true;
    if (!inFunction) {
      mm.nextMenu();
    }
  }     
  else if (up == 1){
    haveInput = true;
    if (!inFunction) {
      mm.prevSelection();
    }
  }     
  else if (down == 1) {
    haveInput = true;
    if (!inFunction) {
      mm.nextSelection();
    }
  }
}


/*
void userInput() {
  if (left == 1) {
    haveInput = true;
    if (!inFunction) {
      mm.prevMenu();
    }
    else if (inFunction) {
      mm.getCurrentMenu()->executeFunction();
    } 
  }
  else if (right == 1) {
    haveInput = true;
    if (!inFunction) {
      mm.nextMenu();
    }
    else if (inFunction) {
      mm.getCurrentMenu()->executeFunction();
    }
  }
  
  else if (up == 1){
    haveInput = true;
    if (!inFunction) {
      mm.prevSelection();
    }
    else if (inFunction) {
      mm.getCurrentMenu()->executeFunction();
    }
  }
  else if (down == 1) {
    haveInput = true;
    if (!inFunction) {
      mm.nextSelection();
    }
    else if (inFunction) {
      mm.getCurrentMenu()->executeFunction();
    }    
  }
}
*/
  
/*
//DESTRINCHAR ESSA FUNÇÃO EM VÁRIAS FUNÇÕES
float testPerSe(int testNumber) {
  float output;
  switch (testNumber) {
    case 1: //Teste 16.6.1.1
      output = 28/4;
      delay(10000);
      output = 30.3/4;
      delay(1800000);
      output = 28/4;
      delay(60000);
      output = 22/4;
      delay(1800000);
      output = 18/4;
      delay(1800000);
      output = 28/4;
      delay(10000);
      onTest = false;
      break;

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

/*
void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

ISR (PCINT0_vect) {
    left = digitalRead(8);
    right = digitalRead(9);
    up = digitalRead(10);
    down = digitalRead(11);
    haveInput = true;
}

*/
