#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "config.h"

/**
    A global config structure. It is intended to be created once (except if new config file loaded)
*/
static config *app_config;

config *cf_read_config(char *path) {
    if(!app_config) {
        app_config = malloc(sizeof(config));
        // Changes locale in order to be able to parse '.' as a decimal separarator
        // NB: This very same line as the first line in main.c has no effect if GUI is used,
        //  probably because some GTK code plays with setlocale, too.
        // TODO: Restore value of locale after config is read? Seems it's not needed for the time being.
        setlocale (LC_NUMERIC , "C");
    } else {
        return app_config;
    }

    if(path == NULL)
        path = "./def_config.txt";

    FILE *fp;

    if ((fp = fopen(path, "r")) == NULL)
    {
        perror("fopen()");
        return NULL;
    }

    // Fixed-size key and value arrays
    char key[CONFIG_ARG_MAX_BYTES];
    char value[CONFIG_ARG_MAX_BYTES];

    while (1) {
        if(fscanf(fp, "%s = %s", key, value) != 2) {
            if(feof(fp))
                break;
            if(key[0] == '#') {
                while (fgetc(fp) != '\n') {} // Read the entire line
                continue;
            }
            perror("fscanf()"); // Current line is neither a comment nor a string with a middle ' = ' sequence
            continue;
        }

        // At this point, we should have a correct key/value pair
        if(!strcmp(key, "b_and_w_threshold")) {
            app_config->b_and_w_thresold = atof(value);
        }
        else if(!strcmp(key, "num_input_xor")) {
            app_config->num_input_xor = atoi(value);
        }
        else if(!strcmp(key, "num_output_xor")) {
            app_config->num_output_xor = atoi(value);
        }
        else if(!strcmp(key, "num_hid_lay_xor")) {
            app_config->num_hid_lay_xor = atoi(value);
        }
        else if(!strcmp(key, "num_hid_neur_xor")) {
            app_config->num_hid_neur_xor = atoi(value);
        }
        else if(!strcmp(key, "num_train_epochs_xor")) {
            app_config->num_train_epochs_xor = atoi(value);
        }
        else if(!strcmp(key, "rlsa_expansion")) {
            app_config->rlsa_expansion = atoi(value);
        }
        else if(!strcmp(key, "num_input_ocr")) {
            app_config->num_input_ocr = atoi(value);
        }
        else if(!strcmp(key, "num_output_ocr")) {
            app_config->num_output_ocr = atoi(value);
        }
        else if(!strcmp(key, "num_hid_lay_ocr")) {
            app_config->num_hid_lay_ocr = atoi(value);
        }
        else if(!strcmp(key, "num_hid_neur_ocr")) {
            app_config->num_hid_neur_ocr = atoi(value);
        }
        else if(!strcmp(key, "ocr_train_file")) {
            app_config->ocr_train_file = malloc(CONFIG_ARG_MAX_BYTES + 1);
            strcpy(app_config->ocr_train_file, value);
        }
        else if(!strcmp(key, "num_train_epochs_ocr")) {
            app_config->num_train_epochs_ocr = atoi(value);
        }
        else if(!strcmp(key, "learn_rate_ocr")) {
            app_config->learn_rate_ocr = atof(value);
        }
        else if(!strcmp(key, "learn_rate_xor")) {
            app_config->learn_rate_xor = atof(value);
        }
    }

    if(fp)
        fclose(fp);

    return app_config;
}

void cf_key_val_to_string(char *path)
{
    //cf_get_app_config(path);
    if(app_config) {
        printf("**************** Oeil_de_Surimi config file ******************\n");
        printf("b_and_w_threshold=%f\n", app_config->b_and_w_thresold);
        printf("num_input_xor=%d\n", app_config->num_input_xor);
        printf("num_output_xor=%d\n", app_config->num_output_xor);
        printf("num_hid_lay_xor=%d\n", app_config->num_hid_lay_xor);
        printf("num_hid_neur_xor=%d\n", app_config->num_hid_neur_xor);
        printf("num_train_epochs_xor=%d\n", app_config->num_train_epochs_xor);
        printf("learn_rate_xor=%f\n", app_config->learn_rate_xor);
        printf("num_input_ocr=%d\n", app_config->num_input_ocr);
        printf("num_output_ocr=%d\n", app_config->num_output_ocr);
        printf("num_hid_lay_ocr=%d\n", app_config->num_hid_lay_ocr);
        printf("num_hid_neur_ocr=%d\n", app_config->num_hid_neur_ocr);
        printf("num_train_epochs_ocr=%d\n", app_config->num_train_epochs_ocr);
        printf("learn_rate_ocr=%f\n", app_config->learn_rate_ocr);
        printf("rlsa_expansion=%d\n", app_config->rlsa_expansion);
        printf("ocr_train_file=%s\n", app_config->ocr_train_file);
        printf("**************** Oeil_de_Surimi config file ******************\n");
    }
}

double cf_get_b_and_w_threshold(void) {
    if(app_config)
        return app_config->b_and_w_thresold;
    return -1.0;
}

int cf_get_num_input_xor(void) {
    if (app_config)
        return app_config->num_input_xor;
    return -1;
}

int cf_get_num_output_xor(void) {
    if (app_config)
        return app_config->num_output_xor;
    return -1;
}

int cf_get_num_hid_lay_xor(void) {
    if (app_config)
        return app_config->num_hid_lay_xor;
    return -1;
}

int cf_get_num_hid_neur_xor(void) {
    if (app_config)
        return app_config->num_hid_neur_xor;
    return -1;
}

int cf_get_num_train_epochs_xor(void) {
    if (app_config)
        return app_config->num_train_epochs_xor;
    return -1;
}

unsigned int cf_get_rlsa_expansion(void) {
    if(app_config)
        return app_config->rlsa_expansion;
    return 0;
}

int cf_get_num_input_ocr(void) {
    if(app_config)
        return app_config->num_input_ocr;
    return -1;
}

int cf_get_num_output_ocr(void) {
    if(app_config)
        return app_config->num_output_ocr;
    return -1;
}

int cf_get_num_hid_lay_ocr(void) {
    if(app_config)
        return app_config->num_hid_lay_ocr;
    return -1;
}

int cf_get_num_hid_neur_ocr(void) {
    if(app_config)
        return app_config->num_hid_neur_ocr;
    return -1;
}

char *cf_get_ocr_train_file(void) {
    if(app_config)
        return app_config->ocr_train_file;
    return NULL;
}

double cf_get_learn_rate_xor(void) {
    if(app_config)
        return app_config->learn_rate_xor;
    return 0.0;
}
double cf_get_learn_rate_ocr(void) {
    if(app_config)
        return app_config->learn_rate_ocr;
    return 0.0;
}

int cf_get_num_train_epochs_ocr(void) {
    if(app_config)
        return app_config->num_train_epochs_ocr;
    return -1;
}

void cf_free_config(void) {
    if(app_config) {
        if(app_config->ocr_train_file)
            free(app_config->ocr_train_file);
        free(app_config);
    }
}

