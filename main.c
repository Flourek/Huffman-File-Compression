#include <string.h>
#include "windows.h"
#include "wincon.h"
#include "header.h"

int main(int argc, char *argv[]) {
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND consoleWnd = GetConsoleWindow();
    DWORD dwProcessId;
    GetWindowThreadProcessId(consoleWnd, &dwProcessId);
    int launched_from_cmd = GetCurrentProcessId()!=dwProcessId;

    if(argc < 2) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("No file specified!\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
        if(!launched_from_cmd) getchar();
        exit(0);
    }

    char src_path[PATH_MAX] = "";
    strncpy(src_path, argv[1], PATH_MAX);

    FILE *src_file = fopen(src_path, "rb");

    if (!src_file) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("Cannot open file!\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
        if(!launched_from_cmd) getchar();
        exit(0);
    }

    char file_name[PATH_MAX] = "";
    char * tmp_file_name = strrchr(src_path, '\\');
    if(!tmp_file_name) tmp_file_name = strrchr(src_path, '/');
    if(!tmp_file_name)
        tmp_file_name = src_path;
    else
        tmp_file_name++;  // get rid of the "/" or "\"
    strncpy(file_name, tmp_file_name, PATH_MAX);

    if(strstr(argv[1], ".epi")){

        printf("Uncompressing \'%s\'\n\n", file_name) ;
        unzip(src_file, src_path);


    }else{
        printf("Compressing file \'%s\'\n", file_name) ;

        zip(src_file, src_path);

        strcpy(strrchr(src_path, '.') + 1, "epi");
        FILE * zip_file = fopen(src_path, "rb");

        int src_size = get_file_size(src_file);
        int zip_size = get_file_size(zip_file);
        fclose(zip_file);



        float size_difference = 100 - ((float) zip_size / (float) src_size ) * 100;
        if (size_difference > 0){
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("\nFilesize reduced by %.2f%\n", size_difference);
        }else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("\nFilesize increased by %.2f% :(\n", -size_difference);
        }
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
    if(!launched_from_cmd) {
        printf("\nPress enter to exit...");
        getchar();
    }

    fclose(src_file);
}
