//
// Created by Flourek on 23/05/2022.
//

#include "header.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"


int zip (FILE *src_file, char dest_path[]) {

    char extension[PATH_MAX] = "";
    // gotta save the original extension src_file the .epi, without the dot too
    char *p_extension = strrchr(dest_path, '.') + 1;
    strcpy(extension, p_extension);                     //save the original extension
    strcpy(p_extension, "epi");                         //replace it with "epi"



    // get the symbols

    fseek(src_file, 0, SEEK_END);
    int src_size = ftell(src_file);
    fseek(src_file, 0, SEEK_SET);

    unsigned char *input = calloc(sizeof(char), src_size + 1);
    fread(input, sizeof(char), src_size, src_file);
    input[src_size] = '\0';

//    printf("%s\n", input);

    Node *head = huffman_tree(input, src_size);
    char *empty = calloc(256,1);
    unsigned char **codes_array = calloc(256, 256);
    codes_array = generate_codes(head, empty, codes_array);




    FILE *out = fopen(dest_path, "wb");
//    FILE *out = create_file(dest_path);

    fwrite(extension, 8, 1, out);



    int codes_array_len = 0;
    for (int i = 0; i < 256; ++i)
        if(codes_array[i])
            codes_array_len++;

    fwrite(&codes_array_len, sizeof(int), 1, out);


    for (int i = 0; i < 256; ++i)
        if(codes_array[i]) {
            Block data;
            data.code = strtoul(codes_array[i], 0, 2);
            data.offset = strlen(codes_array[i]);
            data.symbol = i;
            fwrite(&data, sizeof(Block), 1, out);

//            printf("%d%c %s %d\n", i, i, codes_array[i], data.offset);
        }

    // translate the original to huffman codes
    char *data = calloc(src_size * 10, sizeof(char));
    uint64_t data_length = 0;

    for (int i = 0; i < src_size; i++){
//        if(data_length >= data_size){
//            data_size += input_size;
//            data = realloc(data, data_size);
//        }

        char *code = codes_array[input[i]];
        strcat(data, code);

        data_length += strlen(code);
    }





    // convert the huffman codes to integers and write them to the file
    // the last block has a different size so we have to store it's offset
    char buffer[BLOCK_SIZE + 1] = "";
    int last_block_offset = 0;
    for (int i = 0; i < strlen(data); i += BLOCK_SIZE) {

        strncpy(buffer, data + i, BLOCK_SIZE);

        int len = strlen(buffer);
        if(len < BLOCK_SIZE){
            last_block_offset = BLOCK_SIZE - len;
            memset(buffer+len, '0', last_block_offset);
        }

        uint32_t chunk = strtoul(buffer, 0, 2);
        fwrite(&chunk, sizeof(uint32_t), 1, out);
//        printf("\n%s", buffer);

        strcpy(buffer, "");
    }

    fwrite(&last_block_offset, sizeof(int), 1, out);
    fclose(out);

    return 0;
}

