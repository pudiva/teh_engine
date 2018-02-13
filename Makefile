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
	`pkg-config check --cflags` \
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
	`pkg-config check --libs` \
	`pkg-config libgvc --libs` \
	-lGLESv2 \
	$(LDFLAGS)

#
# target lists
#
SHADERS := \
	build/r_fragment_shader.glsl.c \
	build/r_vertex_shader.glsl.c

LIB_OBJS := \
	build/teh_model.o \
	build/vec.o \
	build/assets.o \
	build/teh_bsp.o \

GUI_OBJS := \
	build/window.o \
	build/r_fragment_shader.glsl.o \
	build/r_vertex_shader.glsl.o \
	build/renderer.o \
	build/controller.o \
	build/r_teh_model.o \

BSPC_OBJS := \
	build/tri.o \
	build/bspc.o \

CHECK_OBJS := \
	build/check/check.o \
	build/check/check_tri_split.o \
	build/check/check_bspc.o \

OBJS := \
	$(LIB_OBJS) \
	$(GUI_OBJS) \
	$(BSPC_OBJS) \
	$(CHECK_OBJS) \
	build/check/check.o \
	build/teh_bspc.o \
	build/teh_engine.o \

PRGS := \
	build/check/check \
	build/teh_bspc \
	build/teh_engine

ALL_FILES := \
	$(SHADERS) \
	$(OBJS) \
	$(PRGS) \

#
# recipes
#

# all and clean
.PHONY: all clean check
all: $(PRGS)

clean:
	rm -f $(ALL_FILES)

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
build/teh_bspc: build/teh_bspc.o $(BSPC_OBJS) $(LIB_OBJS)
build/teh_engine: build/teh_engine.o $(LIB_OBJS) $(GUI_OBJS)
build/check/check: build/check/check.o $(CHECK_OBJS) $(BSPC_OBJS) $(LIB_OBJS) $(GUI_OBJS)
	mkdir -p `dirname $@`
	$(LD) $(LDFLAGS) -o $@ $^
