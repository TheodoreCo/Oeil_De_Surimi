#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <err.h>

#include "../neural_network.h"

int main()
{
    srand(time(NULL));

	unsigned int inputs_number = 2,
                 hidd_lay_num = 1,
                 hidd_neur_num = 3,
                 output_number = 1 ;


    neur_net *nn = instantiate(inputs_number,hidd_lay_num,
            hidd_neur_num,output_number);
	
	xor_train(nn,0.1,10000);
	
	//neur_net *neural = nn_load("xor.nn");
    //double *outputs = malloc(sizeof(double) * output_number);
    //outputs = feed_forward(nn,inputs);
	nn_save(nn,"xor.nn");

    return 0;
}

