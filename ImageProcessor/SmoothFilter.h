#ifndef CONCRETEDECORATORA_H
#define CONCRETEDECORATORA_H

#include <iostream>
#include "decorator.h"

using namespace std;

class ConcreteDecoratorA : public Decorator {
    public:
        ConcreteDecoratorA(Component* component_) : Decorator(component_) {};
        virtual double getPrice() {
            double temp = Decorator::getPrice();
            return temp + getConcreteDecoratorAPrice();
        }
    private:
        double getConcreteDecoratorAPrice() {
            cout << "ConcreteDecoratorA : 50.3" << endl;
            return 50.3;
        }
};

#endif // CONCRETEDECORATORA_H
