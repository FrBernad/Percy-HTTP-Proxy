#ifndef AWAIT_DOH_RESPONSE_H
#define AWAIT_DOH_RESPONSE_H

#include "utils/selector/selector.h"

void 
await_doh_response_on_arrival(unsigned state, struct selector_key *key);

unsigned
await_doh_response_on_read_ready(struct selector_key *key);

#endif