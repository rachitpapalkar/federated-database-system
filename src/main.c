#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include "query_interface.h"
#include "query_decomposer.h"
#include "query_executor.h"
#include "result_aggregator.h"
#include "config.h"
#include "error_handling.h"
#include "postgresql_connector.h"
#include "mongodb_connector.h"
#include "rabbitmq_consumer.h"
#include "semaphores.h"

sem_t seller_sem;
sem_t product_sem;
sem_t commit_sem;

void* seller_consumer_thread(void* arg) {
    while (1) {
        printf("[Seller Thread] Waiting on seller_sem...\n");
        sem_wait(&seller_sem);
        printf("[Seller Thread] seller_sem acquired, consuming messages...\n");
        consume_messages("seller_queue");
        
        // Wait for the commit signal
        printf("[Seller Thread] Waiting on commit_sem...\n");
        sem_wait(&commit_sem);
        printf("[Seller Thread] commit_sem acquired, committing seller messages...\n");
        handle_commit();  // Process and commit seller messages
        
        printf("[Seller Thread] Signaling product_sem...\n");
        sem_post(&product_sem);  // Allow product queue to proceed
    }
    return NULL;
}

void* product_consumer_thread(void* arg) {
    while (1) {
        printf("[Product Thread] Waiting on product_sem...\n");
        sem_wait(&product_sem);
        printf("[Product Thread] product_sem acquired, consuming messages...\n");
        consume_messages("product_queue");

        // Wait for the commit signal
        printf("[Product Thread] Waiting on commit_sem...\n");
        sem_wait(&commit_sem);
        printf("[Product Thread] commit_sem acquired, committing product messages...\n");
        handle_commit();  // Process and commit product messages

        printf("[Product Thread] Signaling seller_sem...\n");
        sem_post(&seller_sem);  // Reset to allow the next seller processing
    }
    return NULL;
}

int main() {
    pthread_t seller_thread, product_thread;

    // Initialize semaphores
    sem_init(&seller_sem, 0, 0);    // Start with seller semaphore available
    sem_init(&product_sem, 0, 0);   // Product semaphore initially unavailable
    sem_init(&commit_sem, 0, 0);    // Commit semaphore initially unavailable

    // Create consumer threads
    if (pthread_create(&seller_thread, NULL, seller_consumer_thread, NULL)) {
        fprintf(stderr, "Error creating seller consumer thread\n");
        return EXIT_FAILURE;
    }
    if (pthread_create(&product_thread, NULL, product_consumer_thread, NULL)) {
        fprintf(stderr, "Error creating product consumer thread\n");
        return EXIT_FAILURE;
    }

    // Main application logic
    int choice;
    PGconn *pg_conn = connect_to_postgresql();
    mongoc_client_t *mongo_client = connect_to_mongodb();

    if (!pg_conn || !mongo_client) {
        fprintf(stderr, "Failed to connect to databases.\n");
        return EXIT_FAILURE;
    }

    while (1) {
        printf("\nMenu:\n");
        printf("1. Insert Product\n");
        printf("2. Insert Seller\n");
        printf("3. Commit Changes\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume the newline character

        switch (choice) {
            case 1:
                printf("[Main] Inserting product...\n");
                gather_product_details(mongo_client);
                break;
            case 2:
                printf("[Main] Inserting seller...\n");
                gather_seller_details(mongo_client);
                
                break;
            case 3:
                printf("[Main] Committing changes, signaling commit_sem...\n");
                // Signal that the commit operation can proceed
                
                sem_post(&commit_sem);
                sem_post(&seller_sem);
                break;
            case 4:
                printf("[Main] Exiting...\n");
                disconnect_from_postgresql(pg_conn);
                disconnect_from_mongodb(mongo_client);
                pthread_cancel(seller_thread);
                pthread_cancel(product_thread);
                pthread_join(seller_thread, NULL);
                pthread_join(product_thread, NULL);
                sem_destroy(&seller_sem);
                sem_destroy(&product_sem);
                sem_destroy(&commit_sem);
                return EXIT_SUCCESS;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
