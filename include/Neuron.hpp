#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include <iostream>

using namespace std;

class Neuron
{
  public:
    Neuron(double val);

    void setVal(double val);
    //activation function is fast sigmoid
    //f(x) = x/(1+|x|)
    void activate();
    //derivation of fast sigmoid is easy
    //f'(x) = f(x)*(1-f(x))
    void derive();

    double getVal(){return this->val;}
    double getActivatedVal(){return this->activatedVal;}
    double getDerivedVal(){return this->derivedVal;}

  private:
    double val;
    double activatedVal;
    double derivedVal;
};

#endif
