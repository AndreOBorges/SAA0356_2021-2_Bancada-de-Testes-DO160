#include "MenuClass.h"
#include "MMClass.h"
#include "VectorClass.h"
#include "FuncClass.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
//bool left, right, up, down; //bool dos botões, quando recebe input ele vira 1
//bool printContent = false; //dentro das funções, o conteúdo apresentado vai ser diferente dos menus. Avaliar se não é o caso de usar inFunction ao invés de printContent
bool inFunction = false; //variável que vai para true assim que uma função começa a ser executada. É responsabilidade da função receber o input "left" e colocar o inFunction em false
String specificContent[4]; //quando entra em uma função, o testo printado não são os menus, e por isso é necessário que a função preencha o que deve ser printado na tela
float output;
bool emerButtonState = false;

void LCDPrint();
void infoDisplay();
//void userInput();


Menu mainMenu("Main menu");
MM mm(&mainMenu); 
Function testOne(testOneFunc, "16.6.1.1", "Voltage (avr value)", "91", &mm);
Function infoFunc(infoDisplay, "Info display", &mm);
Menu tests("Tests"), test1("16.6.1.1", &testOne), test2("16.6.1.2"), test3("16.6.1.3"), test4("16.6.1.4"), test5("16.6.1.5"), test6("16.6.2.1"), test7("16.6.2.2"), test8("16.6.2.3"), test9("16.6.2.4"), retData("Retrieve Data"),  info("Info", &infoFunc);

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
//  info.addFunction(infoDisplay);
//  test2.addFunction(test16611);
    
  lcd.init();// initialize the lcd 
  lcd.backlight();
  pinMode(2, INPUT_PULLUP);             //pino de interrupção
  pinMode(6, OUTPUT);                   //pino de saída de tensão
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  LCDPrint();
  attachInterrupt(digitalPinToInterrupt(2), emerButton, CHANGE);
}

void loop() {
  if (!inFunction) {
    mm.userInput();  
    if(mm.getHaveInput() && !inFunction)
    {
      LCDPrint();
    }
  }
  else if (inFunction){
    mm.getCurrentMenu()->getFunction()->funcManager();
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
  mm.setHaveInput(false);
  delay(500);
}

void emerButton()
{
  output = 0;
  analogWrite(6, output);
  emerButtonState = true;
}


void infoDisplay() {
  if (infoFunc.getFirstIteration()) {
    specificContent[0] = "";
    specificContent[1] = "  Information menu";
    specificContent[2] = "     V1.0  beta";
    specificContent[3] = "";
    infoFunc.setFirstIteration(false);
    LCDPrint();
  }
  
  mm.userInput();
  
  if (mm.checkLeft() == 1) {
    mm.setHaveInput(true);
    inFunction = false;
    infoFunc.setFirstIteration(true);
    mm.prevMenu();     
    LCDPrint();
    }
}


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
