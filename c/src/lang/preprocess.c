#include "lang/preprocess.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void include_modules(char *in_name, char *out_name){
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
                        else if (strncmp(line + 2, "-", 1) != 0){
                                // invalid preprocessor directive
                                assert(0);
                        }
                }
                else {
                        fputs(line, out);
                }
        }
        fclose(in);
        fclose(out);
}

void remove_comments(char *in_name, char *out_name){
        FILE *in = fopen(in_name, "r");
        FILE *out = fopen(out_name, "w");

        char line[256];
        while(fgets(line, 256, in)){
                // check if comment present in line
                char *pos = strstr(line, "!!-");
                if (pos){
                        // if one is, then ignore comment text by null-terminating whe comment starts
                        *pos = 0;
                }

                fputs(line, out);
        }

        fclose(in);
        fclose(out);
}

void preprocess(char *in_name, char *out_name){
        include_modules(in_name, "a.pop");
        remove_comments("a.pop", out_name);
        remove("a.pop");
}
