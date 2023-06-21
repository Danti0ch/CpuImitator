SRC := $(realpath src/$(target))

BUILD := build
BIN := bin

CXX := g++

SRC_SUBDIRS := $(shell find $(SRC) -type d)

SRC_FILES := $(wildcard $(addsuffix /*.cpp, $(SRC_SUBDIRS)))
OBJ_FILES := $(addprefix $(BUILD)/, $(patsubst %.cpp, %.o, $(notdir $(SRC_FILES))))
vpath %.cpp $(SRC_SUBDIRS)

DEP_FILES := $(patsubst %.o, %.d, $(OBJ_FILES))

INCDIRS := $(addprefix -I, $(SRC_SUBDIRS))
CFLAGS = $(INCDIRS) -std=c++17 #-Wall -Wextra -Weffc++ -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Waggressive-loop-optimizations -Walloc-zero -Walloca -Walloca-larger-than=8192 -Warray-bounds -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wdangling-else -Wduplicated-branches -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Winline -Wlarger-than=8192 -Wvla-larger-than=8192 -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wstringop-overflow=4 -Wsuggest-attribute=noreturn -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs -Waligned-new -Walloc-size-larger-than=1073741824 -Walloc-zero -Walloca -Walloca-larger-than=8192 -Wcast-align -Wdangling-else -Wduplicated-branches -Wformat-overflow=2 -Wformat-truncation=2 -Wmissing-attributes -Wmultistatement-macros -Wrestrict -Wshadow=global -Wsuggest-attribute=malloc -fcheck-new -fsized-deallocation -fstack-check -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer
LFLAGS := -export-dynamic -no-pie 

main: $(OBJ_FILES) #$(PLUGINS_FILES)
	$(CXX) -o $(BIN)/$(target).exe $^ $(LFLAGS)

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BUILD)/%.d: %.cpp
	$(CXX) $(INCDIRS) -MM -MT $(patsubst %.d, %.o, $@) $< > $@  

main-gdb: add_debug_flag main

add_debug_flag:
	$(eval CFLAGS += -g)

-include $(DEP_FILES)
.PHONY: clear

clear:
	rm $(OBJ_FILES) $(DEP_FILES)BUILD := build