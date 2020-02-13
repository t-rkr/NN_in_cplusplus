#ifndef _LAYER_HPP_
#define _LAYER_HPP

#include<iostream>
#include "Neuron.hpp"
#include "Matrix.hpp"

class Layer{
  public:
    Layer(int size);
    void setVal(int i, double v);
    Matrix *matrixifyVals();
    Matrix *matrixifyActivatedVals();
    Matrix *matrixifyDerivedVals();

    vector<Neuron *> getNeurons(){ return this->neurons;}
    void setNeurons(vector<Neuron *> neurons){ this->neurons = neurons;}
  private:
    int size;
    vector<Neuron *> neurons;
};

#endif
