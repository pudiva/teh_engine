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
	build/r_vertex_shader.glsl.c

STATE_MACHINES := \
	build/model.rl.c

OBJS := \
	build/window.o \
	build/r_fragment_shader.glsl.o \
	build/r_vertex_shader.glsl.o \
	build/renderer.o \
	build/controller.o \
	build/teh_model.o \
	build/vec.o

BSPC_OBJS := \
	build/teh_bspc_lib.o

PRG_OBJS := \
	build/check/check.o \
	build/teh_bspc.o \
	build/teh_engine.o \

PRGS := \
	build/check/check \
	build/teh_bspc \
	build/teh_engine

ALL_FILES := \
	$(SHADERS) \
	$(STATE_MACHINES) \
	$(OBJS) \
	$(PRGS)

#
# recipes
#

# all and clean
.PHONY: all clean check
all: $(PRGS)

clean:
	rm -f $(SHADERS) $(OBJS) $(PRG_OBJS) $(PRGS)

check: build/check/check
	$^

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
build/%.o: sauce/%.c
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -o $@ -c $<

# prgs
build/teh_bspc: build/teh_bspc.o build/teh_bspc_lib.o $(OBJS)
build/teh_engine: build/teh_engine.o $(OBJS)
	mkdir -p `dirname $@`
	$(LD) $(LDFLAGS) -o $@ $^

# checks
build/check/check: build/check/check.o build/teh_bspc_lib.o $(OBJS)
	mkdir -p `dirname $@`
	$(LD) $(LDFLAGS) -lcheck -o $@ $^
