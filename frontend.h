#include "common.h"
#include <fcntl.h>

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

void print_promt() {
    printf("db > ");
}

void print_row(Row* row) {
    printf("\n(%d, %s, %s)\n", row->id, row->username, row->email);
}

Pager* pager_open(const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if(fd == -1) {
        printf("\nUnable to open file\n");
        exit(EXIT_FAILURE);
    }

    off_t file_len = lseek(fd, 0, SEEK_END);
    
    Pager* pager = (Pager*)malloc(sizeof(Pager));
    pager->fd = fd;
    pager->file_len = file_len;

    for(int i = 0; i < MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

void* get_page(Pager* pager, int page_num) {
    
}

Table* db_open(const char* filename) {
    Pager* pager = pager_open(filename);
    int num_rows = pager->file_len / row_size;

    Table* table = (Table*)malloc(sizeof(Table));
    table->num_rows = num_rows;
    table->pager = pager;

    return table;
}

void delete_table(Table* table) {
    for(int i = 0; i < MAX_PAGES; i++) {
        free(table->pages[i]);
    }

    free(table);
}

void* row_slot(Table* table, int row_num) {
    int page_num = row_num / rows_per_page;
    void* page = table->pages[page_num];

    if(!page) {
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }

    int row_offset = row_num % rows_per_page;
    int byte_offset = row_offset * row_size;

    return page + byte_offset;
}

MetaCommandResult parse_meta_command(InputBuffer* buf, Table* table) {
    if(strcmp(buf->buffer, ".exit") == 0) {
        close_input_buf(buf);
        delete_table(table);

        exit(EXIT_SUCCESS);
    }

    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement) {
    statement->type = STATEMENT_INSERT;

    char* keyword = strtok(input_buffer->buffer, " ");
    char* id_string = strtok(NULL, " ");
    char* username = strtok(NULL, " ");
    char* email = strtok(NULL, " ");

    if (id_string == NULL || username == NULL || email == NULL) {
    return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(id_string);
    if(id < 0) {
    return PREPARE_NEGATIVE_ID;
    }

    if (strlen(username) > COL_UNAME_SIZE) {
    return PREPARE_STRING_TOO_LONG;
    }

    if (strlen(email) > COL_EMAIL_SIZE) {
    return PREPARE_STRING_TOO_LONG;
    }

    statement->row.id = id;
    strcpy(statement->row.username, username);
    strcpy(statement->row.email, email);

    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer* buf, Statement* statement) {
    if(strncmp(buf->buffer, "insert", 6) == 0) {
        return prepare_insert(buf, statement);
    }

    if(strcmp(buf->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}