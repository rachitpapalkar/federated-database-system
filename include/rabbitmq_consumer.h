#ifndef RABBITMQ_CONSUMER_H
#define RABBITMQ_CONSUMER_H

#include <amqp.h>
#include <amqp_tcp_socket.h>

void consume_messages(const char *queue_name);
//void declare_queue(amqp_connection_state_t conn, const char *queue_name);
void handle_commit();
#endif
