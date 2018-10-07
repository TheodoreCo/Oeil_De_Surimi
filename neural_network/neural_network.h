#ifndef NEURAL_NETWORK_H_INCLUDED
#define NEURAL_NETWORK_H_INCLUDED

#include <stdio.h>

/*
    Convention: to emulate an OO language, all functions concerning the neur_net structure will:
    - begin with 'nn_'
    - have a pointer to a neur_net as the first parameter
    Function signatures in GTK also follow a similar model.
*/

// See, e.g., https://github.com/mnielsen/neural-networks-and-deep-learning/blob/master/src/network.py
//  for a different (and more general) approach. The present approach is somewhat limited because
//  it assumes that ALL hiden layers have an equal number of neurons.
typedef struct neur_net {
    int num_inputs;     /* Number of inputs */
    int num_hid_lay;    /* Number of hidden layers */
    // TODO: this structure does not allow to define e.g. 2 hidden layers with a different number of neurons in each hidden layer.
    int num_hid_neur;   /* Number of neurons per hidden layer */
    int num_outputs;    /* Number of outputs */

    double *weights;    /* All weights, including biases.*/
    double *inputs;     /* Array if inputs for every neuron in the input layer, up to down */
    double *outputs;    /* Array of outputs for every neuron in the hidden or output layer, left to right and up to down */
    double *deltas;     /* Array of differences between outputs and expectations (during learning cycles) */
} neur_net;

// Instantiate neural network
neur_net *nn_new_instance(int num_inputs, int num_hid_lay, int num_hid_neur, int num_outputs);

// Pretty print of our neural network
void nn_to_string(neur_net *nn);

// Distroy the neural network
void nn_free(neur_net *nn);

// A sigmoid function.
// TODO: more sigmoid functions to choose among !
//double sigmoid(double x);

// Given a neural network and a set of inputs, the run function returns
double const *nn_run(neur_net const *nn, double const *inputs);

// Instantiate a neur_net structure from a file created with nn_save_to_file()
neur_net *nn_restore_from_file(FILE *file);

// Saves a neur_net to a file
void nn_save_to_file(neur_net *nn, FILE *file);

// One-cycle learning function.
// Calling nn_learn() ==> biases and weights are expected to change
//  to diminish the delta between outputs and expected.
// Of course, the first thing to do is to compute the outputs by calling nn_run.
void nn_learn(neur_net const *nn, double const *inputs, double const *expected, double learning_rate);


// Structure inspired from https://github.com/mnielsen/neural-networks-and-deep-learning/blob/master/src/network.py
//typedef struct neur_net {
//    int layers;         // number of layers, including the inputs and outputs
//    int *neurons;       // array of neurons of size layers
//    double **biases;    // biases for all neurons except input ones
//    double *weights;    // weights between neurons in layer x and neurons in layers x+1. Len =
//    double *deltas;     // deltas for each neuron (except input ones) after a learning cycle. Len = total_neurons - inputs
//} neur_net;
#endif // NEURAL_NETWORK_H_INCLUDED
