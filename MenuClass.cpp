#include "MenuClass.h"

extern bool inFunction;
extern bool haveFunction;

    Menu::Menu(const String title) 
      : title(title)
    {
      fatherMenu = this;
      haveFunction = false;
      programCounter = 0;
      funcStage = 0;
    }
    //função que retorna o títlo para impressão na LCD
    String Menu::getTitle() {
      return title;
    }
    //função que permite adicionar um sub menu (=menu filha) e vincular esse menu como pai
    void Menu::addSubMenu(Menu* childMenu) {
      subMenus.push_back(childMenu);
      childMenu->fatherMenu = this;
    }

    //função que recebe um subMenu e entrega a posição dele no array;
    int Menu::subMenuPosition(Menu* currentSubMenu) {
      for (int i = 0; i < subMenus.size(); i++){
        if (subMenus.getElement(i) == currentSubMenu){
          return i;
        }  
      }
    }
    //função que recebe um número e retorna o subMenu que se encontra naquela posição do vetor
    Menu* Menu::subMenuSelected(int i) {
      if (subMenus.getElement(i) != subMenus.back()) return subMenus.getElement(i);
      else return subMenus.back();
    }
    
    //função que retorna o próximo sub menu em comparação com um menu fornecido
    Menu* Menu::nextSubMenu(int currentSelecPos) {
    if (currentSelecPos == subMenus.size() - 1) return subMenuSelected(currentSelecPos);
    else return subMenuSelected(currentSelecPos + 1);
        }
    
    //função que retorna o sub menu anterior em comparação com um menu fornecido    
    Menu* Menu::prevSubMenu(int currentSelecPos){
    if (currentSelecPos > 0) return subMenuSelected(currentSelecPos - 1);
    else return subMenus.front();
    }
    
    //função que retorna front do vetor subMenus
    Menu* Menu::frontSubMenu() {
      return subMenus.front();
    }

    //função que retorna back do vetor subMenus
    Menu* Menu::backSubMenu() {
      return subMenus.back();
    }

    //função que retorna quantidade de sub menus do menu
    int Menu::getSubMenuQty() {
      return subMenus.size();
    }
    
    //função que retorna o pai
    Menu* Menu::getFatherMenu() {
      return fatherMenu;
    }

    bool Menu::getExistingFunct() {
      return haveFunction;
    }
    /*
    Function* getFunction() {
      return function;
    }*/
    
    void Menu::executeFunction() {
      if (haveFunction) {
        inFunction = true;
        (*func)();
      }
    }
    void Menu::addFunction(void (*function)()) {
      func = function;
      haveFunction = true;
    }

    Menu* Menu::getMenu() { return this; }

    int Menu::getProgramCounter() { return programCounter; }

    void Menu::addProgramCounter() { programCounter++; }

    void Menu::setProgramCounter(int i) { programCounter = i; }

    int Menu::getFuncStage() { return funcStage; }

    void Menu::setFuncStage(int i) { funcStage = i; }

    void Menu::addFuncStage() { funcStage++; }

    void Menu::subFuncStage() { funcStage--; }
