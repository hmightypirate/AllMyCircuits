sinclude ../rules.mk

ARFLAGS = rcs

INCDIR = include
SRCDIR = src
OBJDIR = bin
LIBDIR = lib

SRCFILES = $(wildcard $(SRCDIR)/*.c)
HDRFILES = $(wildcard $(SRCDIR)/*.h)
OBJFILES = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCFILES:.c=.o))
LIBFILES = $(patsubst $(SRCDIR)/%,$(LIBDIR)/lib%,$(SRCFILES:.c=.a))

# USE ALLMYLIBS
LDFLAGS += -L../../allmylibs/lib
CFLAGS += -I../../allmylibs/include
LDLIBS += -lcron -lbuzzer

all: libopencm3 libs

libs: bindir $(LIBFILES)

$(OBJFILES): $(SRCFILES) $(HDRFILES)
	$(CC) $(CFLAGS) -c -o $@ $(patsubst $(OBJDIR)/%,$(SRCDIR)/%,$(@:.o=.c))

$(LIBFILES): $(OBJFILES)
	$(AR) $(ARFLAGS) $@ $(patsubst $(LIBDIR)/lib%,$(OBJDIR)/%,$(@:.a=.o))

bindir:
	@mkdir -p $(OBJDIR) $(LIBDIR)

.PHONY: clean
clean:
	-rm -rf $(OBJDIR) $(LIBDIR)

mrproper: clean
	-$(MAKE) -C libopencm3 clean
