# SDL2-to-SDL1.2 Compatibility Shim Makefile

CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -O2 -fPIC
CFLAGS += $(shell sdl-config --cflags)

SRCDIR = src
INCDIR = include
OBJDIR = obj

SOURCES = $(SRCDIR)/sdl2_video.c \
          $(SRCDIR)/sdl2_render.c \
          $(SRCDIR)/sdl2_joystick.c \
          $(SRCDIR)/sdl2_surface.c \
          $(SRCDIR)/sdl2_filesystem.c \
          $(SRCDIR)/sdl2_messagebox.c \
          $(SRCDIR)/sdl2_keyboard.c \
          $(SRCDIR)/sdl2_audio.c

OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBRARY = libSDL2-compat.a

.PHONY: all clean

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(LIBRARY)

# For webOS cross-compilation
# Override CC, AR, and CFLAGS for the target platform
webos:
	$(MAKE) CC="/opt/PalmPDK/arm-toolchain/bin/arm-none-linux-gnueabi-gcc" \
	        AR="/opt/PalmPDK/arm-toolchain/bin/arm-none-linux-gnueabi-ar" \
	        CFLAGS="-Wall -Wextra -O2 -fPIC -march=armv7-a -mfpu=neon -D__WEBOS__ -I/opt/PalmPDK/include -I/opt/PalmPDK/include/SDL"
