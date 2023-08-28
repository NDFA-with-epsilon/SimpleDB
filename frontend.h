#include "common.h"

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

Table* new_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    table->num_rows = 0;
    
    for(int i = 0; i < MAX_PAGES; i++) {
        table->pages[i] = NULL;
    }

    return table;
}

void delete_table(Table* table) {
    for(int i = 0; i < MAX_PAGES; i++) {
        free(table->pages[i]);
    }

    free(table);
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

ExecuteResult execute_insert(Statement* statement, Table* table) {
    if(table->num_rows >= max_rows) {
        return EXECUTE_TABLE_FULL;
    }

    // Row* row = &(statement->row);
    serialize_row(&(statement->row), row_slot(table, table->num_rows));
    table->num_rows++;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table* table) {
    Row row;
    for(int i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }

    return EXECUTE_SUCCESS;
}


ExecuteResult execute_statement(Statement* statement, Table* table) {
    switch (statement->type) {
    case STATEMENT_INSERT :
       return execute_insert(statement, table);
    
    case STATEMENT_SELECT :
       return execute_select(statement, table);
    }
}