#ifndef HAS_WINDOW_H
#define HAS_WINDOW_H

#include <SDL.h>

extern const unsigned window_w, window_h;
extern SDL_Window* sdl_window;
extern SDL_GLContext sdl_gl_context;
extern unsigned long window_t1, window_t2, window_t3, window_dt;

int window_init();
void window_begin_frames();
void window_before_frame();
void window_after_frame();
void window_pause_frames();
void window_resume_frames();
void window_end_frames();
int window_fini();

#endif
