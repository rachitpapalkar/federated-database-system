#include "rabbitmq_producer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void declare_queue(amqp_connection_state_t conn, const char *queue_name) {
    amqp_bytes_t queue;
    
    queue = amqp_cstring_bytes(queue_name);
    
    // Declare the queue
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn,
        1,  // Channel number
        queue,
        0,  // Passive (0 means don't check if queue exists)
        1,  // Durable (1 means queue will survive server restarts)
        0,  // Exclusive (0 means the queue is not exclusive to the connection)
        0,  // Auto-delete (0 means the queue will not automatically delete)
        amqp_empty_table  // Arguments
    );

    if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "[Producer/Consumer] Failed to declare queue\n");
        return;
    }

    printf("[Producer/Consumer] Queue '%s' declared successfully\n", queue_name);
}

void send_message(const char *queue_name, const char *message, int priority) {
    amqp_connection_state_t conn;
    amqp_socket_t *socket = NULL;
    amqp_bytes_t queue;
    amqp_bytes_t msg;

    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        fprintf(stderr, "Failed to create TCP socket\n");
        return;
    }

    if (amqp_socket_open(socket, "localhost", 5672)) {
        fprintf(stderr, "Failed to open TCP socket\n");
        amqp_destroy_connection(conn);
        return;
    }

    if (amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest").reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "Failed to login\n");
        amqp_destroy_connection(conn);
        return;
    }

    amqp_channel_open(conn, 1);
    if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "Failed to open channel\n");
        amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
        amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(conn);
        return;
    }

    // Declare the queue before sending messages
    declare_queue(conn, queue_name);

    queue = amqp_cstring_bytes(queue_name);
    msg = amqp_cstring_bytes(message);

    amqp_basic_properties_t props;
    memset(&props, 0, sizeof(props));  // Initialize properties structure to zero
    props._flags = AMQP_BASIC_PRIORITY_FLAG;
    props.priority = priority;

    int result = amqp_basic_publish(conn, 1, amqp_cstring_bytes(""), queue, 0, 0, &props, msg);
    if (result < 0) {
        fprintf(stderr, "Failed to publish message\n");
    } else {
        printf("Message sent to queue '%s' with priority %d\n", queue_name, priority);
    }

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}