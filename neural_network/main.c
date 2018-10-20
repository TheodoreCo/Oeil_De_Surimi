#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neural_network.h"

int main()
{

//ASSIGNATIONS

    srand(time(NULL));
    unsigned int inputs_number = 4, 
                 output_number = 1,
                 hidd_lay_num = 1,
                 hidd_neur_num = 3;

    double *inputs = malloc(sizeof(double) * inputs_number);

    neur_net *nn = instantiate(inputs_number,hidd_lay_num,hidd_neur_num,output_number);

//PRETTY PRINT PRE-TRAITEMENT

    pretty_print(nn);
    printf("\n");

//CREATION DE L'ARRAY D'OUTPUT

   double *outputs = malloc(sizeof(double) * output_number);

//EXECUTION DU FEEDFORWARD

    outputs = feed_forward(nn,inputs);

//PRETTY PRINT POST TRAITEMENT 
    
    pretty_print(nn);
    printf("\n *** OUTPUTS DU FEED FORWARD *** \n"); 
    for(unsigned int i = 0; i < output_number; i++)
    {
        printf("Output %u = %lf \n",i,outputs[i]);
    }
    printf("\n");
    return 0;
}

