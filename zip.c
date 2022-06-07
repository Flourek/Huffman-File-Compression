//
// Created by Flourek on 23/05/2022.
//

#include "header.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"


void zip (FILE *src_file, char dest_path[]) {

    EpiFile *zip = calloc(1, sizeof(EpiFile));

    char *p_extension = strrchr(dest_path, '.') + 1;
    strncpy(zip->extension, p_extension, 8);
    strcpy(p_extension, "epi");                         //replace it with "epi"

    // get the file size - number of bytes to zip
    fseek(src_file, 0, SEEK_END);
    int src_size = ftell(src_file);
    fseek(src_file, 0, SEEK_SET);

    // load the contents of the file into a string
    unsigned char *input = calloc(sizeof(char), src_size + 1);
    fread(input, sizeof(char), src_size, src_file);
    input[src_size] = '\0';

    // build a huffman tree out of the data_str and create a map of codes, where a symbol is the key
    Node *head = huffman_tree(input, src_size);
    char *empty = calloc(256,1);
    unsigned char **codes_map = calloc(256, 256);
    codes_map = generate_codes(head, empty, codes_map);

    // get the number of existing codes
    int codes_map_len = 0;
    for (int i = 0; i < 256; ++i)
        if(codes_map[i]) codes_map_len++;
    zip->codes_map_len = codes_map_len;

    zip->codes_map = calloc(codes_map_len, sizeof(CodeBlock));

    // turn the codes map into a binary format
    for (int i = 0, j = 0; i < 256; ++i)
        if(codes_map[i]) {
            CodeBlock data;
            data.code = strtoul(codes_map[i], 0, 2);
            data.offset = strlen(codes_map[i]);
            data.symbol = i;

            zip->codes_map[j++] = data;
        }


    // translate the original contents to a continous string of huffman codes
    char *data_str = calloc(src_size * 10, sizeof(char));
    uint64_t data_strlen = 0;

    for (int i = 0; i < src_size; i++){
        char *code = codes_map[input[i]];
        strcat(data_str, code);
        data_strlen += strlen(code);
    }


    // convert the huffman codes to integers
    // the last block can be shorter so we have to store it's offset

    int n_blocks_to_write = 1 + data_strlen / DATA_BLOCK_SIZE;
    char buffer[DATA_BLOCK_SIZE + 1] = "";

    zip->last_block_offset = 0;
    zip->data_blocks = calloc(n_blocks_to_write, sizeof(DataBlock));

    for (int i = 0, j = 0; i < data_strlen; i += DATA_BLOCK_SIZE) {
        strncpy(buffer, data_str + i, DATA_BLOCK_SIZE);

        if (i + DATA_BLOCK_SIZE >= data_strlen) {                           // if last block
            int len = strlen(buffer);
            zip->last_block_offset = DATA_BLOCK_SIZE - len;
            memset(buffer + len, '0', zip->last_block_offset);
        }

        zip->data_blocks[j++] = strtoul(buffer, 0, 2);
//        printf("\n%s", buffer);
    }


    // write to file :)
    FILE *out = fopen(dest_path, "wb");
    //    FILE *out = create_file(dest_path);

    fwrite(zip->extension, sizeof(char), 8, out);
    fwrite(&zip->codes_map_len, sizeof(int), 1, out);
    fwrite(zip->codes_map, sizeof(CodeBlock), codes_map_len, out);
    fwrite(&zip->last_block_offset, sizeof(int), 1, out);
    fwrite(zip->data_blocks, sizeof(DataBlock), n_blocks_to_write, out);

    fclose(out);

}

