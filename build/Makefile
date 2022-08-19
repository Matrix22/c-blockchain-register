CFLAGS += -c -g -Wall -Wextra -Wpedantic \
                  -Wformat=2 -Wno-unused-parameter \
                  -Wshadow -Wwrite-strings -Wstrict-prototypes \
                  -Wold-style-definition -Wredundant-decls \
                  -Wnested-externs -Wmissing-include-dirs \
                  -Wjump-misses-init -Wlogical-op -O2

PATH_TO_FILES += ../src

FILES += $(PATH_TO_FILES)/blockdag.c $(PATH_TO_FILES)/check_valid_dag.c \
		 $(PATH_TO_FILES)/graph_sets.c $(PATH_TO_FILES)/graph.c \
		 $(PATH_TO_FILES)/lists.c $(PATH_TO_FILES)/ordered_lists.c

.PHONY: build blockdag_obj clean

build: blockdag_obj
	@gcc *.o -o blockdag
	@rm -rf *.o

blockdag_obj: $(FILES)
	@gcc $(CFLAGS) $(FILES)

clean:
	@rm -rf blockdag blockdag.in blockdag.out log_valgrind.txt