#include "neural_network.h"

int main(int argc, char *argv[])
{
    neur_net *nn = nn_new_instance(2, 1, 2, 2);
    nn_to_string(nn);

    nn_free(nn);
    return 0;
}
