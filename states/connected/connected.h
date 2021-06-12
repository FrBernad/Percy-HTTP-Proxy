#ifndef CONNECTED_H
#define CONNECTED_H

#include "utils/selector/selector.h"

void 
connected_on_arrival(unsigned state, struct selector_key *key);

unsigned
connected_on_read_ready(struct selector_key *key);

unsigned
connected_on_write_ready(struct selector_key *key);

#endif
