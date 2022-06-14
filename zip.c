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

    printf("\nGenerating Huffman tree...\n");

    CodeMap * map = huffmanify(input, src_size);

    // Pretty print the codes, ironically the code isn't pretty
    for (int i = 0; i < map->len; ++i){
        if(i % 5 == 0)
            printf("\n");

        switch(map->codes[i].symbol) {
            case '\b': case '\t': case '\n': case '\r': case '\a':
                printf(" ");
                break;
            default:
                printf("%c", map->codes[i].symbol);
        }

        printf(" %s ", map->codes[i].code);
        if(map->codes[i].len < 5)
            printf("\t\t");
        else
            printf("\t");
    }

    zip->codes_map = calloc(sizeof(CodeBlock), map->len);
    zip->codes_map_len = map->len;
    for (int i = 0; i < map->len; ++i) {
        zip->codes_map[i] = (CodeBlock){map->codes[i].symbol, map->codes[i].len};
    }

    CanonCode hashed_codes[256] = {0};
    for (int i = 0; i < map->len; ++i)
        hashed_codes[map->codes[i].symbol] = map->codes[i];


    char *data_str = calloc(src_size * 10, sizeof(char));
    uint64_t data_strlen = 0;

    printf("\n\nConverting the input to Huffman codes...\n");

    // translate the original contents to a continous string of huffman map
    for (int i = 0; i < src_size; i++){
        unsigned char symbol = input[i];

        strncpy(data_str + data_strlen,
                hashed_codes[symbol].code,
                hashed_codes[symbol].len
                );

        data_strlen += hashed_codes[symbol].len;
    }


    // convert the huffman map to a packed form in 32 bit integers
    // the last block can be shorter so we have to store it's offset

    printf("Writing to file...\n");

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
    }

    FILE *out = create_file(dest_path);

    fwrite(zip->extension,          sizeof(zip->extension),             1,                 out);
    fwrite(&zip->codes_map_len,     sizeof(zip->codes_map_len),         1,                 out);
    fwrite(zip->codes_map,          sizeof(CodeBlock),                  map->len,          out);
    fwrite(&zip->last_block_offset, sizeof(zip->last_block_offset),     1,                 out);
    fwrite(zip->data_blocks,        sizeof(DataBlock),                  n_blocks_to_write, out);

    fclose(out);
}

