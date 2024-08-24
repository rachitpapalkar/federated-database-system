#ifndef QUERY_INTERFACE_H
#define QUERY_INTERFACE_H

#include <mongoc/mongoc.h>

// Function prototypes
void gather_seller_details(mongoc_client_t *client);
void gather_product_details(mongoc_client_t *client);

#endif // QUERY_INTERFACE_H
