#ifndef FUNCTION_CLASS_H
#define FUNCTION_CLASS_H

class MM;

#include "Arduino.h"
//#include "MenuClass.h"
//#include "MMClass.h"


class Function {
  private:
    int funcStage; //para funções de vários stages
    bool firstIteration; //bool que começa true e fica false a partir da primeira iteração
    bool isTest; //talvez seja necessária essa bool, pra indicar que trata-se de um teste
    String testNumber;
    String testTitle;
    String totalTime;
    MM* menuManager;
    void (*function)();
//    static String specificContent[4]; //quando entra em uma função, o testo printado não são os menus, e por isso é necessário que a função preencha o que deve ser printado na tela
  public:
    Function(void (*function)(), String testNumber, String testTitle, String totalTime, MM* menuManager);
    
    Function(void (*function)(), String testTitle, MM* menuManager);
    
    bool Function::getFirstIteration();
    
    void Function::setFirstIteration(bool setParameter);
    
    void funcManager();
    
    void stageZero();
    
    void stageOne();

    void stageTwo();

//    static String* getSpecificContent();
};
    void testOneFunc();

#endif //FUNCTION_CLASS_H
