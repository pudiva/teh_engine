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
	`pkg-config libgvc --cflags` \
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
	build/vec.o \
	build/assets.o \
	build/teh_bsp.o \

BSPC_OBJS := \
	build/tri.o \
	build/bspc.o \

PRG_OBJS := \
	build/check/check.o \
	build/teh_bspc.o \
	build/teh_engine.o \

PRGS := \
	build/check/check \
	build/teh_bspc \
	build/teh_engine

CHECK_OBJS := \
	build/check/check.o \
	build/check/check_tri_split.o \
	build/check/check_bspc.o \

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
	rm -f $(SHADERS) $(OBJS) $(BSPC_OBJS) $(PRG_OBJS) $(PRGS)

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
# FIXME: bspc ta dependendo de tudo
build/teh_bspc: build/teh_bspc.o $(BSPC_OBJS) $(OBJS)
	mkdir -p `dirname $@`
	$(LD) $(LDFLAGS) `pkg-config --libs libgvc` -o $@ $^

build/teh_engine: build/teh_engine.o $(OBJS)
	mkdir -p `dirname $@`
	$(LD) $(LDFLAGS) -o $@ $^

# checks
build/check/check: $(CHECK_OBJS) $(BSPC_OBJS) $(OBJS)
	mkdir -p `dirname $@`
	$(LD) $(LDFLAGS) -lcheck -o $@ $^
