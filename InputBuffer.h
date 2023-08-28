#ifndef _INPUT_BUFFER
#define _INPUT_BUFFER_

#include "common.h"

typedef struct InputBuffer {
    char* buffer;
    size_t buffer_len;
    ssize_t input_len;
} InputBuffer;

InputBuffer* new_input_buf() {
    InputBuffer* buf = (InputBuffer*)malloc(sizeof(InputBuffer));
    buf->buffer = NULL;
    buf->buffer_len = 0;
    buf->input_len = 0;

    return buf;
}

void close_input_buf(InputBuffer* buf) {
    free(buf->buffer);
    free(buf);
}

void read_input(InputBuffer* buf) {
    ssize_t bytes_read = getline(&(buf->buffer), &(buf->buffer_len), stdin);
    
    if(bytes_read <= 0) {
        printf("\nCouldn't read input\n");
        exit(EXIT_FAILURE);
    }

    buf->input_len = bytes_read - 1;
    buf->buffer[bytes_read - 1] = 0;
}   

#endif