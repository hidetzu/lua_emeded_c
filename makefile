
CC      = gcc
CFLAGS  = -Wall -g
LDFLAGS =
LIBS    = -llua

INCLUDE = -I./src/include
OBJDIR=./objs

SERVER=server
CLINET=clinet

SERVER_SRCS = $(shell find src/server -name "*.c")
CLINET_SRCS = $(shell find src/client -name "*.c")


all:$(OBJDIR) $(SERVER) $(CLINET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(SERVER):$(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(SERVER_SRCS)))
	$(CC) -o $@ $^ $(LDFLAGS)

$(CLINET):$(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(CLINET_SRCS)))
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)


$(OBJDIR)/%.o : %.c
	@mkdir -p $(dir $@); \
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<


clean:
	@rm -rf $(OBJDIR) $(SERVER) $(CLINET)
