#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstdint>

#include "utils.h"
#include "color_scheme.h"

int delete_extra_spaces(TEXT_DATA *text_data, size_t start_digit_index, size_t line_pointer_index) {
    assert(text_data);

    if (!text_data) {
        graphic_printf(RED, BOLD, "*text_data uint64_t pointer in delete_extra_spaces\n");
        return ALLOCATION_ERROR;
    }

    size_t check_digit_index = start_digit_index + 1;
    while (text_data->text[check_digit_index] == ' '  ||
           text_data->text[check_digit_index] == '\'' ||
           isspace(text_data->text[check_digit_index])) {
           check_digit_index++;
    }
    text_data->LineData[line_pointer_index].lines_pointers      =         &(text_data->text[check_digit_index] );
    text_data->LineData[line_pointer_index].lines_lengths       = strlen( &(text_data->text[check_digit_index]));
    text_data->LineData[line_pointer_index].lines_ends_pointers =         &(text_data->text[check_digit_index +
                                                                            text_data->LineData[line_pointer_index].lines_lengths - 1]);

    return 0;
}

const char* error_data_enum(ERROR_DATA error_inf){
    switch(error_inf) {
        case PROGRAM_ERROR:    return "PROGRAM_ERROR";    break;
        case INPUT_ERROR:      return "INPUT_ERROR";      break;
        case MEMORY_ERROR:     return "MEMORY_ERROR";     break;
        case FILE_ERROR:       return "FILE_ERROR";       break;
        case ALLOCATION_ERROR: return "ALLOCATION_ERROR"; break;

        default:
            assert(0 && "assert in error_data_enum");
            break;
    }
}

int swap(void *value1, void *value2, size_t size) {
    assert(value1);
    assert(value2);
    assert(size);

    uint64_t* array_uint64_t1 = (uint64_t* ) value1;
    uint64_t* array_uint64_t2 = (uint64_t* ) value2;
    size_t uint64_bytes = size / sizeof(uint64_t);

    for (size_t index = 0; index < uint64_bytes; index++) {
        uint64_t buffer        = array_uint64_t1[index];
        array_uint64_t1[index] = array_uint64_t2[index];
        array_uint64_t2[index] = buffer;
    }

    // TODO use functional macros

    if ((size - uint64_bytes * sizeof(uint64_t)) / sizeof(int) == 1) {
        int* array_INT1 = (int* ) ((int* ) array_uint64_t1 + sizeof(uint64_t) * uint64_bytes);
        int* array_INT2 = (int* ) ((int* ) array_uint64_t2 + sizeof(uint64_t) * uint64_bytes);
        int buffer    = array_INT1[0];
        array_INT1[0] = array_INT2[0];
        array_INT2[0] = buffer;
    }

    if ((size - uint64_bytes * sizeof(uint64_t) - sizeof(int)) / sizeof(short int) == 1) {
        short int* array_SINT1 = (short int* ) ((short int* ) array_uint64_t1 + sizeof(uint64_t) * uint64_bytes + sizeof(int));
        short int* array_SINT2 = (short int* ) ((short int* ) array_uint64_t2 + sizeof(uint64_t) * uint64_bytes + sizeof(int));
        short int buffer = array_SINT1[0];
        array_SINT1[0]   = array_SINT2[0];
        array_SINT2[0]   = buffer;
    }

    if ((size - uint64_bytes * sizeof(uint64_t) - sizeof(int) - sizeof(short int)) / sizeof(char) == 1) {
        char* array_CHAR1 = (char* ) ((char* ) array_uint64_t1 + sizeof(uint64_t) * uint64_bytes + sizeof(int) + sizeof(short int));
        char* array_CHAR2 = (char* ) ((char* ) array_uint64_t2 + sizeof(uint64_t) * uint64_bytes + sizeof(int) + sizeof(short int));
        char buffer    = array_CHAR1[0];
        array_CHAR1[0] = array_CHAR2[0];
        array_CHAR2[0] = buffer;
    }

    return 0;
}
