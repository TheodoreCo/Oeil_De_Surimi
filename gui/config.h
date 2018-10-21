#ifndef FILE_OPERATION_H_INCLUDED
#define FILE_OPERATION_H_INCLUDED

#define CONFIG_ARG_MAX_BYTES 128

/*
    This struct will contain all the parameters used in the application.
    // TODO: enrich this struct whenever a new parameter is 'discovered'

    NB: This is the simplest "properties" handler one could imagine.
    The tradeoff is, of course, unflexibility --
        e.g. if parameter 'xx' is defined in the file, the structure should contain an 'xx' field
*/

/** Structure to be filled when reading a properties files */
typedef struct config {
    // Threshold to pass from color to B & W image ((color to be turned to 0 for all pixels with color < b_and_w_threshold)
    double b_and_w_thresold;
    // Number of neurons in the input layer for the XOR NN
    int num_input_xor;
    //Number of neurons in the output layer for the XOR NN
    int num_output_xor;
    //  Number of hidden layers for the XOR NN
    int num_hid_lay_xor;
    // Number of hidden neurons in a hidden layer for the XOR NN
    int num_hid_neur_xor;
    // Number of training epochs for the XOR NN
    int num_train_epochs_xor;
    // Expansion for RLSA function
    unsigned int rlsa_expansion;
    // Number of neurons in the input layer for the OCR NN
    int num_input_ocr;

} config;

config *cf_read_config(char *path);

void cf_key_val_to_string(char *path);

double cf_get_b_and_w_threshold(void);
int cf_get_num_input_xor(void);
int cf_get_num_output_xor(void);
int cf_get_num_hid_lay_xor(void);
int cf_get_num_hid_neur_xor(void);
int cf_get_num_input_ocr(void);
int cf_get_num_train_epochs_xor(void);
unsigned int cf_get_rlsa_expansion(void);

/** Frees the static config structure */
void cf_free_config(void);

#endif // FILE_OPERATION_H_INCLUDED
