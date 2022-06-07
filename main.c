#include <string.h>
#include "header.h"
#include "windows.h"
#include "pthread.h"
#include "unistd.h"

int globalhehe = 1;

_Noreturn void * printanim(void * arg){
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    while (globalhehe){
        sleep(1);
        printf(".");
    }
}

int main(int argc, char *argv[]) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if(argc < 2) { printf("Please provide a filepath\n"); return 1; }

    char src_path[PATH_MAX];
    strcpy(src_path, argv[1]);

    FILE *src_file = fopen(src_path, "rb");
    if (!src_file) { printf("Cannot open file!\n"); return 1; }


    if(strstr(argv[1], ".epi")){
        printf("Uncompressing file...\n") ;
        unzip(src_file, src_path);
    }else{
        printf("Compressing file") ;
        pthread_t io;
        pthread_create(&io, NULL, printanim, NULL);

        zip(src_file, src_path);
        globalhehe = 0;
        pthread_join(io, NULL);


        strcpy(strrchr(src_path, '.') + 1, "epi");
        FILE * zip_file = fopen(src_path, "rb");

        int src_size = get_file_size(src_file);
        int zip_size = get_file_size(zip_file);
        fclose(zip_file);

        float size_difference = ((float) zip_size / (float) src_size ) * 100;
        if (size_difference < 100){
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("\nFilesize reduced by %.2f%\n", size_difference);
        }else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("\nFilesize increased by %.2f% :(\n", size_difference);
        }

    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    printf("Press enter to exit...");
    getchar();

    fclose(src_file);
}
