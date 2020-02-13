#include <iostream>
#include "../include/Neuron.hpp"
#include "../include/Matrix.hpp"
#include "../include/NeuralNetwork.hpp"
#include "../include/utils/MultiplyMatrix.hpp"
int main(){

  //Neuron *n1 = new Neuron(0.9);
  //cout<<"Val: "<<n1->getVal()<<endl;
  //cout<<"Activated Val:"<<n1->getActivatedVal()<<endl;
  //cout<<"Derived Val:"<<n1->getDerivedVal()<<endl;
  //Matrix *m = new Matrix(3,2,true);
  //m->printToConsole();
  //cout<<"----------------------------------------"<<endl;
  //Matrix *mT = m->transpose();
  //mT->printToConsole();
  
  vector<int> topology;
  topology.push_back(3);
  topology.push_back(2);
  topology.push_back(1);

  vector<double> input;
  input.push_back(1.0);
  input.push_back(0.0);
  input.push_back(1.0);


  NeuralNetwork *nn = new NeuralNetwork(topology);
  nn->setCurrentInput(input);
  nn->feedForward();
  nn->printToConsole();
  return 0;
}
