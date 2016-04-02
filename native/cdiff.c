#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

struct s_props {
    int version;
    char* main_class;
    char* class_path;
    char* java_opts;
    char* program_name;
    int is_jar;
    char* jar_file;
};

typedef struct s_props props;

static int check_version(char* command);
static void run_program(char* command, char *args[]);
static props* read_props();
static void free_props(props* props);
static long file_size(FILE* file);

int main(int argc, char *argv[]) {
    props * p = read_props();
    int check = check_version("java");
    if (check == 1) {
        printf("Java encontrado\n");
    }
    free_props(p);
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
        exit(-1);
    }
}

static long file_size(FILE* file) {
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return size;
}

static props* read_props() {
    props* p = (props*) malloc(sizeof (props));
    FILE* file = fopen("cdiff.cfg", "r");
    if (file != NULL) {
        puts("Leyendo archivo cdiff.cfg");
        long size = file_size(file);
        size += 1;
        printf("Tamaño: %ld bytes\n", size);
        //        size += 1;
        char content[size];
        memset(content, 0, size);
        fread(content, size, 1, file);
        fclose(file);
        printf("Contenido: [%s][%ld][%zd]\n", content, sizeof (content), strlen(content));
    } else {
        perror("No se pudo abrir el archivo de propiedades");
        exit(-1);
    }
    return p;
}

static void free_props(props* p) {
    //    free(p->main_class);
    //    free(p->class_path);
    //    free(p->java_opts);
    //    free(p->program_name);
    //    free(p->jar_file);
    free(p);
}

static void run_program(char* command, char *args[]) {

}
