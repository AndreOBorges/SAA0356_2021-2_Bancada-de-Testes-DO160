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

//Classe Vector
template <typename T> class Vector 
{
    // arr is the integer pointer which stores the address of our vector
    T* arr;
    // capacity is the total storage capacity of the vector
    int capacity;
    // current is the number of elements currently present in the vector
    int current;
  public:
    // Default constructor to initialise an initial capacity of 1 element and allocating storage using dynamic allocation
    Vector()
    {
      arr = new T[1];
      capacity = 1;
      current = 0;
    }
    // Function to add an element at the last
    void push_back(T data)
    {
      // if the number of elements is equal to the capacity, that means we don't have space to accommodate more elements. We need to double the capacity
      if (current == capacity) {
        T* temp = new T[2 * capacity];
        
        // copying old array elements to new array
        for (int i = 0; i < capacity; i++) {
          temp[i] = arr[i];
        }
  
        // deleting previous array
        delete[] arr;
        capacity *= 2;
        arr = temp;
      }
  
      // Inserting data
      arr[current] = data;
      current++;
    }
  
    // function to extract element at any index
    T getElement(int index)
    {
      // if index is within the range
      if (index < current)
            return arr[index];
    }
  
    // function to delete last element
    void pop() { current--; }
  
    // function to get size of the vector
    int size() { return current; }
  
    // function to get capacity of the vector
    int getcapacity() { return capacity; }
  
    // function to get the first element
    T front() { return arr[0]; }
  
    // function to get the last element
    T back() { return arr[current]; }
};

//Classe Menu
class Menu {
  private: //usually variables go here
    String title; //o título que será impresso
    Vector<Menu*> subMenus; //vetor de menus filhos                         
    Menu* fatherMenu; //menu pai
//    Function* function;
    bool haveFunction; //se o menu possui função, esse valor é verdadeiro e usado pelo MM para executar função
    void (*func)();
    int funcStage; //para funções de vários stages
    int programCounter;
  public:
    Menu(const String title) 
      : title(title)
    {
      fatherMenu = this;
      haveFunction = false;
      programCounter = 0;
      funcStage = 0;
    }
    //função que retorna o títlo para impressão na LCD
    String getTitle() {
      return title;
    }
    //função que permite adicionar um sub menu (=menu filha) e vincular esse menu como pai
    void addSubMenu(Menu* childMenu) {
      subMenus.push_back(childMenu);
      childMenu->fatherMenu = this;
    }

    //função que recebe um subMenu e entrega a posição dele no array;
    int subMenuPosition(Menu* currentSubMenu) {
      for (int i = 0; i < subMenus.size(); i++){
        if (subMenus.getElement(i) == currentSubMenu){
          return i;
        }  
      }
    }
    //função que recebe um número e retorna o subMenu que se encontra naquela posição do vetor
    Menu* subMenuSelected(int i) {
      if (subMenus.getElement(i) != subMenus.back()) return subMenus.getElement(i);
      else return subMenus.back();
    }
    
    //função que retorna o próximo sub menu em comparação com um menu fornecido
    Menu* nextSubMenu(int currentSelecPos) {
    if (currentSelecPos == subMenus.size() - 1) return subMenuSelected(currentSelecPos);
    else return subMenuSelected(currentSelecPos + 1);
        }
    
    //função que retorna o sub menu anterior em comparação com um menu fornecido    
    Menu* prevSubMenu(int currentSelecPos){
    if (currentSelecPos > 0) return subMenuSelected(currentSelecPos - 1);
    else return subMenus.front();
    }
    
    //função que retorna front do vetor subMenus
    Menu* frontSubMenu() {
      return subMenus.front();
    }

    //função que retorna back do vetor subMenus
    Menu* backSubMenu() {
      return subMenus.back();
    }

    //função que retorna quantidade de sub menus do menu
    int getSubMenuQty() {
      return subMenus.size();
    }
    
    //função que retorna o pai
    Menu* getFatherMenu() {
      return fatherMenu;
    }

    bool getExistingFunct() {
      return haveFunction;
    }
    /*
    Function* getFunction() {
      return function;
    }*/
    
    void executeFunction() {
      if (haveFunction) {
        inFunction = true;
        (*func)();
      }
    }
    void addFunction(void (*function)()) {
      func = function;
      haveFunction = true;
    }

    Menu* getMenu() { return this; }

    int getProgramCounter() { return programCounter; }

    void addProgramCounter() { programCounter++; }

    void setProgramCounter(int i) { programCounter = i; }

    int getFuncStage() { return funcStage; }

    void setFuncStage(int i) { funcStage = i; }

    void addFuncStage() { funcStage++; }

    void subFuncStage() { funcStage--; }
};

//Classe que vai gerenciar o menu
class MM {
  private:
    Menu* mainMenu;
    Menu* currentMenu;
    Menu* currentSelection;
    Menu* lastSelection;
    int currentSelecVecPos; //posição no vetor de subMenus que a atual seleção se encontra;
    int lastSelecVecPos;
    int currentLine;
    int lastLine;
    String currentDisplay[4];
  public:
  MM(Menu* currentMenu)
    : currentMenu(currentMenu)
  {
    currentLine = 0;
    currentSelecVecPos = 0;
    lastLine = currentLine; 
    lastSelecVecPos = currentSelecVecPos;   
    mainMenu = currentMenu;
    currentSelection = currentMenu->nextSubMenu(currentSelecVecPos);
    lastSelection = currentSelection;
  }

  //função que coloca como seleção atual o próximo menu nas opções de submenu
  void nextSelection() {
    lastSelection = currentSelection;
    currentSelection = currentMenu->nextSubMenu(currentSelecVecPos);
    if (lastSelection == currentSelection) {
      currentLine = currentLine;
      currentSelecVecPos = currentSelecVecPos;
    }
    else if (currentLine == 3) {
      currentLine = currentLine;
      currentSelecVecPos++;
    }
    else {
      currentLine++;
      currentSelecVecPos++;
    }
  }

  //função que coloca como seleção atual o menu anterior nas opções de submenu
  void prevSelection() {
    lastSelection = currentSelection;
    currentSelection = currentMenu->prevSubMenu(currentSelecVecPos);
    if (lastSelection == currentSelection) {
      currentLine = currentLine;
      currentSelecVecPos = currentSelecVecPos;
    }
    else if (currentLine == 0) {
      currentLine = currentLine;
      currentSelecVecPos--;
    }
    else {
      currentLine--;
      currentSelecVecPos--;
    }
  }

  //função que avança o menu para o menu selecionado
  void nextMenu() {
   if (!currentSelection->getExistingFunct() && currentSelection->getSubMenuQty() > 0) {
    currentMenu = currentSelection;
    currentSelection = currentMenu->frontSubMenu();
    lastLine = currentLine;
    lastSelecVecPos = currentSelecVecPos;
    currentLine = 0;
    currentSelecVecPos = 0;
    }
    else if (currentSelection->getExistingFunct()){
      currentSelection->executeFunction();
      currentMenu = currentSelection;
      lastLine = currentLine;
      lastSelecVecPos = currentSelecVecPos;
      haveInput = false;      
    }
  }

  //função que volta para o menu pai
  void prevMenu() {
    if (currentMenu != mainMenu) {
      if (!inFunction) {
        currentMenu = currentMenu->getFatherMenu();
        currentLine = lastLine;
        currentSelecVecPos = lastSelecVecPos;
        currentSelection = currentMenu->subMenuSelected(currentSelecVecPos);
      }
      else {
        currentMenu = currentMenu->getFatherMenu();
        inFunction = false;
        currentLine = lastLine;
        currentSelecVecPos = lastSelecVecPos;
        currentSelection = currentMenu->subMenuSelected(currentSelecVecPos);
      }
    }
  }

  void clearDisplay() {
    for (int k = 0; k < 4; k++) {
      currentDisplay[k] = " ";
    }
  }
  
  //função que organiza o array de submenus que serão impressos na tela
  void displayOrganizer(bool diffContent = false, String newContent[4] = 'c') {
//    clearDisplay();
    if (!diffContent) {
      int i = currentMenu->subMenuPosition(currentSelection);
      int j = i;
      for (j; j < i + 4; j++) {
        if ( currentMenu->subMenuPosition(currentMenu->subMenuSelected(j-currentLine)) <= currentMenu->getSubMenuQty()) {
          currentDisplay[j-i] = currentMenu->subMenuSelected(j-currentLine)->getTitle();
        }
        else currentDisplay[j-i] = " ";
      }
    }
    else if (diffContent) {
      for (int i = 0; i < 4; i++) {
        currentDisplay[i] = newContent[i];
      }
    }
  }
  
  //função que retorna um elemento do currentDisplay DEPOIS JUNTAR COM A displayOrganizer
  String getCurrentDisplay(int i) {
  return currentDisplay[i];
  }
  //função que retorna o current line
  int getCurrentLine() {
    return currentLine;
  }

  Menu* getCurrentMenu() {
    return currentMenu;
  }
  
  String currentSelectionTitle() {
    return currentSelection->getTitle();
  }

};
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
