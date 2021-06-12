#include "sniffer_utils.h"

#include <stdio.h>
#include "logger/logger_utils.h"

#include "connections/connections_def.h"

void
sniff_data(struct selector_key *key) {

    proxyConnection *connection = ATTACHMENT(key);

    int dataOwner;
    buffer *bufferToSniff;

    if (key->fd == connection->client_fd) {
        dataOwner = CLIENT_OWNED;
        bufferToSniff = &connection->client_buffer;
    } else {
        dataOwner = ORIGIN_OWNED;
        bufferToSniff = &connection->origin_buffer;
    }

    size_t maxBytes;
    uint8_t *data = buffer_read_ptr(bufferToSniff, &maxBytes);
    unsigned state;

    for (int i = 0; i < connection->bytesToAnalize; i++) {
        state = sniffer_parser_feed(data[i], &connection->sniffer.sniffer_parser, dataOwner,
                                    connection->connectionRequest.port);
        if (state == sniff_done) {
            connection->sniffer.isDone = true;
            log_user_and_password(key);
            break;
        } else if (state == sniff_error) {
            connection->sniffer.isDone = true;
            break;
        }
    }

    connection->bytesToAnalize = 0;
}
