#ifndef NEURAL_NETWORK_H_INCLUDED
#define NEURAL_NETWORK_H_INCLUDED

// Best structure to be defined. See, e.g., https://github.com/mnielsen/neural-networks-and-deep-learning/blob/master/src/network.py
// for a different approach.
struct nn {
    int num_inputs;     /* Number of inputs */
    int num_hid_lay;    /* Number of hidden layers */
    // TODO: this structure does not allow to define e.g. 2 hidden layers with a different number of neurons in each hidden layer.
    int num_hid_neur;   /* Number of neurons per hidden layer */
    int num_outputs;    /* Number of outputs */

    // ....
};

// Instantiate neural network
struct nn *nn_new_instance(int num_inputs, int num_hid_lay, int num_hid_neur, int num_outputs);

#endif // NEURAL_NETWORK_H_INCLUDED
