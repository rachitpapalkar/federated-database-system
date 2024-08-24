#include "query_executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "postgresql_connector.h"

// Function to insert a seller document into MongoDB
void execute_insert_seller(mongoc_client_t *client, bson_t *doc) {
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "ecommerce", "sellers");
    bson_error_t error;

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "Error inserting seller: %s\n", error.message);
    } else {
        printf("Seller inserted successfully.\n");
    }

    mongoc_collection_destroy(collection);
}

// Function to insert a product document into MongoDB
void execute_insert_product(mongoc_client_t *client, bson_t *doc) {
    mongoc_collection_t *product_collection = mongoc_client_get_collection(client, "ecommerce", "products");
    bson_error_t error;

    if (!mongoc_collection_insert_one(product_collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "Error inserting product: %s\n", error.message);
    } else {
        printf("Product inserted successfully.\n");
    }

    mongoc_collection_destroy(product_collection);
}

// Function to execute a query on PostgreSQL
void execute_query(const char *query) {
    PGconn *pg_conn = connect_to_postgresql();
    if (pg_conn == NULL) {
        fprintf(stderr, "Failed to connect to PostgreSQL.\n");
        return;
    }

    // Check if the query is an INSERT_SELLER operation
    if (strncmp(query, "INSERT_SELLER|", 14) == 0) {
        char name[256];
        char contact_info[256];

        // Parse input
        sscanf(query, "INSERT_SELLER|%255[^|]|%255s", name, contact_info);

        // Construct the actual SQL query
        char sql_query[512];
        snprintf(sql_query, sizeof(sql_query), "INSERT INTO SELLERS (name, contact_info) VALUES ('%s', '%s');", name, contact_info);

        // Execute the constructed query
        PGresult *res = PQexec(pg_conn, sql_query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "PostgreSQL query execution failed: %s\n", PQerrorMessage(pg_conn));
        } else {
            printf("Query executed successfully: %s\n", sql_query);
        }

        PQclear(res);
    } 
    // Check if the query is an INSERT_PRODUCT operation
    else if (strncmp(query, "INSERT_PRODUCT|", 15) == 0) {
        char name[256], description[512], category[256];
        double price;

        // Parse input
        sscanf(query, "INSERT_PRODUCT|%255[^|]|%511[^|]|%lf|%255s", name, description, &price, category);

        // Construct the actual SQL query
        char sql_query[1024];
        snprintf(sql_query, sizeof(sql_query), "INSERT INTO PRODUCTS (name, description, price, category) VALUES ('%s', '%s', %f, '%s');", name, description, price, category);

        // Execute the constructed query
        PGresult *res = PQexec(pg_conn, sql_query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "PostgreSQL query execution failed: %s\n", PQerrorMessage(pg_conn));
        } else {
            printf("Query executed successfully: %s\n", sql_query);
        }

        PQclear(res);
    } 
    else {
        // Execute the original query as-is
        PGresult *res = PQexec(pg_conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "PostgreSQL query execution failed: %s\n", PQerrorMessage(pg_conn));
        } else {
            printf("Query executed successfully: %s\n", query);
        }

        PQclear(res);
    }

    disconnect_from_postgresql(pg_conn);
}



