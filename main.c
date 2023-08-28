#include "InputBuffer.h"


int main() {
    InputBuffer* input_buf = new_input_buf();

    while (true)
    {
        print_promt();
        read_input(input_buf);

        if(strcmp(input_buf->buffer, ".exit") == 0) {
            close_input_buf(input_buf);
            exit(EXIT_SUCCESS);
        }
        
        else {
            printf("Unrecognized command '%s'\n", input_buf->buffer);
        }
    }    
}