#include "lang/preprocess.h"

#include <stdio.h>
#include <string.h>

void preprocess(char *in_name, char *out_name){
        FILE *in = fopen(in_name, "r");
        FILE *out = fopen(out_name, "w");

        char line[256];
        while(fgets(line, 256, in)){
                if (strncmp(line, "!!", 2) == 0){
                        if (strncmp(line + 2, "munch", 5) == 0){

                                char file_name[256];
                                strcpy(file_name, "../modules/");
                                strcat(file_name, line + 8);


                                file_name[strlen(file_name) - 1] = 0;

                                strcat(file_name, ".pop");

                                FILE *module = fopen(file_name, "r");

                                while (fgets(line, 256, module)){
                                        fputs(line, out);
                                }

                                fclose(module);
                        }
                }
                else {
                        fputs(line, out);
                }
        }

        fclose(in);
        fclose(out);
}
