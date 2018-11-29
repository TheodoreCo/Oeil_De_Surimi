#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <err.h>

#include "neural_network.h"

neur_net creation()
{

	unsigned int inputs_number = 256;
	unsigned int hidd_neur_num,hidd_lay_num;
	unsigned int output_number = 93;

	scanf("Nombre de hidden layers : %u\nNombre de neurones par hidden layers : %u\n",&hidd_lay_num, &hidd_neur_num);

    srand(time(NULL));
    
    double *inputs = calloc(inputs_number,sizeof(double));

    neur_net *nn = instantiate(inputs_number,hidd_lay_num,
            hidd_neur_num,output_number);

   	double *outputs = malloc(sizeof(double) * output_number);

	nn->outputs = outputs;

	pretty_print(nn);

	return nn;
}



int main(int argc, char *argv[])
{

    if(argc != 5)
    {
        errx(1,"Wrong inputs, please follow this pattern : \n \
                -Number of inputs\n \
                -Number of outputs\n \
                -Number of hidden layers\n \
                -Number of neurons per hidden layer \n");
    }

// PROCEDURE DE CHOIX

	unsigned int choix = 0;
	do{
    	printf("Que voulez-vous faire ?\n1. Créer un réseau de neurones\n2. Entrainer un réseau existant\n3. Utiliser un réseau existant\n\n");
    	scanf("%d",&choix);
    	switch(choix)
    	{
			case 1 :
				neur_net *nn = creation();
				char file[50];
				scanf("Nom du reseau : %c\n", &file);
				nn_save(nn, file);
				break;

			case 2 : 
				char file[50];
				scanf("Nom du reseau a charger : %c\n", &file);
				neur_net *nn = nn_load(file);
				print("Before training network : \n");
				pretty_print(nn);
				ocr_train(nn, 0.1);
				pretty_print(nn);
				break;

			case 3 :
				
				// Chargement reseau
				scanf("Nom du reseau a charger : %c\n", &file);
				neur_net *nn = nn_load(file);
				

				//Recuperation image et linearisation matrice
				
				
				
				//Execution feedforward
				double *outputs = feed_forward(nn, inputs);

				//Recuperation char de sortie
				char max = 0;
				for(int i = 0; i < 93; i++)
				{
					if(*outputs > max)
						max = *outputs;

					outputs++;
				}
				char += 33;
				break;
	
			default:
				break;
		}
	}while(choix > 3);

	return 0;
}

