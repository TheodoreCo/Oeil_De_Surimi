// Instantiate neural network
struct nn *nn_new_instance(int num_inputs, int num_hid_lay, int num_hid_neur, int num_outputs)
{
    // Minimalist checks
    if (
        num_inputs < 1  ||                          // No inputs...
        num_outputs < 1 ||                          // ... or no outputs...
        num_hid_lay < 1 ||                          // ... or no hidden layers...
        (num_hid_lay >= 1 && num_hid_neur < 1))     // ... or no hidden neurons
    {
        return 0;
    }

    // Compute number of weights
    int no_of_weights =
        num_inputs * num_hid_neur +                         // between input & hidden
        num_hid_neur * num_hid_neur * (num_hid_lay - 1) +   // between hidden layers (if more than one hidden layer)
        num_hid_neur * num_outputs;                         // between hidden & output

    int no_of_bias = num_inputs + num_hid_lay * num_hid_neur;



    // TODO
    return 0;
}
