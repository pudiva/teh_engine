#ifndef HAS_ASSETS_H
#define HAS_ASSETS_H

#include <SDL_image.h>
#include "teh_model.h"

struct teh_model* teh_model_get(const char* path);
struct SDL_Surface* image_get(const char* path);

#endif
