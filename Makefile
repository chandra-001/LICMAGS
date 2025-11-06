.RECIPEPREFIX = >
CC = g++
LDFLAGS = -L/usr/local/lib/
CPPFLAGS = -I/usr/local/include/
#CC = clang++
#LDFLAGS = -L/opt/homebrew/lib/
#CPPFLAGS = -I/opt/homebrew/include/

CFLAGS = -std=c++11
GSLFLAGS = -lgsl -lgslcblas
SRC = ./src

_OBJS = $(patsubst $(SRC)/%.cc, %.o, $(wildcard $(SRC)/*.cc))
OBJDIR := objdir
OBJS := $(patsubst %, $(OBJDIR)/%, $(_OBJS))
OPT = -O2

vpath %.cc $(SRC)

lightcone: $(OBJS)
>$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OPT) $(GSLFLAGS) -fopenmp

$(OBJDIR)/main.o: main.cc
>$(CC) $< -c -o $@ $(CFLAGS) $(OPT) -fopenmp

$(OBJDIR)/read_coevals.o: read_coevals.cc
>$(CC) $< -c -o $@ $(CFLAGS) $(OPT)

$(OBJDIR)/cosmology.o: cosmology.cc
>$(CC) $< -c -o $@ $(CFLAGS) $(OPT) $(CPPFLAGS)

$(OBJDIR)/interpolate.o: interpolate.cc
>$(CC) $< -c -o $@ $(CFLAGS) $(OPT) $(CPPFLAGS)

$(OBJDIR)/write_lc.o: write_lc.cc
>$(CC) $< -c -o $@ $(CFLAGS) $(OPT) $(CPPFLAGS)

$(OBJS): | $(OBJDIR)
$(OBJDIR):
> mkdir $(OBJDIR)

.PHONY: clean
clean:
> -rm -r lightcone objdir/
