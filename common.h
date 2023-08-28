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


typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_TOO_LONG,
    PREPARE_NEGATIVE_ID
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef struct InputBuffer {
    char* buffer;
    size_t buffer_len;
    ssize_t input_len;
} InputBuffer;
typedef struct Row {
    unsigned int id;
    char username[COL_UNAME_SIZE + 1];
    char email[COL_EMAIL_SIZE + 1];
} Row;

typedef struct Statement {
    StatementType type;
    Row row; //used by insert statement only
} Statement;

typedef struct Table {
    int num_rows;
    Pager* pager;
} Table;

typedef struct Pager {
    int fd;
    int file_len;
    void* pages[MAX_PAGES];
} Pager;

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

#endif