#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neural_network.h"

neur_net *instantiate(size_t num_inputs, size_t num_hidden_layers,
        size_t num_hidd_neur, size_t num_outputs)
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
            lay->num_neur = num_inputs;
            lay->neur_array = malloc(sizeof(neur) * num_inputs);
            for(unsigned int j = 0; j < num_inputs; j++)     //creation du
            {                                                //layer d'input
                neur *act = malloc(sizeof(neur));
                act->value = 0;
                act->biase = 0;
                act->num_weights = 0;
                lay->neur_array[j] = act;
            }
        }
        else if(i == (int) layer_num -1)
        {
            lay->num_neur = num_outputs;
            lay->neur_array = malloc(sizeof(neur) * num_outputs);
            for(unsigned int k = 0; k < num_outputs; k++)      // creation du
            {                                                  //layer d'output
                neur *act = malloc(sizeof(neur));
                act->value = 0;
                act->biase = (double) rand() / RAND_MAX * 2.0 - 1.0;
                act->num_weights = nn->layer_array[i-1]->num_neur;
                act->weights = malloc(sizeof(neur) * act->num_weights);
                for(unsigned int l = 0; l < act->num_weights; l++)
                {
                    act->weights[l] = (double) rand() / RAND_MAX * 2.0 - 1.0;
                }
                lay->neur_array[k] = act;
            }
            nn->outputs = lay;
        }
        else
        {
            lay->num_neur = num_hidd_neur;
            lay->neur_array = malloc(sizeof(neur) * num_hidd_neur);
            for(unsigned int m = 0; m < num_hidd_neur; m++)     //creation d'un
            {                                                   //hidden layer
                neur *act = malloc(sizeof(neur));
                act->value = 0;
                act->biase = (double) rand() / RAND_MAX * 2.0 - 1.0;
                act->num_weights = nn->layer_array[i-1]->num_neur;
                act->weights = malloc(sizeof(neur) * act->num_weights);
                for(unsigned int l = 0; l < act->num_weights; l++)
                {
                    act->weights[l] = (double) rand() / RAND_MAX * 2.0 - 1.0;
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
            printf("\t neur:%u  key:%lf \n \t biase:%lf \n", j,
                      nn->layer_array[i]->neur_array[j]->value,
                      nn->layer_array[i]->neur_array[j]->biase);
            if(i != 0)
            {
                for(unsigned int k = 0;
                        k < nn->layer_array[i]->neur_array[j]->num_weights;
                        k++)
                {
                    printf("\t\t weigth%u : %lf \n", k,
                            nn->layer_array[i]->neur_array[j]->weights[k]);
                }
            }
        }
    }
    printf("\n");
}




void neur_compute(neur *n, layer *prev_layer)	   //Utilise le layer precedent
{                                                   //pour compute le neurone
   double new_value = 0;
   for(unsigned int i = 0; i < prev_layer->num_neur; i ++)
   {
	    new_value += (prev_layer->neur_array[i]->value) * (n->weights[i]);
   }
   new_value += n->biase;
   n->value = sigmoid(new_value);
}


void layer_compute(layer *l, layer *prev_layer)
{
   for(unsigned int i = 0; i < l->num_neur; i++)
   {
	neur_compute(l->neur_array[i], prev_layer);
   }
}

void inputs_fill(neur_net *nn, double *inputs)
{
	for(unsigned int i = 0; i < nn->layer_array[0]->num_neur ; i++)
	{
		nn->layer_array[0]->neur_array[i]->value = inputs[i];           //SEGFAULT
	}

}



double *feed_forward(neur_net *nn, double *inputs)
{
    inputs_fill(nn, inputs);
    for(unsigned int i = 1; i < nn->num_arrays; i++)
    {
        layer_compute(nn->layer_array[i],nn->layer_array[i-1]);
    }

    unsigned int output_num = nn->layer_array[nn->num_arrays - 1]->num_neur;    //creation d'un array de double a renvoyer
    double *res = malloc(sizeof(double) * output_num);                          //basé sur les values des neurones de l'output layer
    for(unsigned int i = 0; i < output_num; i++)                                //apres feedforward
    {
        res[i] = nn->layer_array[nn->num_arrays-1]->neur_array[i]->value;
    }
    return res;
}

/*

void backprop(neur_net *nn, double *inputs, double *target,
                                                        double learning_rate)
{
    //TODO
}

*/

double sigmoid(double x)
{
    return (1.0 / (1 + exp(-x)));
}

double const *nn_run(neur_net const *nn, double const *inputs)
{
    //TODO modify fonctions according to our parameters ]
    return NULL;

}
/*

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
