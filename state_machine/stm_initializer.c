#include "stm_initializer.h"

#include "../utils/connections_def.h"
#include "states/connected.h"
#include "states/parsing_host.h"
#include "states/send_request_line.h"
#include "states/try_connection_ip.h"
#include "stm.h"

//   unsigned (*on_read_ready) (struct selector_key *key);
//     /** ejecutado cuando hay datos disponibles para ser escritos */
//     unsigned (*on_write_ready)(struct selector_key *key);
//     /** ejecutado cuando hay una resolución de nombres lista */
//     unsigned (*on_block_ready)(struct selector_key *key);

static const struct state_definition connection_states[] = {
    {
        .state = PARSING_REQUEST_LINE,
        .on_arrival = parsing_host_on_arrival,
        .on_departure = NULL,
        .on_read_ready = parsing_host_on_read_ready,
        .on_write_ready = NULL,
        .on_block_ready = NULL
     },
    {
        .state = TRY_CONNECTION_IP,
        .on_arrival = try_connection_ip_on_arrival,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = try_connection_ip_on_write_ready,
        .on_block_ready = NULL},
    {
        .state = DOH_REQUEST,
        .on_arrival = NULL,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL
    },
    {
        .state = DOH_RESPONSE,
        .on_arrival = NULL,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL
    },
    {
        .state = TRY_CONNECTION_DOH_SERVER,
        .on_arrival = NULL,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL
    },
    {
        .state = DOH_RESOLVE_REQUEST_IPV4,
        .on_arrival = NULL,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL
    },
    {
        .state = DOH_RESOLVE_REQUEST_IPV6,
        .on_arrival = NULL,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL
    },
    {
        .state = CONNECTED,
        .on_arrival = connected_on_arrival,
        .on_departure = NULL,
        .on_read_ready = connected_on_read_ready,
        .on_write_ready = NULL
     },
    {
        .state = SEND_REQUEST_LINE,
        .on_arrival = send_request_line_on_arrival,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL},
    {
        .state = DONE,
        .on_arrival = NULL,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL
    },
    {
        .state = ERROR,
        .on_arrival = NULL,
        .on_departure = NULL,
        .on_read_ready = NULL,
        .on_write_ready = NULL
    }};

void init_connection_stm(struct state_machine *stm) {
    stm_init(stm, PARSING_REQUEST_LINE, ERROR, connection_states);
}