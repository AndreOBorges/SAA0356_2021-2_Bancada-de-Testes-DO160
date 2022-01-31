#ifndef MENU_MANAGER_CLASS_H
#define MENU_MANAGER_CLASS_H

#include "Arduino.h"
//#include "MenuClass.h"
//#include "FuncClass.h"

class Function;
class Menu;

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
    bool haveInput; //quando algum dos botóes da positivo, esse bool vai pra positivo, permitindo que uma nova impressão de menus seja feita, já com a atualização do input
    bool left, right, up, down;
  public:
  MM(Menu* currentMenu);

  //função que recebe input do usuário
  void userInput();

  void zeroInputs();
  
  bool checkLeft();

  bool checkRight();

  bool checkUp();

  bool checkDown();

//função que entrega atual valor de haveInput
  bool getHaveInput();

  //função que define valor de haveInput
  void setHaveInput(bool haveInputStatus);

  //função que coloca como seleção atual o próximo menu nas opções de submenu
  void nextSelection();
  
  //função que coloca como seleção atual o menu anterior nas opções de submenu
  void prevSelection();

  //função que avança o menu para o menu selecionado
  void nextMenu();

  //função que volta para o menu pai
  void prevMenu();

  void clearDisplay();
  
  //função que organiza o array de submenus que serão impressos na tela
  void displayOrganizer(bool diffContent = false, String* newContent = 'c');
  
  //função que retorna um elemento do currentDisplay DEPOIS JUNTAR COM A displayOrganizer
  String getCurrentDisplay(int i);
  
  //função que retorna o current line
  int getCurrentLine();

  Menu* getCurrentMenu();
  
  String currentSelectionTitle();

};

#endif //MENU_MANAGER_CLASS_H
