#include "neural_network.h"
#define num_inputs 2
#define num_outputs 2

int main(int argc, char *argv[])
{
    neur_net *nn = nn_new_instance(num_inputs, 2, 2, num_outputs);
    nn_to_string(nn);

    double inputs[num_inputs] = {0.56, 0.23};
    double const *result = nn_run(nn, inputs);
    nn_to_string(nn);

    printf("\nResult of computation was: \n");
    for(int i=0; i<num_outputs; i++) {
        printf("%.5f ", result[i]);
    }
    printf("\n");

    nn_free(nn);
    return 0;
}
