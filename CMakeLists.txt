cmake_minimum_required(VERSION 3.10)

#Project name
project(NN)

add_executable(
  nn_c
  src/main.cpp
  src/Neuron.cpp
  src/Matrix.cpp
  src/Layer.cpp
  src/NeuralNetwork.cpp
  src/utils/MatrixToVector.cpp
  src/utils/MultiplyMatrix.cpp
  )
