CORTEX_DIR	:= deps/cortex-vm
CORTEX_LIB	:= $(CORTEX_DIR)/lib/libcortex-vm.a
CORTEX_H	:= $(CORTEX_DIR)/lib

SRC_DIR		:= src
INC_DIR		:= include
BUILD_DIR	:= build
TARGET		:= rib-lang

$(CORTEX_LIB):
	git submodule update --init --recursive
	$(MAKE) -C $(CORTEX_DIR) lib

CFLAGS	+= -I$(CORTEX_H)
LDFLAGS	+= $(CORTEX_LIB) -lm

SRCS	:= $(wildcard $(SRC_DIR)/*.c) main.c
OBJS	:= $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) $(CFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	$(MAKE) -C $(CORTEX_DIR) clean
	rm -rf $(TARGET)

-include $(DEPS)
