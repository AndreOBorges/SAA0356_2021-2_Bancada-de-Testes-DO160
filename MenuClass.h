#ifndef CLASS_MENU_H
#define CLASS_MENU_H

#include "Arduino.h"
#include "VectorClass.h"
//#include "FuncClass.h"
//#include "MMClass.h"

class Function;

//Classe Menu
class Menu {
  private: //usually variables go here
    String title; //o título que será impresso
    Vector<Menu*> subMenus; //vetor de menus filhos                         
    Menu* fatherMenu; //menu pai
    bool haveFunction; //se o menu possui função, esse valor é verdadeiro e usado pelo MM para executar função
    Function* function;
  public:
    Menu(const String title);

    Menu(const String title, Function* function);
    
    //função que retorna o títlo para impressão na LCD
    String getTitle();

    //função que permite adicionar um sub menu (=menu filha) e vincular esse menu como pai
    void addSubMenu(Menu* childMenu);

    //função que recebe um subMenu e entrega a posição dele no array;
    int subMenuPosition(Menu* currentSubMenu);

    //função que recebe um número e retorna o subMenu que se encontra naquela posição do vetor
    Menu* subMenuSelected(int i);
    
    //função que retorna o próximo sub menu em comparação com um menu fornecido
    Menu* nextSubMenu(int currentSelecPos);
    
    //função que retorna o sub menu anterior em comparação com um menu fornecido    
    Menu* prevSubMenu(int currentSelecPos);
    
    //função que retorna front do vetor subMenus
    Menu* frontSubMenu();

    //função que retorna back do vetor subMenus
    Menu* backSubMenu();

    //função que retorna quantidade de sub menus do menu
    int getSubMenuQty();
    
    //função que retorna o pai
    Menu* getFatherMenu();

    Menu* getMenu();

    bool getExistingFunct();

    Function* getFunction();
};

#endif //CLASS_MENU_H
