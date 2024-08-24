#include "error_handling.h"
#include <stdio.h>

void log_error(const char *error_message) {
    fprintf(stderr, "Error: %s\n", error_message);
}

void handle_error(int error_code) {
    switch (error_code) {
        case 1:
            log_error("Database connection failed.");
            break;
        case 2:
            log_error("Query execution error.");
            break;
        default:
            log_error("Unknown error.");
            break;
    }
}
