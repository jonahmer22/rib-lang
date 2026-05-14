SRC_DIR		:= src
INC_DIR		:= include
BUILD_DIR	:= build
TARGET		:= rib

CFLAGS		+= -I$(INC_DIR)
LDFLAGS		+= -lcortex-vm -lm

SRCS		:= $(wildcard $(SRC_DIR)/*.c) main.c
OBJS		:= $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS		:= $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

-include $(DEPS)
