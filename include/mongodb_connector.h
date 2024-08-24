#ifndef MONGODB_CONNECTOR_H
#define MONGODB_CONNECTOR_H

#include <mongoc/mongoc.h> // MongoDB C Driver

// Function prototypes
mongoc_client_t* connect_to_mongodb();
void disconnect_from_mongodb(mongoc_client_t *client);
void apply_mongodb_schema(mongoc_client_t *client);
bson_t* execute_mongodb_query(mongoc_client_t *client, const char *query);

#endif // MONGODB_CONNECTOR_H
