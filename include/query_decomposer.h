#ifndef QUERY_DECOMPOSER_H
#define QUERY_DECOMPOSER_H

#include <bson/bson.h>

// Function prototypes
bson_t* create_seller_document(const char* name, const char* contact_info);
bson_t* create_product_document(const char* name, const char* description, double price, const char* category);

#endif // QUERY_DECOMPOSER_H
