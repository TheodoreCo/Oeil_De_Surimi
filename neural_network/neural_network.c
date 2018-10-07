#include <stdarg.h> /* for va_list etc. */
#include <stdlib.h> /* for malloc */
#include <stdio.h>  /* for printf */
#include <math.h>

#include "neural_network.h"

static inline double sigmoid(double x)
{
    return 1.0 / (1 + exp(-x));
}

/* Scales the range of rand() to -0.5 .. +0.5*/
static double rnd(void) {
    double ret_val =  rand() / (double)RAND_MAX - 0.5;
    printf("Random returns: %.5f\n", ret_val);
    return ret_val;
}


neur_net *nn_new_instance(int num_inputs, int num_hid_lay, int num_hid_neur, int num_outputs)
{
    // Minimalist checks
    if (
        num_inputs < 1  ||                          // No inputs...
        num_outputs < 1 ||                          // ... or no outputs...
        num_hid_lay < 1 ||                          // ... or no hidden layers...
        (num_hid_lay >= 1 && num_hid_neur < 1))     // ... or no hidden neurons
    {
        return 0;
    }

    const int num_of_pure_weights =
        num_inputs * num_hid_neur +                         // between input & hidden
        num_hid_neur * num_hid_neur * (num_hid_lay - 1) +   // between hidden layers (if more than one hidden layer)
        num_hid_neur * num_outputs;                         // between hidden & output

    const int num_of_bias = num_hid_lay * num_hid_neur;
    const int num_of_weights = num_of_pure_weights + num_of_bias;

    const int num_hidden_and_output = num_hid_lay * num_hid_neur + num_outputs;
    //const int num_total_neurons = num_inputs + num_hidden_and_output;

    const int space_for_weights = num_of_weights * sizeof (double);
    const int space_for_inputs = num_inputs * sizeof (double);
    const int space_for_outputs = num_hidden_and_output * sizeof (double);
    const int space_for_deltas = num_hidden_and_output * sizeof (double);

    const int total_space =
        sizeof (neur_net) +
        space_for_weights +
        space_for_inputs +
        space_for_outputs +
        space_for_deltas;

    neur_net *nn = malloc(total_space);
    if(!nn)
        return 0;

    nn->num_inputs = num_inputs;
    nn->num_hid_lay = num_hid_lay;
    nn->num_hid_neur = num_hid_neur;
    nn->num_outputs = num_outputs;

    // |---------------------------|
    // | deltas (doubles)
    // |---------------------------| <- begin of memory allocation for deltas
    // | outputs (doubles)
    // |---------------------------| <- begin of memory allocation for outputs of hidden and output layers
    // | inputs (doubles)
    // |---------------------------| <- begin of memory allocation for inputs
    // | weights (doubles)
    // |---------------------------| <- begin of memory allocation for weights
    // | neur_net (num_inputs etc)
    // |---------------------------| <- start of memory allocation for a neur_net structure

    // The weights pointer in the memory block is just after the size of the neur_net structure.
    nn->weights = (double *) ((char *)nn + sizeof (neur_net));
    nn->inputs = nn->weights + num_of_weights;
    nn->outputs = nn->inputs + num_inputs;
    nn->deltas = nn->outputs + num_hidden_and_output;

    // Time to randomly initialize the weights !
    for(int i=0; i<num_of_weights; i++) {
        nn->weights[i] = rnd();
    }

    return nn;
}

void nn_to_string(neur_net *nn) {
    printf("\n\n********* Oeil-de-Surimi's Neural network ************\n");
    printf("Layers: %d", nn->num_hid_lay + 2);
    printf(" [%d", nn->num_inputs);
    for(int i = 0; i < nn->num_hid_lay; i++) {
        printf(", %d", nn->num_hid_neur);
    }
    printf(", %d]", nn->num_outputs);

    printf("\nWeights: \n\t");

    // TODO: duplicated code -> extract function !
    const int num_of_pure_weights =
        nn->num_inputs * nn->num_hid_neur +                             // between input & hidden
        nn->num_hid_neur * nn->num_hid_neur * (nn->num_hid_lay - 1) +   // between hidden layers (if more than one hidden layer)
        nn->num_hid_neur * nn->num_outputs;                             // between hidden & output

    const int num_of_bias = nn->num_hid_lay * nn->num_hid_neur;
    const int num_of_weights = num_of_pure_weights + num_of_bias;
    for(int i=0; i<num_of_weights; i++) {
        printf("%.3f ", nn->weights[i]);
    }

    printf("\n********* Oeil-de-Surimi's Neural network ************\n\n");
}

void nn_free(neur_net *nn)
{
    if(nn != NULL)
    {
        free(nn);
    }
}


//
//    Initializes the structure from a variable number of arguments.
//    For example, nn_new_instance(3, 2, 10, 4) should return a pointer to
//    a neural network with 2 inputs, 1 hidden layer with 10 neurons and 4 outputs.
//*/
//struct neur_net *nn_new_instance(int num_layers, ...)
//{
//    // Minimal checks
//    if(num_layers < 3) return 0;    // There must be at least 3 layers
//
//    // va_list is a type to hold information about variable arguments
//    va_list ap;
//
//    // va_start must be called before accessing variable argument list
//    va_start(ap, num_layers);
//
//    const int size = sizeof(neur_net);
//    neur_net *ret = malloc(size);
//    if (!ret) return 0;
//
//    ret->layers = num_layers;
//    ret->neurons = malloc(num_layers * sizeof(int));
//
//    if(!ret->neurons) {
//        free(ret);
//        return 0;
//    }
//
//    // Now arguments can be accessed one by one using va_arg macro.
//    // TODO: call malloc to allocate space for biases and weights
//    // TODO: randomly initialize biases and weights
//    for (int i = 0; i < num_layers; i++)
//    {
//        // printf(" %d", va_arg(ap, int)); /* TEST CORRECT ARG HANDLING */
//        ret->neurons[i] = va_arg(ap, int);
//
//
//    }
//
//    return ret;
//}
