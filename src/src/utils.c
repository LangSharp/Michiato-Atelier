#include "utils.h"
#include <time.h>

void utils_trim(char *str) {
    char *start = str;
    char *end;

    while (isspace((unsigned char)*start)) start++;

    if (*start == 0) {
        *str = 0;
        return;
    }

    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    *(end + 1) = 0;

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

void utils_get_current_date(char *date_buffer) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    snprintf(date_buffer, DATE_FORMAT_LEN, "%02d%02d%02d",
             (unsigned int)tm_info->tm_mday,
             (unsigned int)(tm_info->tm_mon + 1),
             (unsigned int)(tm_info->tm_year % 100));
}

int utils_get_int_input(const char *prompt) {
    char buffer[32];
    int value = 0;

    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        utils_trim(buffer);
        value = atoi(buffer);
    }
    return value;
}

double utils_get_double_input(const char *prompt) {
    char buffer[32];
    double value = 0;

    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        utils_trim(buffer);
        value = atof(buffer);
    }
    return value;
}

void utils_get_string_input(const char *prompt, char *buffer, size_t max_len) {
    printf("%s", prompt);
    if (fgets(buffer, max_len, stdin) != NULL) {
        utils_trim(buffer);
    }
    buffer[max_len - 1] = 0;
}

int utils_confirm(const char *message) {
    char response[8];
    printf("%s (s/n): ", message);
    if (fgets(response, sizeof(response), stdin) != NULL) {
        utils_trim(response);
        return (response[0] == 's' || response[0] == 'S');
    }
    return 0;
}

void utils_pause(void) {
    char buffer[256];
    printf("\nPresione Enter para continuar...");
    fflush(stdout);
    fgets(buffer, sizeof(buffer), stdin);
}

void utils_clear_screen(void) {
    printf("\033[2J\033[H");
}

void utils_print_header(const char *title) {
    int len = strlen(title);
    int padding = (40 - len) / 2;

    printf("\n");
    utils_print_separator();
    printf("%*s%s\n", padding + len % 2, "", title);
    utils_print_separator();
}

void utils_print_separator(void) {
    printf("========================================\n");
}
