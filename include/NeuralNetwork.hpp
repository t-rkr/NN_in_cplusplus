#ifndef _NEURALNETWORK_HPP
#define _NEURALNETWORK_HPP_

#include <iostream>
#include "Matrix.hpp"
#include "Layer.hpp"
using namespace std;

class NeuralNetwork{
  public:
    NeuralNetwork(vector<int> topology);
    void setCurrentInput(vector<double> input);
    void printToConsole();
  private:
    vector<int> topology;
    int topologySize;
    vector<Layer *> layers;
    vector<Matrix *> weightMatrices;
    vector<double> input;
};

#endif
