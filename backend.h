#include "frontend.h"

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