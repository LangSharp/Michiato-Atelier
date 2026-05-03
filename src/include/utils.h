#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME 64
#define MAX_DESC 128
#define MAX_LINE 512
#define DATE_FORMAT_LEN 8
#define MAX_RECORDS 1000

typedef enum {
    OP_SUCCESS = 0,
    OP_ERROR_NOT_FOUND,
    OP_ERROR_DUPLICATE_KEY,
    OP_ERROR_INVALID_DATA,
    OP_ERROR_NO_STOCK,
    OP_ERROR_FILE_IO,
    OP_ERROR_FULL,
    OP_ERROR_INVALID_ID
} OperationResult;

void utils_trim(char *str);
void utils_get_current_date(char *date_buffer);
int utils_get_int_input(const char *prompt);
double utils_get_double_input(const char *prompt);
void utils_get_string_input(const char *prompt, char *buffer, size_t max_len);
int utils_confirm(const char *message);
void utils_pause(void);
void utils_clear_screen(void);
void utils_print_header(const char *title);
void utils_print_separator(void);

#endif