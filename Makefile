OBJ := main.o codify.o utilities.o gui.o mode.o file.o
CPPFLAGS := -I. -g
INCLUDES := prefix.h
.INCLUDE_DIRS = .


encode: -lmenu -lform -lpanel -lncursesw -lpthread $(OBJ) 
	gcc $^ -o $@  -g
	./$@
	rm -f $(OBJ) $@
	clear	

$(OBJ): $(INCLUDES)


.PHONY: clean
clean:
	rm -f encode $(OBJ)

