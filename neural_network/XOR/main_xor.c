#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <err.h>

#include "../neural_network.h"

int main(void)
{
	/*
	if (argc != 3)
	{
		errx(1, "Wrong inputs parameters, please follow this pattern :\n \
			-1st input value (0 or 1)\n \
			-2nd input value (0 or 1)\n");
	}
	*/

//ASSIGNATIONS

	// neur_net *neural = nn_load("xor.nn");


    srand(time(NULL));

	unsigned int inputs_number = 2,
                 hidd_lay_num = 1,
                 hidd_neur_num = 3,
                 output_number = 1 ;

   //  double *inputs = calloc(inputs_number,sizeof(double));
//    double target[1] = {1};

   // inputs[0] = (unsigned int) atoi(argv[1]);
   // inputs[1] = (unsigned int) atoi(argv[2]);
	/*
	inputs[0] = (unsigned int) 0;
	inputs[1] = (unsigned int) 1;
	*/

    neur_net *nn = instantiate(inputs_number,hidd_lay_num,
            hidd_neur_num,output_number);

//LOAD DU RESEAU


//PRETTY PRINT PRE-TRAITEMENT

    pretty_print(nn);
    printf("\n");

//CREATION DE L'ARRAY D'OUTPUT

 //  double *outputs = malloc(sizeof(double) * output_number);

//EXECUTION DU FEEDFORWARD

   // outputs = feed_forward(nn,inputs);

//PRETTY PRINT POST TRAITEMENT

    printf("\n POST TRAITEMENT \n\n");

//EXECUTION DE LA BACKPROP

	xor_train(nn,0.1,10000);

	pretty_print(nn);


//SAVE DU FICHIER

	nn_save(nn,"xor.nn");

    return 0;
}

