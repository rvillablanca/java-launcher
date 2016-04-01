#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int check_version(char* command);

int main(int argc, char *argv[]) {
    check_version("java");
    return 0;
}

static int check_version(char* command) {
    char full_command[150];
    char* version_command = " -version 2>&1";
    memset(full_command, 0, 150);
    sprintf(full_command, "%s %s", command, version_command);
    printf("Comando a ejecutar: %s\n", full_command);
    char output[1024];
    FILE* fp;
    fp = popen(full_command, "r");
    if (fp != NULL) {
        puts("Comando ejecutado");
        if (fgets(output, sizeof (output), fp) != NULL) {
            printf("Salida: %s", output);
        } else {
            puts("No se pudo obtener la salida del comando\n");
        }
        pclose(fp);
        return 1;
    } else {
        perror("Error al ejecutar comando");
        return 0;
    }
}
