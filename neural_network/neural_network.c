#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <err.h>

#include "neural_network.h"

neur_net *instantiate(size_t num_inputs, size_t num_hidden_layers,
        size_t num_hidd_neur, size_t num_outputs)
{
    unsigned int layer_num = num_hidden_layers + 2; //Nombre total de layers
	neur_net *nn = malloc(sizeof(neur_net));    //malloc du reseau nn

    nn->layer_array = malloc(sizeof(layer*) * layer_num);   //malloc de l'array
    nn->num_arrays = layer_num;                             //de layer de nn

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
    if (nn)
    {
        for(unsigned int i=0; i<nn->num_arrays; i++)
        {
            layer *l_layer = nn->layer_array[i];

            if(l_layer)
            {
                for(unsigned int j=0; j<l_layer->num_neur; j++)
                {
                    neur *l_neur = l_layer->neur_array[j];
                    if(l_neur)
                    {
                        if(l_neur->num_weights > 0)
                            free(l_neur->weights);
                        free(l_neur);
                    }
                }
                free(l_layer->neur_array);
                free(l_layer);
            }
        }
        free(nn->layer_array);
        free(nn);
        nn = NULL;
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
		nn->layer_array[0]->neur_array[i]->value = inputs[i];
	}

}



double *feed_forward(neur_net *nn, double *inputs)
{
    inputs_fill(nn, inputs);
    for(unsigned int i = 1; i < nn->num_arrays; i++)
    {
        layer_compute(nn->layer_array[i],nn->layer_array[i-1]);
    }

    //creation d'un array de double a renvoyer
    //basé sur les values des neurones de l'output layer
    //apres feedforward

    unsigned int output_num = nn->layer_array[nn->num_arrays - 1]->num_neur;
    double *res = malloc(sizeof(double) * output_num);
    for(unsigned int i = 0; i < output_num; i++)
    {
        res[i] = nn->layer_array[nn->num_arrays-1]->neur_array[i]->value;
    }
    return res;
}



void backprop(neur_net *nn, double *inputs, double *target,
                                                        double learning_rate)
{
    feed_forward(nn,inputs);

    for(unsigned int i = nn->num_arrays-1; i >= 1; i--)
	{

	    	layer *l = nn->layer_array[i];
		for(unsigned int j = 0; j < l->num_neur; j++)
			{
				neur *n = l->neur_array[j];
				double d_x = n->value * (1 - n->value);
				n->d_y = 0;
				if(i == nn->num_arrays-1)
				{
					n->d_y = n->value - target[j];
				}
				else
				{
					for(unsigned int k = 0; k < nn->layer_array[i+1]->num_neur; k++)
					{
						neur *next_neur = nn->layer_array[i+1]->neur_array[k];
						n->d_y += (next_neur->weights[j] * (next_neur->value *
							(1 - next_neur->value)) * next_neur->d_y);
					}
				}
				n->biase -= learning_rate * n->d_y * d_x;
				for(unsigned int k = 0; k < n->num_weights; k++)
				{
					double d_w = nn->layer_array[i-1]->neur_array[k]->value;
					n->weights[k] -= learning_rate * d_w * n->d_y * d_x;

				}

			}

	}


}

/*
void ocr_train(neur_net *nn, double learning_rate)
{

}
*/

void xor_train(neur_net *nn, double learning_rate)
{
	double param_1[2] = {0,0},
		param_2[2] = {1,1},
		param_3[2] = {1,0},
		param_4[2] = {0,1};
	double target_1 = 0,
		target_2 = 0,
		target_3 = 1,
		target_4 = 1;


	for(unsigned int i = 0; i < 100000; i++)
	{
		backprop(nn,param_1, &target_1, learning_rate);
		backprop(nn,param_2, &target_2, learning_rate);
		backprop(nn,param_3, &target_3, learning_rate);
		backprop(nn,param_4, &target_4, learning_rate);
	}
	nn_save(nn, "xor.nn");

}




double sigmoid(double x)
{
    return (1.0 / (1 + exp(-x)));
}


void nn_save(neur_net *nn, char *path)
{
	// VALABLE POUR LE XOR

	// OUVERTURE DU FICHIER

    FILE* file = NULL;

    file = fopen(path, "w+");

    if(file == NULL)
    {
    errx(1,"Writing error; can't save the network");
    }

	//INFOS POUR INSTANTIATE

	fprintf(file,"%u %u %u %u",
		nn->layer_array[0]->num_neur,
		nn->num_arrays - 2,
		nn->layer_array[1]->num_neur,
		nn->layer_array[nn->num_arrays - 1]->num_neur);

	// STOCKAGE

	for(unsigned int i = 0; i < nn->num_arrays; i++)
	{
		fprintf(file,"%u[",i);


		for(unsigned int j = 0; j < nn->layer_array[i]->num_neur; j++)
		{
			fprintf(file,"%u",j);
			fprintf(file,"(%lf ",
				nn->layer_array[i]->neur_array[j]->biase);
			for(unsigned int k = 0;
			  k < nn->layer_array[i]->neur_array[j]->num_weights;
			  k++)
			{
				fprintf(file,"|%lf",
				nn->layer_array[i]->neur_array[j]->weights[k]);
			}
			fprintf(file,")");
		}
		fprintf(file,"]");
	}
	fprintf(file,"~"); //EOF

	//FERMETURE DU FICHIER

	fclose(file);

}



neur_net *nn_load(char *path)
{

	// OUVERTURE DU FICHIER

    FILE* file = NULL;

    file = fopen(path,"r");

    if(file == NULL)
    {
	    errx(1,"reading error; can't load the file");
    }

	// LECTURES PARAMETRES DE BASES
	// NUM_INPUTS - NUM_HIDD_LAYERS - NUM_HIDD_NEUR - NUM_OUTPUTS

	unsigned int num_inputs = 0;
	unsigned int num_hidden_layers = 0;
	unsigned int num_hidd_neur = 0;
	unsigned int num_outputs = 0;

	fscanf(file, "%u %u %u %u",
		&num_inputs,
		&num_hidden_layers,
		&num_hidd_neur,
		&num_outputs);

	//INSTANTIATION DU NN

	neur_net *nn = instantiate(num_inputs,
				num_hidden_layers,
				num_hidd_neur,
				num_outputs);


	int layer_count = 0,
		neur_count = 0,
		weight_count = 0;

	double act_weight = 0,
		act_biase = 0;

	//TRAITEMENT PREMIER LAYER
	
	unsigned int layer_counts;
	fscanf(file,"%d[",&layer_counts);
	int carac = fgetc(file);
	while(carac != ']')
	{
		fscanf(file,"%d",&neur_count);
		fscanf(file,"(%lf )",&act_biase);

		//ATTRTIBUTION AU NN

		nn->layer_array[layer_count]->neur_array[neur_count]->biase =
			act_biase;

		carac = fgetc(file);
	}


	// TRAITEMENT GENERAL FROM 1ST HIDDEN LAYER

	layer_count = 1;
	while(carac != '~')
	{

		fscanf(file,"%d[",&layer_count);
		do
		{
			fscanf(file,"%d(",&neur_count);
			fscanf(file,"%lf ",&act_biase);

			nn->layer_array[layer_count]->neur_array[neur_count]->biase =
				act_biase;

			carac = fgetc(file);
			weight_count = 0;
			while(carac != ')')
			{
				fscanf(file,"%lf",&act_weight);
				nn->layer_array[array_count]->neur_array[neur_count]->weight[weight_count];
				weight_count++;
				carac = fgetc(file);
			}
			carac = fgetc(file);
		} while(carac != "]")
		carac = fgetc(file);
	return nn;
}



