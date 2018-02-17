#ifndef HAS_ASSETS_H
#define HAS_ASSETS_H

#include <SDL_image.h>
#include "teh.h"
#include "beh.h"

struct teh* teh_get(const char* path);
struct beh* beh_get(const char* path);
struct SDL_Surface* image_get(const char* path);

#endif
