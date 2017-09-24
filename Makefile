#
# TEH_ENGINE MAKEFIEL!!1!11ONE!
#

#
# configs
#

# compiler
CC := gcc
CFLAGS := \
	-Isauce \
	`pkg-config sdl2 --cflags` \
	`pkg-config SDL2_image --cflags` \
	`pkg-config SDL2_mixer --cflags` \
	-std=c99 \
	-Wall \
	-pipe \
	-ggdb \
	-DOPENGL_ES \
	$(CFLAGS)

# linker
LD := gcc
LDFLAGS := \
	-lm \
	`pkg-config sdl2 --libs` \
	`pkg-config SDL2_image --libs` \
	`pkg-config SDL2_mixer --libs` \
	-lGLESv2 \
	$(LDFLAGS)

# ragel
RAGEL := ragel

RLFLAGS := \


#
# target lists
#
SHADERS := \
	build/r_fragment_shader.glsl.c \
	build/r_vertex_shader.glsl.c \

STATE_MACHINES := \
	build/model.rl.c

OBJS := \
	build/window.o \
	build/renderer.o \
	build/controller.o \
	build/teh_model.o \
	build/teh_engine.o \

PRGS := \
	build/teh_engine


#
# recipes
#

# all and clean
.PHONY: all clean
all: $(PRGS)

clean:
	rm -f $(SHADERS) $(OBJS) $(PRGS)

# shaders
build/%.glsl.c: sauce/%.glsl
	mkdir -p `dirname $@`
	cp $< .
	xxd -i `basename $<` $@
	rm `basename $<`

# state machines
build/%.rl.c: sauce/%.rl
	mkdir -p `dirname $@`
	$(RAGEL) $(RLFLAGS) -o $@ $<

# objs
build/renderer.o: $(SHADERS)
build/%.o: sauce/%.c
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -o $@ -c $<

# prgs
build/teh_engine: $(OBJS) $(SHADERS)
$(PRGS):
	mkdir -p `dirname $@`
	$(LD) $(LDFLAGS) -o $@ $^
