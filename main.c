#include "common.h"



int main() {
    InputBuffer* input_buf = new_input_buf();
    Table* table = new_table();
    
    while (true)
    {
        print_promt();
        read_input(input_buf);

        if(input_buf->buffer[0] == '.') {
            switch (parse_meta_command(input_buf, table)) {
            case META_COMMAND_SUCCESS:
                continue;
            
            case META_COMMAND_UNRECOGNIZED_COMMAND:
                printf("\nUnrecognized command '%s'\n", input_buf->buffer);
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buf, &statement)) {
        case PREPARE_SUCCESS :
            break;
        
        case PREPARE_SYNTAX_ERROR :
            printf("\nSyntax Error: couldn't parse statement\n");
            continue;

        case PREPARE_UNRECOGNIZED_STATEMENT:
            printf("\nUnrecognized keyword at the beginning of '%s'\n", input_buf->buffer);
            
            continue;
        }

        switch(execute_statement(&statement, table)) {
            case EXECUTE_SUCCESS :
                printf("\nExecuted\n");
                break;
            
            case EXECUTE_TABLE_FULL :
                printf("\nError: Table is full\n");
                break;
        }
    }    
}