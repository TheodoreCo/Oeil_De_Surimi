#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neural_network.h"

int main(int argc, char *argv[])
{
    /* New network with 2 inputs,
     * 1 hidden layer of 2 neurons,
     * and 1 output. */
    neur_net *nn = nn_new_instance(2, 1, 2, 1);
    printf("Before learning...");
    nn_to_string(nn);

    const double learning_rate = 2.0;
    const int num_epochs = 500;
    nn_xor_learn(nn, num_epochs, learning_rate);

    printf("After learning...");
    nn_to_string(nn);

    /* Run the network and see what it predicts. */
    const double inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

    printf("Output for [0.0, 0.0] is %1.f.\n", *nn_run(nn, inputs[0]));
    printf("Output for [0.0, 1.0] is %1.f.\n", *nn_run(nn, inputs[1]));
    printf("Output for [1.0, 0.0] is %1.f.\n", *nn_run(nn, inputs[2]));
    printf("Output for [1.0, 1.0] is %1.f.\n", *nn_run(nn, inputs[3]));

    nn_free(nn);
    return 0;
}
