ifeq ($(BUILD),release)
	CFLAGS += -O3 -s -DNDEBUG
else
	CFLAGS += -O0 -g
endif

TARGET    := a.out

PC_DEPS   := gl glfw3
PC_CFLAGS := $(shell pkg-config --cflags $(PC_DEPS))
PC_LIBS   := $(shell pkg-config --libs $(PC_DEPS))

SRCS      := $(shell find src -name *.c)
OBJS      := $(SRCS:%=build/%.o)
DEPS      := $(OBJS:.o=.d)

INCS      := -I./include #$(addprefix -I,$(shell find ./include -type d))

CFLAGS    += $(PC_CFLAGS) $(INCS) -MMD -MP
LDLIBS    += $(PC_LIBS) -lm

build/$(TARGET): $(OBJS)
	$(CC) -g $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

build/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean syntastic
clean:
	rm -f build/$(TARGET) $(OBJS) $(DEPS)

syntastic:
	echo $(CFLAGS) | tr ' ' '\n' > .syntastic_c_config

release:
	-$(MAKE) "BUILD=release"

-include $(DEPS)
