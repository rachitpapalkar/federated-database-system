#ifndef POSTGRESQL_CONNECTOR_H
#define POSTGRESQL_CONNECTOR_H

#include <libpq-fe.h>  // PostgreSQL library

// Function prototypes
PGconn* connect_to_postgresql();
void disconnect_from_postgresql(PGconn *conn);
void apply_postgresql_schema(PGconn *conn);
PGresult* execute_postgresql_query(PGconn *conn, const char *query);

#endif // POSTGRESQL_CONNECTOR_H

