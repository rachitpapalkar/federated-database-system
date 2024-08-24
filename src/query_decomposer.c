#include "query_decomposer.h"

// Function to create BSON document for a seller
bson_t* create_seller_document(const char* name, const char* contact_info) {
    bson_t *doc = bson_new();
    BSON_APPEND_UTF8(doc, "name", name);
    BSON_APPEND_UTF8(doc, "contact_info", contact_info);
    return doc;
}

// Function to create BSON document for a product
bson_t* create_product_document(const char* name, const char* description, double price, const char* category) {
    bson_t *doc = bson_new();
    BSON_APPEND_UTF8(doc, "name", name);
    BSON_APPEND_UTF8(doc, "description", description);
    BSON_APPEND_DOUBLE(doc, "price", price);
    BSON_APPEND_UTF8(doc, "category", category);
    return doc;
}
