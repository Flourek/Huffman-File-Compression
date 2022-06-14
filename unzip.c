//
// Created by Flourek on 23/05/2022.
//

#include "header.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

void unzip(FILE * src_file, char dest_path[]) {

    EpiFile *zip = calloc(1, sizeof(EpiFile));

    fread(zip->extension,      sizeof(zip->extension), 1, src_file);
    fread(&zip->codes_map_len, sizeof(zip->codes_map_len), 1, src_file);

    // set the destination path extension to the encoded one

    char *p_extension = strrchr(dest_path, '.') + 1;
    strncpy(p_extension, zip->extension, sizeof(zip->extension));

    // turn the encoded tree information in the a nice map
    CodeMap * map = calloc(1, sizeof(CodeMap));
    map->len = zip->codes_map_len;
    map->codes = calloc(map->len, sizeof(CanonCode));

    printf("Reconstructing Huffman codes...\n");

    //reconstruct the tree infromation
    for (int i = 0; i < map->len; ++i) {
        CodeBlock data = {0};
        fread(&data, sizeof(CodeBlock), 1, src_file);

        map->codes[i].symbol = data.symbol;
        map->codes[i].len = data.len;
    }

    generate_canonical_codes(map);

//        printf("%c \t%s \t%d  \n", map->codes[i].symbol, map->codes[i].code, strtol(map->codes[i].code, 0 , 2));




    fread(&zip->last_block_offset, sizeof(zip->last_block_offset), 1, src_file);

    // turn the binary encoded data into a string of the huffman codes
    char *data_str = calloc(KB(1), sizeof(char));
    uint64_t block_count = 0, data_strlen = 0, data_size = KB(1000);
    DataBlock block;


    while(fread(&block, sizeof(DataBlock), 1, src_file)){
        if(data_strlen - MAX_CODE_LEN >= data_size){
            data_size += KB(10000);
            data_str = realloc(data_str, data_size * 2);
        }

        char *code = int_to_binary_str(block);
        strncpy(data_str + data_strlen, code, DATA_BLOCK_SIZE);

        data_strlen += DATA_BLOCK_SIZE;
        block_count++;

    }


    // account for the surplus of bits in the last block
    data_strlen -= zip->last_block_offset;
    data_str[data_strlen + 1] = 0;

    FILE *out = create_file(dest_path);
//    FILE *out = fopen("testen/256 e.txt", "wb");


    printf("Converting the codes back into the original...\n");
    printf("Writing to file...\n");

    CanonCode hashed_codes[512] = {0};
    for (int i = 0; i < map->len; ++i)
        hashed_codes[strtol(map->codes[i].code, 0 , 2)] = map->codes[i];

    // convert the huffman codes to the original symbols, accorduing to the codes_map
    char buffer[DATA_BLOCK_SIZE + 1] = "";
    int buffer_strlen = 0;

    for (int i = 0; i < data_strlen; ++i) {
        buffer[buffer_strlen] = data_str[i];
        buffer_strlen++;
        int hash_index = strtol(buffer, 0 , 2);

        if (strcmp(buffer, hashed_codes[hash_index].code) == 0){
            fprintf(out, "%c", hashed_codes[hash_index].symbol);
            memset(buffer, 0, DATA_BLOCK_SIZE + 1);
            buffer_strlen = 0;
        }
    }

    fclose(out);

}