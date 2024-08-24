#include "config.h"
#include <stdio.h>

// Load configuration from a file or environment variables if needed
void load_config() {
    // For simplicity, we're using hardcoded values.
    // In a real application, you might read from a config file or environment variables.
    // Example:
    // POSTGRESQL_HOST = getenv("POSTGRESQL_HOST") ? getenv("POSTGRESQL_HOST") : POSTGRESQL_HOST;
    // POSTGRESQL_PORT = getenv("POSTGRESQL_PORT") ? atoi(getenv("POSTGRESQL_PORT")) : POSTGRESQL_PORT;
    // And so on...
}

// Print configuration for debugging purposes
void print_config() {
    printf("PostgreSQL Configuration:\n");
    printf("Host: %s\n", POSTGRESQL_HOST);
    printf("Port: %d\n", POSTGRESQL_PORT);
    printf("Database Name: %s\n", POSTGRESQL_DBNAME);
    printf("User: %s\n", POSTGRESQL_USER);
    printf("Password: %s\n", POSTGRESQL_PASSWORD); // Be cautious with printing passwords

    printf("MongoDB Configuration:\n");
    printf("URI: %s\n", MONGODB_URI);
    printf("Database Name: %s\n", MONGODB_DBNAME);
}

// Print the PostgreSQL schema
void print_postgresql_schema() {
    printf("PostgreSQL Schema:\n");
    printf("Products Table:\n%s\n", POSTGRESQL_SCHEMA_PRODUCTS);
    printf("Sellers Table:\n%s\n", POSTGRESQL_SCHEMA_SELLERS);
    printf("Product Sellers Table:\n%s\n", POSTGRESQL_SCHEMA_PRODUCT_SELLERS);
    printf("Reviews Table:\n%s\n", POSTGRESQL_SCHEMA_REVIEWS);
}

// Print the MongoDB schema
void print_mongodb_schema() {
    printf("MongoDB Schema:\n");
    printf("Products Collection:\n%s\n", MONGODB_SCHEMA_PRODUCTS);
    printf("Reviews Collection:\n%s\n", MONGODB_SCHEMA_REVIEWS);
}
