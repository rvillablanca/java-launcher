#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

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

char *strstrip(char *s);
char* get_str_value(FILE* file, char* property);

int main(int argc, char* argv[]) {
    props * p = read_props();
    int check = check_version("java");
    if (check == 1) {
        //        printf("Java encontrado\n");
    }
    free_props(p);
    return 0;
}

static int check_version(char* command) {
    char full_command[150];
    char* version_command = " -version 2>&1";
    memset(full_command, 0, 150);
    sprintf(full_command, "%s %s", command, version_command);
    //    printf("Comando a ejecutar: %s\n", full_command);
    char output[1024];
    FILE* fp;
    fp = popen(full_command, "r");
    if (fp != NULL) {
        //        puts("Comando ejecutado");
        if (fgets(output, sizeof (output), fp) != NULL) {
            //            printf("Salida: %s", output);
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

static props* read_props() {
    props* p = (props*) malloc(sizeof (props));
    FILE* file = fopen("cdiff_test.cfg", "r");
    if (file != NULL) {
        char* version = get_str_value(file, "version");
        char* main_class = get_str_value(file, "main_class");
        char* class_path = get_str_value(file, "class_path");
        char* java_opts = get_str_value(file, "java_opts");
        char* program_name = get_str_value(file, "program_name");
        char* is_jar = get_str_value(file, "is_jar");
        char* jar_file = get_str_value(file, "jar_file");
        fclose(file);
    } else {
        perror("No se pudo abrir el archivo de propiedades");
        exit(-1);
    }
    return p;
}

char* get_str_value(FILE* file, char* property) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char* value = NULL;
    while ((read = getline(&line, &len, file)) != -1) {
        if (strstr(line, property) != NULL) {
            char* rest = strchr(line, ' ');
            if (rest != NULL) {
                printf("[%s]", line);
                int index = rest - line;
                char* opt = line + index + 1;
                opt = strstrip(opt);
                printf("[%s][%zu]", opt, strlen(opt));
                int size = strlen(opt) + 1;
                value = (char*) malloc(sizeof (char) * size);
                strcpy(value, opt);
            }
        }
    }
    free(line);
    return value;
}

char *strstrip(char* s) {
    size_t size;
    char *end;

    size = strlen(s);

    if (!size) {
        return s;
    }

    end = s + size - 1;
    while (end >= s && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    while (*s && isspace(*s)) {
        s++;
    }

    return s;
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
