#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <err.h>

#include "neural_network.h"

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

    printf("Que voulez-vous faire ?\n1. Créer un réseau de neurones\n2. Entrainer un réseau existant\n3. Utiliser un réseau existant\n\n");
    int choix = 0;
    scanf("%d",&choix);
    switch(choix)
    {case 1 :




//ASSIGNATIONS

    srand(time(NULL));
    unsigned int inputs_number = (unsigned int) atoi(argv[1]), 
                 output_number = (unsigned int) atoi(argv[2]),
                 hidd_lay_num = (unsigned int) atoi(argv[3]),
                 hidd_neur_num = (unsigned int) atoi(argv[4]);

    double *inputs = calloc(inputs_number,sizeof(double));

    neur_net *nn = instantiate(inputs_number,hidd_lay_num,
            hidd_neur_num,output_number);

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

