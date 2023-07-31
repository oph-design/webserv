NAME			=	webserv

CC				=	c++
CFLAGS			=	-Wall -Wextra -Werror -g
LFLAGS			=
LCFLAGS			=

################################################################################
################################################################################

SRC					=	$(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_DIR				=	src/
SRC_FILES			=	main.c

PLACEHOLDER			=	$(addprefix $(PLACEHOLDER_DIR), $(PLACEHOLDER_FILES))
PLACEHOLDER_DIR		=	src/placeholder/
PLACEHOLDER_FILES	=	placeholder.cpp

ALL_SRC				=	$(SRC) $(PLACEHOLDER)

################################################################################
################################################################################

OBJ_DIR			=	obj/
ALL_OBJ			=	$(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(ALL_SRC))
ALL_OBJ_DIR		=	$(sort $(dir $(ALL_OBJ)))

################################################################################
################################################################################

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(CC) $(LCFLAGS) -c $< -o $@ $(CFLAGS)
	@echo $(LGREEN)"compiled "$^$(DEFAULT)

$(NAME): $(ALL_OBJ_DIR) $(ALL_OBJ)
	@$(CC) $(ALL_OBJ) -o $(NAME) $(LFLAGS) $(LCFLAGS)
	@echo $(GREEN)" linked "$@$(DEFAULT)

clean:
	@echo $(RED)"cleaning:"
	@rm -rfv $(patsubst %/,%,$(OBJ_DIR))
	@echo "cleaned"$(DEFAULT)

fclean:
	@echo $(RED)"fcleaning:"
	@rm -rfv $(patsubst %/,%,$(OBJ_DIR)) $(NAME)
	@echo "fcleaned"$(DEFAULT)

re:	fclean all

bonus:
	@echo "make sure to implement bonus"

################################################################################
################################################################################

norm:
	@echo "implement linter here"

################################################################################
################################################################################

LSAN			=	lib/LeakSanitizer
LSANLIB			=	$(LSAN)/liblsan.a

ifeq ($(OS),Linux)
	LSANLFLAGS := -rdynamic -Llib/LeakSanitizer -llsan -ldl -lstdc++
endif
ifeq ($(OS),Darwin)
	LSANLFLAGS := -Llib/LeakSanitizer -llsan -lc++
endif

lsan: CFLAGS += -I $(LSAN) -Wno-gnu-include-next
lsan: LFLAGS += $(LSANLFLAGS)
lsan: fclean $(LSANLIB)
lsan: all

$(LSAN):
	@git submodule init $(LSAN)
	@git submodule update $(LSAN)

$(LSANLIB): $(LSAN)
	@$(MAKE) -C $(LSAN)
	@echo $(GREEN)"created liblsan.a"$(DEFAULT)

################################################################################
################################################################################

GREEN			= "\033[32m"
LGREEN			= "\033[92m"
DEFAULT			= "\033[39m"
RED				= "\033[31m"

################################################################################
################################################################################

.PHONY: all clean fclean re
