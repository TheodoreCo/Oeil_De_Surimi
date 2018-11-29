#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <err.h>

#include "neural_network.h"

int main(/* int argc, char *argv[] */)
{
	
	// INSTIATE NN

	size_t num_inputs = 256;
	size_t num_hidden_layers = 2;
	size_t num_hidd_neur = 3;
	size_t num_outputs = 93;

	neur_net *nn = instantiate(num_inputs,num_hidden_layers,num_hidd_neur,num_outputs);

	nn_save(nn,"Reseau_test");

	return 0;
}

