#ifndef _NEURALNETWORK_HPP
#define _NEURALNETWORK_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include "utils/MultiplyMatrix.hpp"
#include "Matrix.hpp"
#include "Layer.hpp"
using namespace std;

class NeuralNetwork{
  public:
    NeuralNetwork(vector<int> topology);
    void setCurrentInput(vector<double> input);
    void setCurrentTarget(vector<double> target){this->target = target;};
    void printToConsole();
    void feedForward();
    void backPropogation();
    
    void setErrors();

    Matrix *getNeuronMatrix(int index){return this->layers.at(index)->matrixifyVals();};
    Matrix *getActivatedNeuronMatrix(int index){return this->layers.at(index)->matrixifyActivatedVals();};
    Matrix *getDerivedNeuronMatrix(int index){return this->layers.at(index)->matrixifyDerivedVals();};
    Matrix *getWeightMatrix(int index){return this->weightMatrices.at(index);};
    
    void setNeuronValue(int indexLayer, int indexNeuron, double val){ this->layers.at(indexLayer)->setVal(indexNeuron, val);};
    double getTotalError(){ return this->error; };
    vector<double> getErrors() {return this->errors;};


  private:
    vector<int> topology;
    int topologySize;
    vector<Layer *> layers;
    vector<Matrix *> weightMatrices;
    vector<Matrix *> gradientMatrices;
    vector<double> input;
    vector<double> target;
    double error;
    vector<double> errors;
    vector<double> historicalErrors;
};

#endif
