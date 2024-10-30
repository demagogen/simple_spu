#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <cstdio>

#include "text_data.h"
#include "utils.h"
#include "color_scheme.h"

int count_symbols(FILE *file_handle, TEXT_DATA *text_data) {
    assert(file_handle);
    assert(text_data);

    if (!text_data) {
        graphic_printf(RED, BOLD, "*text_data null pointer in count_symbols\n");
        return ALLOCATION_ERROR;
    }

    fseek(file_handle, 0, SEEK_END);
    text_data->digits = ftell(file_handle);
    if (text_data->digits < 0) {
        graphic_printf(RED, BOLD, "*text_data null pointer in count_symbols\n");
    }
    fseek(file_handle, 0, SEEK_SET);

    return 0;
}

int fill_text(FILE *file_handle, TEXT_DATA *text_data) {
    assert(file_handle);
    assert(text_data);

    if (!text_data) {
        graphic_printf(RED, BOLD, "*text_data null pointer in fill_text\n");
        return MEMORY_ERROR;
    }
    // TODO write macro
    if (!file_handle) {
        graphic_printf(RED, BOLD, "*fp null pointer in fill_text\n");
        return MEMORY_ERROR;
    }

    text_data->text = (char*) calloc(text_data->digits + 1, sizeof(char));
    if (!text_data->text) {
        graphic_printf(RED, BOLD, "calloc allocation error in fill_text\n");
        return ALLOCATION_ERROR;
    }

    fread(text_data->text, sizeof(char), text_data->digits, file_handle);

    separate_text_on_strings   (text_data);
    count_strings              (text_data);
    fill_lines_pointers        (text_data);

    return 0;
}

int separate_text_on_strings(TEXT_DATA *text_data) {
    assert(text_data);

    ERROR_DATA error_info = PROGRAM_ERROR;
    if (!text_data) {
        graphic_printf(RED, BOLD, "*fp null pointer in separate_text_on_strings\n");
        return MEMORY_ERROR;
    }
    for (size_t digit_index = 0; digit_index < text_data->digits; digit_index++) {
        if (text_data->text[digit_index] == '\n') {
            text_data->text[digit_index] =  '\0';
        }
    }

    return 0;
}

int count_strings(TEXT_DATA *text_data) {
    assert(text_data);

    if (!text_data) {
        graphic_printf(RED, BOLD, "null pointer on text_data in count_strings");
        return MEMORY_ERROR;
    }

    text_data->lines = -1;
    for (size_t digit_index = 0; digit_index < text_data->digits; digit_index++) {
        if (text_data->text[digit_index] == '\0') {
            text_data->lines++;
        }
    }

    return 0;
}

int fill_lines_pointers(TEXT_DATA *text_data) {
    assert(text_data);

    if (!text_data) {
        graphic_printf(RED, BOLD, "null pointer error in fill_lines_pointers");
        return MEMORY_ERROR;
    }
    size_t line_pointer_index = 0;
    text_data->LineData = (LINE_DATA*) calloc(text_data->digits, sizeof(LINE_DATA));
    text_data->LineData[line_pointer_index].lines_pointers =        text_data->text;
    text_data->LineData[line_pointer_index].lines_lengths  = strlen(text_data->text);
    line_pointer_index++; //TODO fix this in onegin

    for (size_t digit_index = 0; digit_index < text_data->digits; digit_index++) {
        if (text_data->text[digit_index] == '\0') {
            delete_extra_spaces(text_data, digit_index, line_pointer_index);
            line_pointer_index++;
        }
    }

    return 0;
}

int print_text(FILE *file, TEXT_DATA *text_data) {
    assert(text_data);
    assert(file);

    if (!text_data) {
        graphic_printf(RED, BOLD, "null pointer error in print_text");
        return MEMORY_ERROR;
    }

    for (size_t line_index = 0; line_index < text_data->lines; line_index++) {
        if (text_data->text[line_index] != '\0' || text_data->text[line_index] != '\n') {
            if (text_data->LineData[line_index].lines_lengths == 0) {
                continue;
            }
            fprintf(file, "%s\n", text_data->LineData[line_index].lines_pointers);
        }
    }

    return 0;
}

int free_text_data(TEXT_DATA *text_data) {
    assert(text_data);

    free(text_data->LineData);
    free(text_data->text);

    return 0;
}
