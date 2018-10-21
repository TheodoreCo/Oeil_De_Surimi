#include <stdarg.h> /* for va_list etc. */
#include <stdio.h>  /* for printf */
#include <math.h>   /* for e, exp, ... */
#include <memory.h> /* for memcpy */
#include <assert.h> /* for assert */

#include "neural_network.h"

static inline double sigmoid(double x)
{
    return 1.0 / (1 + exp(-x));
}

/* Scales the range of rand() to -0.5 .. +0.5*/
static double rnd(void) {
    double ret_val =  rand() / (double)RAND_MAX - 0.5;
    // printf("Random returns: %.5f\n", ret_val);
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

    const int hidden_weights = num_hid_lay ? (num_inputs+1) * num_hid_neur + (num_hid_lay-1) * (num_hid_neur+1) * num_hid_neur : 0;
    const int output_weights = (num_hid_lay ? (num_hid_neur+1) : (num_inputs+1)) * num_outputs;
    const int total_weights = (hidden_weights + output_weights);

    const int total_neurons = (num_inputs + num_hid_neur * num_hid_lay + num_outputs);

    const int size = sizeof(neur_net) + sizeof(double) * (total_weights + total_neurons + (total_neurons - num_inputs));
    neur_net *nn = malloc(size);
    if (!nn) return 0;

    nn->num_inputs = num_inputs;
    nn->num_hid_lay = num_hid_lay;
    nn->num_hid_neur = num_hid_neur;
    nn->num_outputs = num_outputs;

    nn->total_weights = total_weights;
    nn->total_neurons = total_neurons;

    /* Set pointers. */
    nn->weights = (double*)((char*)nn + sizeof(neur_net));
    nn->outputs = nn->weights + nn->total_weights;
    nn->deltas = nn->outputs + nn->total_neurons;


//    const int num_of_pure_weights =
//        num_inputs * num_hid_neur +                         // between input & hidden
//        num_hid_neur * num_hid_neur * (num_hid_lay - 1) +   // between hidden layers (if more than one hidden layer)
//        num_hid_neur * num_outputs;                         // between hidden & output
//
//    const int num_of_bias = num_hid_lay * num_hid_neur;
//    const int num_of_weights = num_of_pure_weights + num_of_bias;
//
//    const int num_hidden_and_output = num_hid_lay * num_hid_neur + num_outputs;
//    //const int num_total_neurons = num_inputs + num_hidden_and_output;
//
//    const int space_for_weights = num_of_weights * sizeof (double);
//    const int space_for_inputs = num_inputs * sizeof (double);
//    const int space_for_outputs = num_hidden_and_output * sizeof (double);
//    const int space_for_deltas = num_hidden_and_output * sizeof (double);
//
//    const int total_space =
//        sizeof (neur_net) +
//        space_for_weights +
//        space_for_inputs +
//        space_for_outputs +
//        space_for_deltas;
//
//    neur_net *nn = malloc(total_space);
//    if(!nn)
//        return 0;
//
//    nn->num_inputs = num_inputs;
//    nn->num_hid_lay = num_hid_lay;
//    nn->num_hid_neur = num_hid_neur;
//    nn->num_outputs = num_outputs;
//
//    // |---------------------------|
//    // | deltas (doubles)
//    // |---------------------------| <- begin of memory allocation for deltas
//    // | outputs (doubles)
//    // |---------------------------| <- begin of memory allocation for outputs of hidden and output layers
//    // | inputs (doubles)
//    // |---------------------------| <- begin of memory allocation for inputs
//    // | weights (doubles)
//    // |---------------------------| <- begin of memory allocation for weights
//    // | neur_net (num_inputs etc)
//    // |---------------------------| <- start of memory allocation for a neur_net structure
//
//    // The weights pointer in the memory block is just after the size of the neur_net structure.
//    nn->weights = (double *) ((char *)nn + sizeof (neur_net));
//    nn->inputs = nn->weights + num_of_weights;
//    nn->outputs = nn->inputs + num_inputs;
//    nn->deltas = nn->outputs + num_hidden_and_output;

    // Time to randomly initialize the weights !
    srand(time(NULL));
    for(int i=0; i<total_weights; i++)
    {
        nn->weights[i] = rnd();
    }

//    // Inputs, outputs & deltas are all 0.0
//    for(int i=0; i<num_hidden_and_output; i++)
//    {
//        nn->outputs[i] = 0.0;
//        nn->deltas[i] = 0.0;
//    }

    return nn;
}

void nn_to_string(neur_net *nn)
{
    printf("\n\n********* Oeil-de-Surimi's Neural network ************\n");
    printf("Layers: %d", nn->num_hid_lay + 2);
    printf(" [%d", nn->num_inputs);
    for(int i = 0; i < nn->num_hid_lay; i++)
    {
        printf(", %d", nn->num_hid_neur);
    }
    printf(", %d]", nn->num_outputs);

    printf("\nWeights: \n\t");

    for(int i=0; i<nn->total_weights; i++)
    {
        printf("%.3f ", nn->weights[i]);
    }

    printf("\nInputs: \n\t");
    for(int i=0; i<nn->num_inputs; i++)
    {
        printf("%.3f ", nn->outputs[i]);
    }

    printf("\nHidden & Outputs: \n\t");
    for(int i=nn->num_inputs; i<nn->total_neurons; i++)
    {
        printf("%.3f ", nn->outputs[i]);
    }

    printf("\nDeltas: \n\t");
    for(int i=0; i<nn->total_neurons-nn->num_inputs; i++)
    {
        printf("%.3f ", nn->deltas[i]);
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

double const *nn_run(neur_net const *nn, double const *inputs)
{
    double const *w = nn->weights;
    double *o = nn->outputs + nn->num_inputs;
    double const *i = nn->outputs;

    memcpy(nn->outputs, inputs, sizeof(double) * nn->num_inputs);

    int h, j, k;

//    if (!nn->num_hid_lay) {
//        double *ret = o;
//        for (j = 0; j < nn->num_outputs; ++j) {
//            double sum = *w++ * -1.0;
//            for (k = 0; k < nn->inputs; ++k) {
//                sum += *w++ * i[k];
//            }
//            *o++ = genann_act_output(nn, sum);
//        }
//
//        return ret;
//    }

    //printf("w-nn->weights = %ld\n", w-nn->weights);
    /* Figure input layer */
    for (j = 0; j < nn->num_hid_neur; ++j)
    {
        double sum = *w++ * -1.0;
        for (k = 0; k < nn->num_inputs; ++k)
        {
            sum += *w++ * i[k];
            //printf("w-nn->weights = %ld\n", w-nn->weights);
        }
        *o++ = sigmoid(sum);
    }

    i += nn->num_inputs;

    /* Figure hidden layers, if any. */
    for (h = 1; h < nn->num_hid_lay; ++h)
    {
        for (j = 0; j < nn->num_hid_neur; ++j)
        {
            double sum = *w++ * -1.0;
            for (k = 0; k < nn->num_hid_neur; ++k)
            {
                sum += *w++ * i[k];
            }
            *o++ = sigmoid(sum);
        }

        i += nn->num_hid_neur;
    }

    double const *ret = o;

    /* Figure output layer. */
        for (j = 0; j < nn->num_outputs; ++j)
    {
        double sum = *w++ * -1.0;
        for (k = 0; k < nn->num_hid_neur; ++k)
        {
            sum += *w++ * i[k];
        }
        *o++ = sigmoid(sum);
    }

    /* Sanity check that we used all weights and wrote all outputs. */
    assert(w - nn->weights == nn->total_weights);
    assert(o - nn->outputs == nn->total_neurons);

    return ret;
}

void nn_learn(neur_net const *nn, double const *inputs, double const *expected, double learning_rate)
{
    // First cycle, in order to have the first outputs & deltas
    nn_run(nn, inputs);

    int h, j, k;

    /* First set the output layer deltas. */
    {
        double const *o = nn->outputs + nn->num_inputs + nn->num_hid_lay * nn->num_hid_neur; /* First output. */
        double *d = nn->deltas + nn->num_hid_lay * nn->num_hid_neur; /* First delta. */
        double const *t = expected; /* First desired output. */

        for (j = 0; j < nn->num_outputs; ++j)
        {
            *d++ = *t++ - *o++;
        }
    }


    /* Set hidden layer deltas, start on last layer and work backwards. */
    /* Note that loop is skipped in the case of hidden_layers == 0. */
    for (h = nn->num_hid_lay - 1; h >= 0; --h)
    {

        /* Find first output and delta in this layer. */
        double const *o = nn->outputs + nn->num_inputs + (h * nn->num_hid_neur);
        double *d = nn->deltas + (h * nn->num_hid_neur);

        /* Find first delta in following layer (which may be hidden or output). */
        double const * const dd = nn->deltas + ((h+1) * nn->num_hid_neur);

        /* Find first weight in following layer (which may be hidden or output). */
        double const * const ww = nn->weights + ((nn->num_inputs+1) * nn->num_hid_neur) + ((nn->num_hid_neur+1) * nn->num_hid_neur * (h));

        for (j = 0; j < nn->num_hid_neur; ++j)
        {

            double delta = 0;

            for (k = 0; k < (h == nn->num_hid_lay-1 ? nn->num_outputs : nn->num_hid_neur); ++k)
            {
                const double forward_delta = dd[k];
                const int windex = k * (nn->num_hid_neur + 1) + (j + 1);
                const double forward_weight = ww[windex];
                delta += forward_delta * forward_weight;
            }

            *d = *o * (1.0-*o) * delta;
            ++d;
            ++o;
        }
    }


    /* Train the outputs. */
    {
        /* Find first output delta. */
        double const *d = nn->deltas + nn->num_hid_neur * nn->num_hid_lay; /* First output delta. */

        /* Find first weight to first output delta. */
        double *w = nn->weights + (nn->num_hid_lay
                                   ? ((nn->num_inputs+1) * nn->num_hid_neur + (nn->num_hid_neur+1) * nn->num_hid_neur * (nn->num_hid_lay-1))
                                   : (0));

        /* Find first output in previous layer. */
        double const * const i = nn->outputs + (nn->num_hid_lay
                                                ? (nn->num_inputs + (nn->num_hid_neur) * (nn->num_hid_lay-1))
                                                : 0);

        /* Set output layer weights. */
        for (j = 0; j < nn->num_outputs; ++j)
        {
            *w++ += *d * learning_rate * -1.0;
            for (k = 1; k < (nn->num_hid_lay ? nn->num_hid_neur : nn->num_inputs) + 1; ++k)
            {
                *w++ += *d * learning_rate * i[k-1];
            }

            ++d;
        }
    }


    /* Train the hidden layers. */
    for (h = nn->num_hid_lay - 1; h >= 0; --h)
    {

        /* Find first delta in this layer. */
        double const *d = nn->deltas + (h * nn->num_hid_neur);

        /* Find first input to this layer. */
        double const *i = nn->outputs + (h
                                         ? (nn->num_inputs + nn->num_hid_neur * (h-1))
                                         : 0);

        /* Find first weight to this layer. */
        double *w = nn->weights + (h
                                   ? ((nn->num_inputs+1) * nn->num_hid_neur + (nn->num_hid_neur+1) * (nn->num_hid_neur) * (h-1))
                                   : 0);


        for (j = 0; j < nn->num_hid_neur; ++j)
        {
            *w++ += *d * learning_rate * -1.0;
            for (k = 1; k < (h == 0 ? nn->num_inputs : nn->num_hid_neur) + 1; ++k)
            {
                *w++ += *d * learning_rate * i[k-1];
            }
            ++d;
        }
    }
}

void nn_xor_learn(neur_net const* nn, int num_epochs, double learning_rate)
{
    // XOR truth table:
    const double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const double expected[4] = {0, 1, 1, 0};

    // TODO: training should be done on random-choosen patterns !
    for (int i = 0; i < num_epochs; ++i) {
        nn_learn(nn, input[0], expected + 0, learning_rate);
        nn_learn(nn, input[1], expected + 1, learning_rate);
        nn_learn(nn, input[2], expected + 2, learning_rate);
        nn_learn(nn, input[3], expected + 3, learning_rate);
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
