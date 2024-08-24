#include "rabbitmq_consumer.h"
#include "rabbitmq_producer.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "query_executor.h"

#define MAX_MESSAGES 100  // Adjust as needed
#define CONSUME_TIMEOUT 15 // Timeout in seconds

static char *message_queue[MAX_MESSAGES];
static int queue_size = 0;

/*void declare_queue(amqp_connection_state_t conn, const char *queue_name) {
    amqp_bytes_t queue = amqp_cstring_bytes(queue_name);

    // Declare the queue with consistent parameters
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn,
        1,  // Channel number
        queue,
        1,  // Passive (0 means the queue should be created if it does not exist)
        1,  // Durable (1 means the queue will survive server restarts)
        0,  // Exclusive (0 means the queue is not exclusive to the connection)
        0,  // Auto-delete (0 means the queue will not automatically delete)
        amqp_empty_table  // Arguments
    );

    if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "[Producer/Consumer] Failed to declare queue\n");
        return;
    }

    printf("[Producer/Consumer] Queue '%s' declared successfully\n", queue_name);
}*/



void consume_messages(const char *queue_name) {
    amqp_connection_state_t conn;
    amqp_socket_t *socket = NULL;
    amqp_bytes_t queue;
    struct timeval timeout;

    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        fprintf(stderr, "[Consumer] Failed to create TCP socket\n");
        return;
    }

    if (amqp_socket_open(socket, "localhost", 5672)) {
        fprintf(stderr, "[Consumer] Failed to open TCP socket\n");
        return;
    }

    if (amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest").reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "[Consumer] Failed to login\n");
        return;
    }

    amqp_channel_open(conn, 1);
    if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "[Consumer] Failed to open channel\n");
        return;
    }

    declare_queue(conn, queue_name);

    queue = amqp_cstring_bytes(queue_name);
    amqp_basic_consume(conn, 1, queue, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);

    printf("[Consumer] Waiting for messages on queue: %s\n", queue_name);

    timeout.tv_sec = CONSUME_TIMEOUT;
    timeout.tv_usec = 0;

    int messages_processed = 0;

    while (1) {
        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;

        amqp_maybe_release_buffers(conn);

        res = amqp_consume_message(conn, &envelope, &timeout, 0);

        if (res.reply_type == AMQP_RESPONSE_NORMAL && envelope.message.body.len > 0) {
            char *message = strndup((char *)envelope.message.body.bytes, envelope.message.body.len);
            printf("[Consumer] Received message: %s\n", message);

            if (queue_size < MAX_MESSAGES) {
                message_queue[queue_size++] = message;
                messages_processed++;
            } else {
                fprintf(stderr, "[Consumer] Message queue is full!\n");
                free(message);
            }

            amqp_basic_ack(conn, 1, envelope.delivery_tag, 0);
        } else if (res.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION && res.library_error == AMQP_STATUS_UNEXPECTED_STATE) {
            fprintf(stderr, "[Consumer] Error consuming message: %d\n", res.reply_type);
            break;
        } else {
            printf("[Consumer] No more messages or timeout, exiting...\n");
            break;
        }

        amqp_destroy_envelope(&envelope);
    }

    printf("[Consumer] Processed %d messages.\n", messages_processed);

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}




void handle_commit() {
    for (int i = 0; i < queue_size; i++) {
        execute_query(message_queue[i]);
        free(message_queue[i]);
    }
    queue_size = 0;  // Reset the queue after processing
}
