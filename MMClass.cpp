#include "MMClass.h"

extern bool haveInput;
extern bool inFunction;
extern bool haveFunction;

//Classe que vai gerenciar o menu

  MM::MM(Menu* currentMenu)
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
  void MM::nextSelection() {
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
  void MM::prevSelection() {
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
  void MM::nextMenu() {
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
  void MM::prevMenu() {
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

  void MM::clearDisplay() {
    for (int k = 0; k < 4; k++) {
      currentDisplay[k] = " ";
    }
  }
  
  //função que organiza o array de submenus que serão impressos na tela
  void MM::displayOrganizer(bool diffContent = false, String newContent[4] = 'c') {
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
  String MM::getCurrentDisplay(int i) {
  return currentDisplay[i];
  }
  
  //função que retorna o current line
  int MM::getCurrentLine() {
    return currentLine;
  }

  Menu* MM::getCurrentMenu() {
    return currentMenu;
  }
  
  String MM::currentSelectionTitle() {
    return currentSelection->getTitle();
  }
