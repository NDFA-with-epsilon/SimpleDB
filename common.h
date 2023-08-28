#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct InputBuffer {
    char* buffer;
    size_t buffer_len;
    ssize_t input_len;
} InputBuffer;


typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct Statement {
    StatementType type;
} Statement;

void print_promt() {
    printf("db > ");
}

MetaCommandResult parse_meta_command(InputBuffer* buf) {
    if(strcmp(buf->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    }

    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* buf, Statement* statement) {
    if(strncmp(buf->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }

    if(strcmp(buf->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
    switch (statement->type) {
    case STATEMENT_INSERT :
        /* code */
        printf("\ninsert\n");
        break;
    
    case STATEMENT_SELECT :
        printf("\nselect\n");
        break;
    }
}

#endif