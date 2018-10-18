#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neural_network.h"

int main()
{
    srand(time(NULL));
    neur_net *nn = instantiate(4,1,3,1);
    pretty_print(nn);
    return 0;
}

