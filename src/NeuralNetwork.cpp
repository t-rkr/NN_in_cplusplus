#include "../include/NeuralNetwork.hpp"
#include "../include/utils/MultiplyMatrix.hpp"

void NeuralNetwork::backPropogation(){
  //output to hidden layer
  int outputLayerIndex = this->layers.size()-1;
  Matrix *derivedValuesYToZ = this->layers.at(outputLayerIndex)->matrixifyDerivedVals();
  Matrix *gradientsYToZ = new Matrix(1,this->layers.at(outputLayerIndex)->getNeurons().size(),false);

  for(int i=0;i < this->errors.size();i++)
  {
    double d = derivedValuesYToZ->getValue(0,i);
    double e = this->errors.at(i);
    double g = d*e;
    gradientsYToZ->setValue(0,i,g);
  }
  
  int lastHiddenLayerIndex = outputLayerIndex -1;
  Layer *lastHiddenLayer = this->layers.at(lastHiddenLayerIndex);
  Matrix *weightsOutputHidden = this->weightMatrices.at(outputLayerIndex -1);
  Matrix *deltaOutputHidden = (new utils::MultiplyMatrix(
                              gradientsYToZ->transpose(),
                              lastHiddenLayer->matrixifyActivatedVals()))->execute();

  for(int i=(outputLayerIndex -1);i>=0;i--){
    
  }

}

void NeuralNetwork::setErrors(){
  if(this->target.size() == 0){
    cerr << "No target for this neural network"<<endl;
    assert(false);
  }

  if(this->target.size() != this->layers.at(this->layers.size() -1)->getNeurons().size()){
    cerr<<"Target size is not the same as output layer size: "<<this->layers.at(this->layers.size() -1)->getNeurons().size()<<endl;
    assert(false);
  }

  this->error = 0.00;
  int outputLayerIndex = this->layers.size() -1;
  cout<<"Output Layer Index"<<outputLayerIndex<<endl;
  vector<Neuron *> outputNeurons = this->layers.at(outputLayerIndex)->getNeurons();
  for(int i=0; i<target.size();i++)
  {
    double tempErr = (outputNeurons.at(i)->getActivatedVal() - target.at(i));
    errors.push_back(tempErr);
    this->error +=tempErr;
  }
  historicalErrors.push_back(this->error);
}

void NeuralNetwork::feedForward(){
  for(int i=0; i<(this->layers.size()-1);i++){
        Matrix *a = this->getNeuronMatrix(i);

        if(i!=0){
          a = this->getActivatedNeuronMatrix(i);
        }
        Matrix *b = this->getWeightMatrix(i);
        Matrix *c = (new utils::MultiplyMatrix(a,b))->execute();

        for(int c_index=0; c_index < c->getNumCols();c_index++){
          this->setNeuronValue(i+1, c_index, c->getValue(0,c_index));
        }
    }
}

void NeuralNetwork::printToConsole(){
  for(int i=0;i< this->layers.size();i++){
    cout<<"Layer: "<<i<<endl;
    if(i==0){
      Matrix *m = this->layers.at(i)->matrixifyVals();
      m->printToConsole();
    }
    else{
      Matrix *m = this->layers.at(i)->matrixifyActivatedVals();
      m->printToConsole();
    }
    cout<<"============================================================"<<endl;
    if(i < this->layers.size()-1){
      cout<<"Weight matrix: "<<i<<endl;
      this->getWeightMatrix(i)->printToConsole();
    }
    cout<<"============================================================"<<endl;
  }
}

void NeuralNetwork::setCurrentInput(vector<double> input){
  this->input = input;
  for(int i=0; i<input.size();i++){
    this->layers.at(0)->setVal(i, input.at(i));
  }
}

NeuralNetwork::NeuralNetwork(vector<int> topology){
  this->topology = topology;
  this->topologySize = topology.size();
  for(int i=0;i<topologySize;i++){
    Layer *l = new Layer(topology.at(i));
    this->layers.push_back(l);
  }
  for(int i=0;i<(topologySize-1);i++){
    Matrix *m = new Matrix(topology.at(i), topology.at(i+1),true);
    this->weightMatrices.push_back(m);
  }
}

