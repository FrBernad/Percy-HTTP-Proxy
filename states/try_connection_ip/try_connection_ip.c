#include "try_connection_ip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connections/connections.h"
#include "connections/connections_def.h"
#include "metrics/metrics.h"
#include "utils/doh/doh_utils.h"
#include "utils/net/net_utils.h"

static int
check_origin_connection(int socketfd);

void try_connection_ip_on_arrival(const unsigned state, struct selector_key *key) {
    proxyConnection *connection = ATTACHMENT(key);

    selector_set_interest(key->s, connection->client_fd, OP_NOOP);
    selector_set_interest(key->s, connection->origin_fd, OP_WRITE);
}

unsigned
try_connection_ip_on_write_ready(struct selector_key *key) {
    proxyConnection *connection = ATTACHMENT(key);

    if (check_origin_connection(connection->origin_fd)) {
        /*Si dohConnection == NULL es que nunca mande el doh_request, si no esta active estoy probando ipv+ */
        if (connection->connectionRequest.host_type == domain &&
            (connection->dohConnection == NULL || !connection->dohConnection->isActive)) {
            if (connection->connectionRequest.connect) {
                buffer_reset(&connection->client_buffer);
            }
            return SEND_DOH_REQUEST;
        }
        /*En este caso ya estoy conectado al origin*/
        if (connection->connectionRequest.connect) {
            buffer_reset(&connection->client_buffer);
            register_new_connection();
            return CONNECTED;
        }
        register_new_connection();
        return SEND_REQUEST_LINE;
    }

    // try next ip from doh response
    if (connection->connectionRequest.host_type == domain && connection->dohConnection != NULL) {
        return try_next_dns_connection(key);
    }

    printf("ERROR AL CONECTARSE A LA IP O AL DOH SERVER");
    connection->error = INTERNAL_SERVER_ERROR;
    return ERROR;
}

static int
check_origin_connection(int socketfd) {
    int opt;
    socklen_t optlen = sizeof(opt);
    if (getsockopt(socketfd, SOL_SOCKET, SO_ERROR, &opt, &optlen) < 0) {
        printf("Error getsockopt.\n\n");
        return false;
    }

    if (opt != 0) {
        printf("Connection could not be established, closing sockets.\n\n");
        return false;
    }

    return true;
}
