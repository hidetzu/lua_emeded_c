MAKEFLAGS += -rR --include-dir=$(CURDIR)
MAKEFLAGS += --no-print-directory

export CC = gcc
export AT = @
export MKDIR  = mkdir -p
export RM     = rm -rf

########################################
# User setting
USE_DEBUG=yes

TOP_DIR=.
OUTPUT_DIR = work
BIN_DIR = $(OUTPUT_DIR)/bin
LIB_DIR = $(OUTPUT_DIR)/lib
TARGET=sample

########################################
# helpers
source-dir-to-binary-dir = $(addprefix $(OUTPUT_DIR)/, $1)
source-to-object = $(call source-dir-to-binary-dir, \
	$(subst .c,.o,$(filter %.c,$1))) \


SRCS-lua=$(shell find lua -name "*.c")
SRCS-core=$(shell find  src -name "*.c")

SRCS+=$(SRCS-core) $(SRCS-lua)
INCLUDE=-I./include -I.
CFLAGS= -Wall -g
LIB-DIR = -L$(LIB_DIR)
LIB-DIR = 
LDFLAGS = -llua

objs=$(call source-to-object, $(SRCS))


.PHONY:all clean

all:$(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET):$(objs)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LIB-DIR) $(LDFLAGS)


$(OUTPUT_DIR)/%.o:%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	$(AT)$(RM) $(OUTPUT_DIR) $(TARGET)


############
# utils

tags:
	$(RM) tags
	@find . -name "*.[ch]" | xargs ctags


