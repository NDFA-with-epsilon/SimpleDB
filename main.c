#include "InputBuffer.h"


int main() {
    InputBuffer* input_buf = new_input_buf();

    while (true)
    {
        print_promt();
        read_input(input_buf);

        if(input_buf->buffer[0] == '.') {
            switch (parse_meta_command(input_buf)) {
            case META_COMMAND_SUCCESS:
                /* code */
                continue;
            
            case META_COMMAND_UNRECOGNIZED_COMMAND:
                printf("\nUnrecognized command '%s'\n", input_buf->buffer);
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buf, &statement)) {
        case PREPARE_SUCCESS :
            /* code */
            continue;;
        
        case PREPARE_UNRECOGNIZED_STATEMENT:
            printf("\nUnrecognized keyword at the beginning of '%s'\n", input_buf->buffer);
            
            continue;
        }

        execute_statement(&statement);
    }    
}