#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <stdlib.h>

struct neur
{
    unsigned int num_weights;

    double value;
    double *weights;
    double biase;
    double d_y;
};

typedef struct neur neur;

typedef struct layer layer;
struct layer
{
    unsigned int num_neur;
    neur **neur_array;
};

typedef struct neur_net neur_net;
struct neur_net
{
    unsigned int num_arrays;
    layer **layer_array;
    layer *outputs;
};

neur_net *instantiate(size_t num_inputs, size_t num_hidden_layers,
	size_t num_hidd_neur, size_t num_outputs);

void neur_net_free(neur_net *nn);

void pretty_print(neur_net *nn);

void neur_compute(neur *n, layer *prev_layer);

void layer_compute(layer *l, layer *prev_layer);

double *feed_forward(neur_net *nn, double *inputs);

void backprop(neur_net *nn, double *inputs, double *target,
	double learning_rate);

double sigmoid(double x);

void nn_save(neur_net *nn, char *path);

void ocr_train(neur_net *nn, double learning_rate, unsigned int epochs);

<<<<<<< HEAD
void xor_train(neur_net *nn, double learning_rate, unsigned int epochs);
=======
void xor_train(neur_net *nn, double learning_rate, unsigned int epochs);
>>>>>>> master

neur_net *nn_load(char *path);

double const *nn_run(neur_net const *nn, double const *inputs);

//TODO run function

//TODO train function

#endif
