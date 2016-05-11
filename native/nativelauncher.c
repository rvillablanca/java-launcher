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
    char* is_jar;
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
static int check_version(char* command, int required);
static void run_program(char* command, props p);
static props* read_props();
static void free_props(props* p);
static int check_props(props* p);

//Utility
static char* strstrip(char *s);
static char* get_str_value(char* property);
static void error(char* error);
static void safe_free(char* prop);
static void replace_char(char* source, char old, char new);

int main(int argc, char* argv[]) {
    props * p = read_props();
    int valid = check_version("java", p->version);
    if (valid == 1) {
        run_program("java", *p);
    } else {
        char* java_home = getenv("JAVA_HOME");
        if (java_home != NULL) {
            char command[1001];
            char* suffix = "bin/java";
            memset(command, 0, 1001);
            snprintf(command, 1000, "%s%s", java_home, suffix);
            printf("%s\n", command);
            valid = check_version(command, p->version);
            if (valid == 1) {
                run_program(command, *p);
            }
        } else {
            printf("Java Home null\n");
        }

    }
    free_props(p);
    return 0;
}

static int check_version(char* command, int required) {
    int result = 0;
    char full_command[151];
    char* version_command = " -version 2>&1";
    memset(full_command, 0, 151);
    snprintf(full_command, 150, "%s %s", command, version_command);
    char output[1024];
    FILE* fp;
    fp = popen(full_command, "r");
    if (fp != NULL) {
        if (fgets(output, sizeof (output), fp) != NULL) {
            char* version = "version";
            char* p = strstr(output, version);
            if (p != NULL) {
                p = p + 7;
                replace_char(p, '"', ' ');
                p = strstrip(p);
                int v = atoi(p + 2);
                result = v >= required;
            } else {
                puts("Version not found");
            }
        } else {
            puts("Cannot get output command\n");
        }
        pclose(fp);
    } else {
        perror("Error while executing command");
    }
    return result;
}

static void replace_char(char* source, char old, char new) {
    int max = strlen(source);
    for (int i = 0; i < max; i++) {
        if (source[i] == old) {
            source[i] = new;
        }
    }
}

static props* read_props() {
    props* p = (props*) malloc(sizeof (props));
    char* version = get_str_value("version");
    p->_main_class = get_str_value("main_class");
    p->_class_path = get_str_value("class_path");
    p->_java_opts = get_str_value("java_opts");
    p->program_name = get_str_value("program_name");
    p->_jar_file = get_str_value("jar_file");
    p->is_jar = get_str_value("is_jar");
    p->version = version != NULL ? atoi(version) : 0;

    safe_free(version);
    int valid = check_props(p);
    if (!valid) {
        error("Configuration file is not valid, check all options values");
        free_props(p);
        exit(-1);
    }
    return p;
}

static int check_props(props * p) {
    if (!p->version) {
        error("version option must be set");
        return 0;
    }
    if (!p->is_jar) {
        error("is_jar option must be set");
        return 0;
    }
    if (strcmp(p->is_jar, "true") == 0) {
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

char* get_str_value(char* property) {
    char* value = NULL;
    FILE* file = fopen("launcher.conf", "r");
    if (file != NULL) {
        char* line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, file)) != -1) {
            char aux[read + 1];
            memcpy(aux, line, read + 1);
            if (strstr(aux, property) != NULL) {
                char* aux_2 = strstrip(aux);
                char* rest = strchr(aux_2, ' ');
                if (rest != NULL) {
                    int index = rest - aux_2;
                    char* opt = aux_2 + index + 1;
                    opt = strstrip(opt);
                    int size = strlen(opt) + 1;
                    if (size > 1) {
                        value = (char*) malloc(sizeof (char) * size);
                        memcpy(value, opt, size);
                        break;
                    }
                }
            }
        }
        free(line);
        fclose(file);
    } else {
        perror("Cannot open properties file");
    }
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

static void free_props(props * p) {
    safe_free(p->_main_class);
    safe_free(p->_class_path);
    safe_free(p->_java_opts);
    safe_free(p->_jar_file);
    safe_free(p->program_name);
    safe_free(p->is_jar);
    free(p);
}

static void safe_free(char* prop) {
    if (prop) {
        free(prop);
    }
}

static void run_program(char* command, props p) {
    size_t len = strlen(command);
    if (p._class_path) {
        len += strlen(p._class_path);
    }
    if (p._jar_file) {
        len += strlen(p._jar_file);
    }
    if (p._main_class) {
        len += strlen(p._main_class);
    }
    if (p._java_opts) {
        len += strlen(p._java_opts);
    }

    len += strlen(p.program_name);
    len = len + 1024;
    char buffer[len];
    memset(buffer, 0, len);
    char* aux = buffer;
    int val = sprintf(aux, "%s ", command);
    aux += val;
    if (p._java_opts) {
        val = sprintf(aux, "%s ", p._java_opts);
        aux += val;
    }
    if (strcmp(p.is_jar, "true") == 0) {
        val = sprintf(aux, "-jar %s ", p._jar_file);
        aux += val;
    } else {
        if (p._class_path) {
            val = sprintf(aux, "-cp %s ", p._class_path);
            aux += val;
        }
        sprintf(aux, "%s", p._main_class);
    }
    printf("[%lu][%zu][%s]\n", sizeof (buffer), strlen(buffer), buffer);
}

static void error(char* error) {
    fprintf(stderr, "%s\n", error);
}
