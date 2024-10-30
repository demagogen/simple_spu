#ifndef TEXT_DATA_H_
#define TEXT_DATA_H_

#include <cstdio>

struct LINE_DATA {
    int lines_lengths         = 0;
    char* lines_pointers      = NULL;
    char* lines_ends_pointers = NULL;
};

struct TEXT_DATA {
    int digits            = 0;
    int lines             = 0;
    LINE_DATA *LineData   = NULL;
    char  *text           = NULL;
};

int count_symbols            (FILE *file_handle, TEXT_DATA *text_data);
int separate_text_on_strings (TEXT_DATA *text_data);
int fill_text                (FILE *fp, TEXT_DATA *text_data);
int count_strings            (TEXT_DATA *text_data);
int fill_lines_pointers      (TEXT_DATA *text_data);
int print_text               (FILE *file_handle, TEXT_DATA *text_data);
int free_text_data           (TEXT_DATA *text_data);

#endif

