#include <stdio.h>
#include <stlib.h>
#include <math.h>
#include <time.h>

#include "neural_network.h"

neur_net *instantiate(size_t num_inputs, size_t num_hidden_layers, size_t num hidd_neur, size_t num_outputs)
{
    //TODO 
    return NULL
}

void neur_net_free(neur_net *nn)
{
    
    if (nn != NULL)
    {
        free(nn);
    }
}

void pretty_print(neur_net *nn)
{
    //TODO
    return NULL
}

void neur_compute(neur *n);
{
    //TODO
    return NULL
}

void layer_compute(layer *l);
{
    //TODO
    return NULL
}

double *feed_forward(neur_net *nn, double *inputs);
{
    //TODO
    return NULL
}

void backprop(neur_net *nn, double *inputs, double *target, double learning_rate);
{
    //TODO
    return NULL
}

double sigmoid(double x);
{
    return (1.0 / (1 + exp(-x));
}


void nn_save(neur_net *nn, char * path);
{
    //TODO
    return NULL
}

neur_net *nn_load(char *path);
{
    //TODO
    return NULL
}

