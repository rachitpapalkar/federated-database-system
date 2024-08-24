#include "postgresql_connector.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

PGconn* connect_to_postgresql() {
    const char *conninfo = "host=localhost port=5432 dbname=ecommerce user=rachit password=rachit";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to PostgreSQL failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    return conn;
}

void disconnect_from_postgresql(PGconn *conn) {
    PQfinish(conn);
}

PGresult* execute_postgresql_query(PGconn *conn, const char *query) {
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit(1);
    }
    return res;
}

void apply_postgresql_schema(PGconn *conn) {
    PGresult *res;

    // Apply PostgreSQL schemas from config.h
    res = PQexec(conn, POSTGRESQL_SCHEMA_PRODUCTS);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Failed to create products table: %s", PQerrorMessage(conn));
    }
    PQclear(res);

    res = PQexec(conn, POSTGRESQL_SCHEMA_SELLERS);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Failed to create sellers table: %s", PQerrorMessage(conn));
    }
    PQclear(res);

    res = PQexec(conn, POSTGRESQL_SCHEMA_PRODUCT_SELLERS);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Failed to create product_sellers table: %s", PQerrorMessage(conn));
    }
    PQclear(res);

    res = PQexec(conn, POSTGRESQL_SCHEMA_REVIEWS);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Failed to create reviews table: %s", PQerrorMessage(conn));
    }
    PQclear(res);
}
