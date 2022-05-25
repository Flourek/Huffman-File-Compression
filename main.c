#include <stdlib.h>
#include <string.h>
#include "header.h"

// TOCOMMENCE
// binary search in that one place
// besser names
// dynamic resize input



int main(int argc, char *argv[]) {

//    FILE * f = fopen("255.txt", "wb");
//    for (int i = 0; i < 255; ++i) {
//        fprintf(f, "%c", i);
//
//    }


    if(argc != 2) { printf("learn to use"); return 1; }

    char src_path[PATH_MAX];
    strcpy(src_path, argv[1]);


    FILE *src_file = fopen(src_path, "rb");
    if (!src_file) { printf("cannot creat fil\n"); return 1; }





    if(strstr(argv[1], ".epi")){
        printf("is epi :DDDD\n") ;
        unzip(src_file, src_path);
    }else{
        printf("not epi :((((\n") ;
        zip(src_file, src_path);
    }



    printf("\nsuccess :D");
    fclose(src_file);
}
