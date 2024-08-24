#include "query_interface.h"
#include "query_decomposer.h"
#include "query_executor.h"
#include "rabbitmq_producer.h"

// Function to get seller details from the user
void gather_seller_details(mongoc_client_t *client) {
    char name[256], contact_info[512];
    printf("Enter seller name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline character

    printf("Enter seller contact info: ");
    fgets(contact_info, sizeof(contact_info), stdin);
    contact_info[strcspn(contact_info, "\n")] = 0;  // Remove newline character

    bson_t *doc = create_seller_document(name, contact_info);
    execute_insert_seller(client, doc);

    // Send a message to RabbitMQ
    char message[1024];
    snprintf(message, sizeof(message), "INSERT_SELLER|%s|%s", name, contact_info);
    send_message("seller_queue", message, 2);  // Priority can be adjusted

    bson_destroy(doc);
}

void gather_product_details(mongoc_client_t *client) {
    char name[256], description[512], category[256];
    double price;
    int num_sellers;

    // Handle sellers first
    printf("Enter the number of sellers: ");
    scanf("%d", &num_sellers);
    getchar();  // Consume the newline character

    for (int i = 0; i < num_sellers; i++) {
        gather_seller_details(client);
    }

    // Now gather product details
    printf("Enter product name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline character

    printf("Enter product description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;  // Remove newline character

    printf("Enter product price: ");
    scanf("%lf", &price);
    getchar();  // Consume the newline character

    printf("Enter product category: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;  // Remove newline character

    bson_t *product_doc = create_product_document(name, description, price, category);
    execute_insert_product(client, product_doc);

    // Send a message to RabbitMQ
    char message[2048];
    snprintf(message, sizeof(message), "INSERT_PRODUCT|%s|%s|%f|%s", name, description, price, category);
    send_message("product_queue", message, 1);  // Priority can be adjusted

    bson_destroy(product_doc);
}
