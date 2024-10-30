#ifndef UTILS_H_
#define UTILS_H_

#include <cstdio>

#include "text_data.h"

enum ERROR_DATA {
    PROGRAM_ERROR    = -4,
    INPUT_ERROR      = -3,
    MEMORY_ERROR     = -2,
    FILE_ERROR       = -1,
    ALLOCATION_ERROR = 0
};

const char* error_data_enum(ERROR_DATA error_inf);

int delete_punctuation_endings (TEXT_DATA *text_data);
int delete_extra_spaces        (TEXT_DATA *text_data, size_t start_digit_index, size_t line_pointer_index);
int swap                       (void *value1, void *value2, size_t size);

#endif
