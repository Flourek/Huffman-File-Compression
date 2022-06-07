//
// Created by Flourek on 23/05/2022.
//

#include "header.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

void unzip(FILE * src_file, char dest_path[]) {

    EpiFile *zip = calloc(1, sizeof(EpiFile));

    fread(zip->extension, sizeof(char), 8, src_file);
    fread(&zip->codes_map_len, sizeof(int), 1, src_file);

    // set the destination path extension to the encoded one
    char *p_extension = strrchr(dest_path, '.') + 1;
    strncpy(p_extension, zip->extension, sizeof(zip->extension));

    // turn the encoded tree information in the a nice map
    Key *codes_map = calloc(zip->codes_map_len, sizeof(Key));

    for (int i = 0; i < zip->codes_map_len; ++i) {
        CodeBlock data = {0};
        fread(&data, sizeof(CodeBlock), 1, src_file);

        codes_map[i].value = data.symbol;
        strcpy(codes_map[i].code, int_to_binary_str(data.code) + DATA_BLOCK_SIZE - data.offset);
    }

    fread(&zip->last_block_offset, sizeof(int), 1, src_file);

    // turn the binary encoded data into a string of the huffman codes
    char *data_str = calloc(KB(1), sizeof(char));
    uint64_t block_count = 0, data_str_len = 0, data_size = KB(1);
    DataBlock block;

    while(fread(&block, sizeof(DataBlock), 1, src_file)){
        if(data_str_len - MAX_CODE_LEN >= data_size){
            data_size += KB(1);
            data_str = realloc(data_str, data_size * 2);
        }

        char *code = int_to_binary_str(block);
        strncat(data_str, code, DATA_BLOCK_SIZE);

        data_str_len += strlen(code);
        block_count++;
    }

    data_str_len -= zip->last_block_offset;
    data_str[data_str_len] = 0;

    FILE *out = create_file(dest_path);
//    FILE *out = fopen("original2.txt", "wb");

    // decode the huffman codes
    char buffer[DATA_BLOCK_SIZE + 1] = "";
    for (int i = 0; i < data_str_len; ++i) {
        strncat(buffer, &data_str[i], 1);

        for (int j = 0; j < zip->codes_map_len; ++j) {
            if(strcmp(buffer, codes_map[j].code) == 0){
                fprintf(out, "%c", codes_map[j].value);
                strset(buffer, 0);
            }
        }
    }

    fclose(out);

}