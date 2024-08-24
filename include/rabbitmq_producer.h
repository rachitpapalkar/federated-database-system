#ifndef RABBITMQ_PRODUCER_H
#define RABBITMQ_PRODUCER_H

#include <amqp.h>
#include <amqp_tcp_socket.h>

void send_message(const char *queue_name, const char *message, int priority);
void declare_queue(amqp_connection_state_t conn, const char *queue_name);
#endif
