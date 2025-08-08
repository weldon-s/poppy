#ifndef CHUNK_H
#define CHUNK_H

#include <stdbool.h>

#include "codegen/register.h"

struct chunk;

struct chunk *new_chunk();
void free_chunk(struct chunk *chunk);

void add_variable(struct chunk *chunk, char *var);
bool has_variable(struct chunk *chunk, char *var);

char *push_chunk(struct chunk *chunk);
char *pop_chunk(struct chunk *chunk);

char *read_variable(struct chunk *chunk, enum reg into, char *var, enum reg chunk_address);
char *write_variable(struct chunk *chunk, char *var, enum reg from, enum reg chunk_address);

#endif