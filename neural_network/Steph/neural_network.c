#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neural_network.h"

neur_net *instantiate(size_t num_inputs, size_t num_hidden_layers, size_t num_hidd_neur, size_t num_outputs)
{
    unsigned int layer_num = num_hidden_layers + 2; //Nombre total de layers
	neur_net *nn = malloc(sizeof(neur_net));    //malloc du reseau nn
    nn->layer_array = malloc(sizeof(layer*) * layer_num);   //malloc de l'array de layer de nn 
    nn->num_arrays = layer_num;
    

    for(int i = 0; i < (int) layer_num; i++)           //creation des layers
    {
        layer *lay = malloc(sizeof(layer));
        if (i == 0)
        {
            printf("Init input layer \n");
            lay->num_neur = num_inputs;
            lay->neur_array = malloc(sizeof(neur) * num_inputs);
            for(unsigned int j = 0; j < num_inputs; j++)     //creation du layer d'input
            {
                printf("init neurone %u \n",j);
                neur *act = malloc(sizeof(neur));
                act->value = 0;
                act->biase = 0;
                act->num_weights = 0;
                lay->neur_array[j] = act;
            }
        }
        else if(i == (int) layer_num -1)
        {
            printf("Init output layer \n");
            lay->num_neur = num_outputs;
            lay->neur_array = malloc(sizeof(neur) * num_outputs);
            for(unsigned int k = 0; k < num_outputs; k++)      // creation du layer d'output
            {
                printf("init neurone %u \n",k);
                neur *act = malloc(sizeof(neur));
                act->value = 0;
                act->biase = (double) rand() / RAND_MAX * 2.0 - 1.0;
                act->num_weights = nn->layer_array[i-1]->num_neur;
                act->weights = malloc(sizeof(neur) * act->num_weights);
                for(unsigned int l = 0; l < act->num_weights; l++)
                {
                    act->weights[l] = (double) rand() / RAND_MAX * 2.0 - 1.0;
                    printf("weight n°%u \n", l);
                }
                lay->neur_array[k] = act;
            }
            nn->outputs = lay;
        }
        else
        {
            printf("Init hidden layer %u \n", i);
            lay->num_neur = num_hidd_neur;
            lay->neur_array = malloc(sizeof(neur) * num_hidd_neur);
            for(unsigned int m = 0; m < num_hidd_neur; m++)      // creation d'un hidden layer
            {
                printf("init neurone %u \n",m);
                neur *act = malloc(sizeof(neur));
                act->value = 0;
                act->biase = (double) rand() / RAND_MAX * 2.0 - 1.0;
                act->num_weights = nn->layer_array[i-1]->num_neur;
                act->weights = malloc(sizeof(neur) * act->num_weights);
                for(unsigned int l = 0; l < act->num_weights; l++)
                {
                    act->weights[l] = (double) rand() / RAND_MAX * 2.0 - 1.0;
                    printf("weight n°%u \n", l);
                }
                lay->neur_array[m] = act;
            }
        }
        nn->layer_array[i] = lay;
    }
    return nn;
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
    printf("*****************************************************\n");
    printf("********** Oeil de Surimi's Neural Network **********\n");
    printf("*****************************************************\n");
    printf("\n \n");
    for(unsigned int i = 0; i < nn->num_arrays; i++)
    {
        printf("layer : %u \n",i);
        for(unsigned int j = 0; j < nn->layer_array[i]->num_neur ; j++)
        {
            printf("\t neur:%u  key:%lf \n",j,nn->layer_array[i]->neur_array[j]->value);
            if(i != 0)
            {
                for(unsigned int k = 0; k < nn->layer_array[i]->neur_array[j]->num_weights; k++)
                {
                    printf("\t\t weigth%u : %lf \n", k, nn->layer_array[i]->neur_array[j]->weights[k]);
                }
            }
        }
    }
    printf("oui");
}

/*

void neur_compute(neur *n)
{
    //TODO
}

void layer_compute(layer *l)
{
    //TODO
}

double *feed_forward(neur_net *nn, double *inputs)
{
    //TODO
    return NULL;
}

void backprop(neur_net *nn, double *inputs, double *target, double learning_rate)
{
    //TODO
}

double sigmoid(double x)
{
    return (1.0 / (1 + exp(-x)));
}


void nn_save(neur_net *nn, char *path)
{
    //TODO
}

neur_net *nn_load(char *path)
{
    //TODO
    return NULL;
}

*/
