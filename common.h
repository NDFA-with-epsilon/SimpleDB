#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define COL_UNAME_SIZE 32
#define COL_EMAIL_SIZE 255
#define MAX_PAGES 100

#define SIZE_OF_ATTRIBUTE(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const int id_size = SIZE_OF_ATTRIBUTE(Row, id);
const int uname_size = SIZE_OF_ATTRIBUTE(Row, username);
const int email_size = SIZE_OF_ATTRIBUTE(Row, email);

const int id_offset = 0;
const int uname_offset = id_offset + id_size;
const int email_offset = uname_offset + uname_size;
const int row_size = id_size + uname_size + email_size;

const int PAGE_SIZE = 4096;
const int rows_per_page = PAGE_SIZE / row_size;
const int max_rows = rows_per_page * MAX_PAGES;

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
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct Row {
    unsigned int id;
    char username[COL_UNAME_SIZE];
    char email[COL_EMAIL_SIZE];
} Row;

typedef struct Statement {
    StatementType type;
    Row row; //used by insert statement only
} Statement;

typedef struct Table {
    int num_rows;
    void* pages[MAX_PAGES];
} Table;

void print_promt() {
    printf("db > ");
}

void print_row(Row* row) {
    printf("\n(%d, %s, %s)\n", row->id, row->username, row->email);
}

void serialize_row(Row* src, void* dst) {
    memcpy(dst + id_offset, &(src->id), id_size);
    memcpy(dst+ uname_offset, &(src->username), uname_size);
    memcpy(dst+ email_offset, &(src->email), email_size);
}

void deserialize_row(void* src, Row* dst) {
    memcpy(&(dst->id), src + id_offset, id_size);
    memcpy(&(dst->username), src + uname_offset, uname_size);
    memcpy(&(dst->email), src + email_offset, email_size);
}

void* row_slot(Table* table, int row_num) {
    
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

        int args = sscanf(buf->buffer, "insert %d %s %s", &(statement->row.id), statement->row.username, statement->row.email);

        if(args < 3) return PREPARE_SYNTAX_ERROR;
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
        printf("\ninsert\n");
        break;
    
    case STATEMENT_SELECT :
        printf("\nselect\n");
        break;
    }
}

#endif