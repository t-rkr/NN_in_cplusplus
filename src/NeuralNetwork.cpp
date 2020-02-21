#include "../include/NeuralNetwork.hpp"
#include "../include/utils/MultiplyMatrix.hpp"

void NeuralNetwork::backPropogation(){


  vector<Matrix *> newWeights;
  Matrix *gradient;
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
                              lastHiddenLayer->matrixifyActivatedVals()))->execute()->transpose();
  Matrix *newWeightsOutputToHidden = new Matrix(deltaOutputHidden->getNumRows(),deltaOutputHidden->getNumCols(),false);

  for(int r = 0; r < deltaOutputHidden->getNumRows();r++){
    for(int c=0; c < deltaOutputHidden->getNumCols();c++){
      double originalWeight = weightsOutputHidden->getValue(r, c);
      double deltaWeight = deltaOutputHidden->getValue(r, c);
      newWeightsOutputToHidden->setValue(r, c, (originalWeight - deltaWeight));
    }
  }
  newWeights.push_back(newWeightsOutputToHidden);
  gradient = new Matrix(gradientsYToZ->getNumRows(), gradientsYToZ->getNumCols(),false);
  //cout<<"Output to Hidden New Weights"<<endl;
  //newWeightsOutputToHidden->printToConsole();
  
  for(int r = 0; r < gradientsYToZ->getNumRows(); r++){    
    for(int c = 0; c < gradientsYToZ->getNumCols(); c++){
      gradient->setValue(r, c, gradientsYToZ->getValue(r,c));
    }
  }
  //Moving from last hidden layer down to input layer
  for(int i=(outputLayerIndex -1);i>0;i--){
    Layer *l = this->layers.at(i);
    Matrix *activatedHidden = l->matrixifyDerivedVals();
    Matrix *derivedHidden = l->matrixifyDerivedVals();
    Matrix *derivedGradients = new Matrix(1, l->getNeurons().size(),false );
   
    Matrix *weightMatrix = this->weightMatrices.at(i);
    Matrix *orignalWeight = this->weightMatrices.at(i-1);

    for(int r = 0; r < weightMatrix->getNumRows(); r++){
      double sum=0;
      for (int c = 0; c < weightMatrix->getNumCols();c++){
           double p = gradient->getValue(0, c) * weightMatrix->getValue(r,c);
           sum += p;
        }
      double g = sum*activatedHidden->getValue(0, r);
      derivedGradients->setValue(0, r, g);
      }
    Matrix *leftNeurons = (i-1)==0 ? this->layers.at(0)->matrixifyVals() : this->layers.at(i-1)->matrixifyActivatedVals();

    Matrix *deltaWeights = (new utils::MultiplyMatrix(derivedGradients->transpose(), leftNeurons))->execute()->transpose();

    Matrix *newWeightsHidden = new Matrix(deltaWeights->getNumRows(), deltaWeights->getNumCols(), false);
    
    for(int r =0 ; r < newWeightsHidden->getNumRows();r++){
      for (int c=0; c < newWeightsHidden->getNumCols();c++){
        double w = orignalWeight->getValue(r, c);
        double d = deltaWeights->getValue(r,c);
        double n = w - d;
        newWeightsHidden->setValue(r, c, n);
      }
    }
    gradient = new Matrix(derivedGradients->getNumRows(), derivedGradients->getNumCols(), false);
    for(int r = 0; r < derivedGradients->getNumRows(); r++){    
      for(int c = 0; c < derivedGradients->getNumCols(); c++){
        gradient->setValue(r, c, derivedGradients->getValue(r,c));
    }
  }
    newWeights.push_back(newWeightsHidden);
  }
  reverse(newWeights.begin(), newWeights.end());
  this->weightMatrices = newWeights;
}

void NeuralNetwork::printTargetToConsole(){
  for(int i =0; i<this->target.size();i++){
    cout<<this->target.at(i)<<"\t";
  }
  cout<<endl;
}

void NeuralNetwork::printInputToConsole(){
  for(int i = 0; i<this->input.size();i++){
    cout<<this->input.at(i)<<"\t";
  }
  cout<<endl;
}

void NeuralNetwork::printOutputToConsole(){
  int indexOfOutputLayer = this->layers.size() -1;
  Matrix *outputValues = this->layers.at(indexOfOutputLayer)->matrixifyActivatedVals();
  for(int c = 0; c<outputValues->getNumCols();c++){
    cout<<outputValues->getValue(0,c)<<"\t";
  } 
  cout<<endl;
}

void NeuralNetwork::setErrors(){
  errors.clear();
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
  vector<Neuron *> outputNeurons = this->layers.at(outputLayerIndex)->getNeurons();
  for(int i=0; i<target.size();i++)
  {
    //double tempErr = (outputNeurons.at(i)->getActivatedVal() - target.at(i));
    double tempErr = (outputNeurons.at(i)->getActivatedVal() - target.at(i));
    errors.push_back(tempErr);
    this->error +=pow(tempErr,2);
  }
  this->error = 0.5 * this->error;
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

