#include "./include/blockdag.h"

int main(int argc, char* argv[]) {
    if (argc > 3) {
        perror("Too much line commands");
        return EXIT_FAILURE;
    }

    if (strncmp(argv[1], "-c1", MAX_COMM_LEN) == 0) {
        check_valid_dag();
    } else if (strncmp(argv[1], "-c2", MAX_COMM_LEN) == 0) {
        graph_sets(argv[2]);
    } else {
        perror("Command unknown");
        return EXIT_FAILURE;
    }

    return 0;
}