#include "MenuClass.h"
#include "MMClass.h"
#include "VectorClass.h"
#include "FuncClass.h"
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SS 53
#define SPIMOSI 51
#define SPIMISO 50
#define SPISCK 52
#define emergencyButton 2


LiquidCrystal_I2C lcd(0x27, 20, 4);
bool inFunction = false; //variável que vai para true assim que uma função começa a ser executada. É responsabilidade da função receber o input "left" e colocar o inFunction em false
String specificContent[4]; //quando entra em uma função, o testo printado não são os menus, e por isso é necessário que a função preencha o que deve ser printado na tela
float output;
bool emerButtonState = false;
Vector<float> voltageReading;
Vector<unsigned long> timeReading;
String lastTest;

void LCDPrint();
void infoDisplay();
void retrieveDataFunction();

Menu mainMenu("Main menu");
MM mm(&mainMenu); 
Function testOne(testOneFunc, "16.6.1.1", "Voltage (avr value)", "91", &mm);
Function infoFunc(infoDisplay, "Info display", &mm);
Function retrieveData(retrieveDataFunction, "Retrieve Data", &mm);
Menu tests("Tests"), test1("16.6.1.1", &testOne), test2("16.6.1.2"), test3("16.6.1.3"), test4("16.6.1.4"), test5("16.6.1.5"), test6("16.6.2.1"), test7("16.6.2.2"), test8("16.6.2.3"), test9("16.6.2.4"), retData("Retrieve Data", &retrieveData),  info("Info", &infoFunc);

File myFile;

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
  mm.refresh();
  
  lcd.init();// initialize the lcd 
  lcd.backlight();
  pinMode(emergencyButton, INPUT_PULLUP);             //pino de interrupção
  pinMode(PWMOutput, OUTPUT);                   //pino de saída de tensão
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  LCDPrint();
  attachInterrupt(digitalPinToInterrupt(emergencyButton), emerButton, CHANGE);
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
  analogWrite(PWMOutput, output);
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


void retrieveDataFunction() {
  bool inRetrieve = false;
  bool retrieveSuccess = false;
  
  if (retrieveData.getFirstIteration()) {
    if(voltageReading.size() == 0){
      specificContent[0] = " No data available";
      specificContent[1] = "";
      specificContent[2] = " Execute a test for";
      specificContent[3] = "  new, fresh data";
    }
    else {
      specificContent[0] = "   Data avaliable";
      specificContent[1] = "Last test: " + lastTest;
      specificContent[2] = "";
      specificContent[3] = "<-Back    Retrieve->";     
    }
    retrieveData.setFirstIteration(false);
    LCDPrint();
  }
  mm.userInput();

  if (mm.checkLeft() == 1) {
    mm.setHaveInput(true);
    inFunction = false;
    retrieveData.setFirstIteration(true);
    mm.prevMenu();     
    LCDPrint();
    }
    
  else if (mm.checkRight() == 1 && voltageReading.size() != 0) {
    mm.setHaveInput(true);
    if(!SD.begin(SS)){
      specificContent[0] = "SD connection failed";
      specificContent[1] = "Check if SD card is";
      specificContent[2] = "in position and try";
      specificContent[3] = "        again";     
      LCDPrint(); 
    }
    else inRetrieve = true;
  }

  while(inRetrieve) {
    int percentage;
    float oneForthSize = voltageReading.size()/4;

    if(retrieveData.getFirstIteration()) {
      specificContent[0] = "Data being retrieved";
      specificContent[1] = "    Please wait";
      specificContent[2] = "";
      specificContent[3] = "   Progress: " + String(percentage) + "%";
    }
    
    for(int i = 0; i < voltageReading.size(); i++){
      int j;
      
      j++;
      
      if (j > oneForthSize) {
        j = 0;
        percentage += 25;
        specificContent[3] = "   Progress: " + String(percentage) + "%";     
        LCDPrint();
      }
    }
    retrieveSuccess = true;
  }
  if (retrieveSuccess) {
      specificContent[0] = "   Data retrieved";
      specificContent[1] = "    successfully";
      specificContent[2] = "";
      specificContent[3] = "Back to main menu...";
      delay(1000);
      mm.setHaveInput(true);
      inFunction = false;
      retrieveData.setFirstIteration(true);
      mm.prevMenu();     
      LCDPrint();
  }
}
