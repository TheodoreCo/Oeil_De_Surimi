#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <err.h>
#include <dirent.h>

#include "neural_network.h"

int main()
{
	
	// INSTIATE NN

	size_t num_inputs = 256;
	size_t num_hidden_layers = 2;
	size_t num_hidd_neur = 3;
	size_t num_outputs = 93;

	neur_net *nn = instantiate(num_inputs,num_hidden_layers,num_hidd_neur,num_outputs);
	
	ocr_train(nn,0.01,100);

	return 0;
	
}
