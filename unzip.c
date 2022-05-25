//
// Created by Flourek on 23/05/2022.
//

#include "header.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

int unzip(FILE * src_file, char dest_path[]) {

    char dest_extension[PATH_MAX];
    fread(dest_extension, 8, 1, src_file);

    // set the destination path extension to the encoded one
    char *p_extension = strrchr(dest_path, '.') + 1;
    strncpy(p_extension, dest_extension, strlen(dest_extension));


    // turn the encoded tree information in the 2nd line into a nice lookup table
    Key *codes_dict = calloc(256, sizeof(Key));

    int dict_size = 0;
    fread(&dict_size, sizeof(int), 1, src_file);


    for (int i = 0; i < dict_size; ++i) {
        Block data = {0};
        fread(&data, sizeof(Block), 1, src_file);

        codes_dict[i].value = data.symbol;
        strcpy(codes_dict[i].code, int_to_binary_str(data.code) + 32 - data.offset);

    }

    codes_dict = realloc(codes_dict, dict_size * sizeof(Key));



//    for (int i = 0; i < dict_size; ++i) {
//        printf("%c%s ", codes_dict[i].value, codes_dict[i].code);
//    }


    char *binary = calloc(1024, sizeof(char));
    uint64_t block_c = 0, binary_length = 0, binary_size = 1024;
    uint32_t block;

    while(fread(&block, sizeof(uint32_t), 1, src_file)){
        if(binary_length - 16 >= binary_size){
            binary_size += 1024;
            binary = realloc(binary, binary_size * 2 * sizeof(char));
        }

        char *code = int_to_binary_str(block);
        strcat(binary, code);

        binary_length += strlen(code);
        block_c++;
    }

    // the last block contains the number of surplus bits in the previous one
    int cutoff = (block_c - 1) * sizeof(uint32_t) * 8 - block;
    binary[cutoff] = 0;



    FILE *out = create_file(dest_path);
//    FILE *out = fopen("original2.txt", "wb");

    // decode the huffman encoded_tree, iterates over a dict with descending probabilities
    char buffer[256] = "";
    for (int i = 0; i < strlen(binary); ++i) {
        strncat(buffer, &binary[i], 1);

        for (int j = 0; j < dict_size; ++j) {
            if(strcmp(buffer, codes_dict[j].code) == 0){
                fprintf(out, "%c", codes_dict[j].value);
                strset(buffer, 0);
            }
        }
    }

    fclose(out);

    return 0;
}