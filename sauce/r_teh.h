#ifndef HAS_R_TEH_H
#define HAS_R_TEH_H

#include "teh.h"

void r_teh_load_vbo(struct teh* x);
void r_teh_load_texture(struct teh* x);
void r_teh_load_all(struct teh* x);
void r_teh_at_time(struct teh* x, unsigned long t);
void r_teh(struct teh* x, float w, int f1, int f2, int off, int n);
void r_teh_unload_texture(struct teh* x);
void r_teh_unload_vbo(struct teh* x);
void r_teh_unload_all(struct teh* x);

#endif
