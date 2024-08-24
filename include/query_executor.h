#ifndef QUERY_EXECUTOR_H
#define QUERY_EXECUTOR_H

#include <mongoc/mongoc.h>
#include <bson/bson.h>

// Function prototypes
void execute_insert_seller(mongoc_client_t *client, bson_t *doc);
void execute_insert_product(mongoc_client_t *client, bson_t *doc);

void execute_query(const char *query);

#endif // QUERY_EXECUTOR_H
