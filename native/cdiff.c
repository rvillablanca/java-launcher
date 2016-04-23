#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

struct s_props {
    //    Required
    int version;
    int is_jar;
    char* program_name;

    //    Maybe required
    char* _main_class;
    char* _jar_file;

    //    Not required
    char* _class_path;
    char* _java_opts;

};

typedef struct s_props props;

//Logic functions
static int check_version(char* command);
static void run_program(char* command, char *args[]);
static props* read_props();
static void free_props(props* p);
static int check_props(props* p);

//Utility
static char *strstrip(char *s);
static char* get_str_value(FILE* file, char* property);
static void error(char* error);
static void p_error(char* error);
static void safe_free(char* prop);

int main(int argc, char* argv[]) {
    props * p = read_props();
    if (!p) {
        error("Cannot create props");
        exit(-1);
    }
    int valid = check_props(p);
    if (!valid) {
        error("Configuration file is not valid, check all options values");
        exit(-1);
    }
    valid = check_version("java");
    if (valid == 1) {
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
    char output[1024];
    FILE* fp;
    fp = popen(full_command, "r");
    if (fp != NULL) {
        if (fgets(output, sizeof (output), fp) != NULL) {
        } else {
            puts("Cannot get output command\n");
        }
        pclose(fp);
        return 1;
    } else {
        perror("Error while executing command");
        return 0;
    }
}

static props* read_props() {
    props* p = NULL;
    FILE* file = fopen("cdiff_test.cfg", "r");
    if (file != NULL) {
        p = (props*) malloc(sizeof (props));
        char* version = get_str_value(file, "version");
        char* is_jar = get_str_value(file, "is_jar");
        p->_main_class = get_str_value(file, "main_class");
        p->_class_path = get_str_value(file, "class_path");
        p->_java_opts = get_str_value(file, "java_opts");
        p->program_name = get_str_value(file, "program_name");
        p->_jar_file = get_str_value(file, "jar_file");
        p->version = version != NULL ? atoi(version) : 0;
        p->is_jar = is_jar != NULL ? atoi(is_jar) : 0;
        fclose(file);
    } else {
        p_error("Cannot open properties file");
    }
    return p;
}

static int check_props(props* p) {
    if (p->version == 0) {
        error("version option must be set");
        return 0;
    }
    if (p->is_jar == 0) {
        error("is_jar option must be set");
        return 0;
    }
    if (p->is_jar) {
        if (!p->_jar_file) {
            error("jar_file option must be set");
            return 0;
        }
    } else {
        if (!p->_main_class) {
            error("main_class option must be set");
            return 0;
        }
    }

    return 1;
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
                int index = rest - line;
                char* opt = line + index + 1;
                opt = strstrip(opt);
                printf("[%s][%zu]\n", opt, strlen(opt));
                int size = strlen(opt) + 1;
                value = (char*) malloc(sizeof (char) * size);
                strcpy(value, opt);
                break;
            }
        }
    }
    free(line);
    return value;
}

char* strstrip(char* s) {
    size_t size;
    char* end;

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
    safe_free(p->_main_class);
    safe_free(p->_class_path);
    safe_free(p->_class_path);
    safe_free(p->_java_opts);
    safe_free(p->program_name);
    free(p);
}

static void safe_free(char* prop) {
    if (prop) {
        free(prop);
    }
}

static void run_program(char* command, char *args[]) {

}

static void error(char* error) {
    fprintf(stderr, "%s", error);
}

static void p_error(char* error) {
    perror(error);
}