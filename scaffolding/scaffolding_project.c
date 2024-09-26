#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void create_file(const char *path, const char *content) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Could not create file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", content);
    fclose(file);
}

void create_directory(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);  // Create directory with permissions
    }
}

void create_project(const char *project_name) {
    char include_dir[256], src_dir[256], build_dir[256];
    char file_path[256];

    // Create directory structure
    sprintf(include_dir, "%s/include", project_name);
    sprintf(src_dir, "%s/src", project_name);
    sprintf(build_dir, "%s/build", project_name);

    create_directory(project_name);
    create_directory(include_dir);
    create_directory(src_dir);
    create_directory(build_dir);

    // Create hello.h
    sprintf(file_path, "%s/hello.h", include_dir);
    create_file(
        file_path,
        "#ifndef HELLO_H\n"
        "#define HELLO_H\n\n"
        "void hello(void);\n\n"
        "#endif\n"
    );

    // Create hello.c
    sprintf(file_path, "%s/hello.c", src_dir);
    create_file(
        file_path,
        "#include <stdio.h>\n"
        "#include \"hello.h\"\n\n"
        "void hello(void) {\n"
        "    printf(\"Hello, world!\\n\");\n"
        "}\n"
    );

    // Create main.c
    sprintf(file_path, "%s/main.c", src_dir);
    create_file(
        file_path,
        "#include \"hello.h\"\n\n"
        "int main() {\n"
        "    hello();\n"
        "    return 0;\n"
        "}\n"
    );

    // Create Makefile content
    char makefile_content[1024];
    sprintf(makefile_content,
        "# Makefile\n\n"
        "CC = gcc\n"
        "CFLAGS = -Wall -Iinclude\n"
        "SRC_DIR = src\n"
        "BUILD_DIR = build\n"
        "TARGET = $(BUILD_DIR)/%s\n\n"
        "all: $(TARGET)\n\n"
        "$(TARGET):\n"
        "	$(CC) $(CFLAGS) $(SRC_DIR)/hello.c $(SRC_DIR)/main.c -o $(TARGET)\n\n"
        "$(SRC_DIR)/%%.o: $(SRC_DIR)/%%.c\n"
        "	$(CC) $(CFLAGS) -c $< -o $%%.o\n\n"
        "clean:\n"
        "	rm -f $(TARGET)\n\n"
        ".PHONY: all clean\n",
        project_name
    );

    // Create Makefile
    sprintf(file_path, "%s/Makefile", project_name);
    create_file(file_path, makefile_content);

    printf("Project '%s' scaffold created successfully!\n", project_name);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <project_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    create_project(argv[1]);
    return EXIT_SUCCESS;
}
