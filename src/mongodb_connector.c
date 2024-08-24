#include "mongodb_connector.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

mongoc_client_t* connect_to_mongodb() {
    mongoc_init();
    mongoc_client_t *client = mongoc_client_new("mongodb://localhost:27017");
    if (!client) {
        fprintf(stderr, "Failed to connect to MongoDB.\n");
        exit(1);
    }
    return client;
}

void disconnect_from_mongodb(mongoc_client_t *client) {
    mongoc_client_destroy(client);
    mongoc_cleanup();
}

bson_t* execute_mongodb_query(mongoc_client_t *client, const char *query) {
    // Example placeholder for MongoDB query execution
    // You should use MongoDB C Driver functions to execute the query
    bson_t *result = bson_new();
    return result;

}

#include "mongodb_connector.h"
#include "config.h"

void apply_mongodb_schema(mongoc_client_t *client) {
    bson_t command;
    bson_t validator;
    bson_t properties;
    bson_t seller_properties;
    bson_t items;
    bson_t required;
    bson_error_t error;

    // Create 'products' collection with schema validation
    bson_init(&command);
    bson_init(&validator);
    bson_init(&properties);
    bson_init(&seller_properties);
    bson_init(&items);
    bson_init(&required);

    BSON_APPEND_UTF8(&command, "create", "products");
    BSON_APPEND_UTF8(&validator, "bsonType", "object");

    // Required fields
    BSON_APPEND_UTF8(&required, "0", "name");
    BSON_APPEND_UTF8(&required, "1", "price");
    BSON_APPEND_UTF8(&required, "2", "sellers");
    BSON_APPEND_ARRAY(&validator, "required", &required);

    // Properties
    BSON_APPEND_DOCUMENT_BEGIN(&properties, "name", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "string");
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT_BEGIN(&properties, "description", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "string");
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT_BEGIN(&properties, "price", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "double");
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT_BEGIN(&properties, "category", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "string");
    bson_append_document_end(&properties, &seller_properties);

    // Sellers array
    BSON_APPEND_DOCUMENT_BEGIN(&seller_properties, "seller_id", &items);
    BSON_APPEND_UTF8(&items, "bsonType", "objectId");
    bson_append_document_end(&seller_properties, &items);

    BSON_APPEND_DOCUMENT_BEGIN(&seller_properties, "price", &items);
    BSON_APPEND_UTF8(&items, "bsonType", "double");
    bson_append_document_end(&seller_properties, &items);

    BSON_APPEND_DOCUMENT_BEGIN(&seller_properties, "stock", &items);
    BSON_APPEND_UTF8(&items, "bsonType", "int");
    bson_append_document_end(&seller_properties, &items);

    BSON_APPEND_DOCUMENT(&seller_properties, "properties", &seller_properties);
    BSON_APPEND_DOCUMENT(&properties, "sellers", &seller_properties);

    BSON_APPEND_DOCUMENT(&validator, "properties", &properties);

    // Attach validator to the command
    BSON_APPEND_DOCUMENT(&command, "validator", &validator);

    if (!mongoc_client_command_simple(client, "ecommerce", &command, NULL, NULL, &error)) {
        fprintf(stderr, "Failed to create 'products' collection: %s\n", error.message);
    }

    // Cleanup
    bson_destroy(&required);
    bson_destroy(&items);
    bson_destroy(&seller_properties);
    bson_destroy(&properties);
    bson_destroy(&validator);
    bson_destroy(&command);

    // Create 'reviews' collection with schema validation
    bson_init(&command);
    bson_init(&validator);
    bson_init(&properties);

    BSON_APPEND_UTF8(&command, "create", "reviews");
    BSON_APPEND_UTF8(&validator, "bsonType", "object");

    // Required fields
    BSON_APPEND_UTF8(&required, "0", "product_id");
    BSON_APPEND_UTF8(&required, "1", "rating");
    BSON_APPEND_UTF8(&required, "2", "review_date");
    BSON_APPEND_ARRAY(&validator, "required", &required);

    // Properties
    BSON_APPEND_DOCUMENT_BEGIN(&properties, "product_id", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "objectId");
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT_BEGIN(&properties, "user_id", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "objectId");
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT_BEGIN(&properties, "rating", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "int");
    BSON_APPEND_INT32(&seller_properties, "minimum", 1);
    BSON_APPEND_INT32(&seller_properties, "maximum", 5);
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT_BEGIN(&properties, "review_text", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "string");
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT_BEGIN(&properties, "review_date", &seller_properties);
    BSON_APPEND_UTF8(&seller_properties, "bsonType", "date");
    bson_append_document_end(&properties, &seller_properties);

    BSON_APPEND_DOCUMENT(&validator, "properties", &properties);

    // Attach validator to the command
    BSON_APPEND_DOCUMENT(&command, "validator", &validator);

    if (!mongoc_client_command_simple(client, "ecommerce", &command, NULL, NULL, &error)) {
        fprintf(stderr, "Failed to create 'reviews' collection: %s\n", error.message);
    }

    // Cleanup
    bson_destroy(&required);
    bson_destroy(&seller_properties);
    bson_destroy(&properties);
    bson_destroy(&validator);
    bson_destroy(&command);
}


