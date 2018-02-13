#ifndef HAS_R_TEH_MODEL_H
#define HAS_R_TEH_MODEL_H

#include "teh_model.h"

void r_teh_model_load_vbo(struct teh_model* x);
void r_teh_model_load_texture(struct teh_model* x);
void r_teh_model_load_all(struct teh_model* x);
void r_teh_model_at_time(struct teh_model* x, unsigned long t);
void r_teh_model(struct teh_model* x, float w, int f1, int f2, int off, int n);
void r_teh_model_unload_texture(struct teh_model* x);
void r_teh_model_unload_vbo(struct teh_model* x);
void r_teh_model_unload_all(struct teh_model* x);

#endif
