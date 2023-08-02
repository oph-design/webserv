NAME							=	webserv

CC								=	c++
LCFLAGS						=	# -fsanitize=address
HEADERFLAGS				=	-I src/core
CFLAGS						=	$(LCFLAGS) $(HEADERFLAGS) \
											-std=c++98 -Wall -Wextra -Werror -g -pedantic
LFLAGS						=	$(LCFLAGS)

################################################################################
################################################################################

SRC								=	$(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_DIR						=	src/
SRC_FILES					=	main.cpp 

CORE							=	$(addprefix $(CORE_DIR), $(CORE_FILES))
CORE_DIR					=	src/core/
CORE_FILES				=	Header.cpp TcpServer.cpp

ALL_SRC						=	$(SRC) $(CORE)

################################################################################
################################################################################

OBJ_DIR						=	obj/
ALL_OBJ						=	$(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(ALL_SRC))
ALL_OBJ_DIR				=	$(sort $(dir $(ALL_OBJ)))

################################################################################
################################################################################

all: $(ALL_OBJ_DIR) $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
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

REDIRECT					= $(HOME)/goinfre/docker

$(REDIRECT):
	./docker/setup.sh

docker: $(REDIRECT)
	./docker/docker_run.sh

$(ALL_OBJ_DIR):
	mkdir -p $(ALL_OBJ_DIR)

norm:
	cpplint $(ALL_SRC)

################################################################################
################################################################################

GREEN							=	"\033[32m"
LGREEN						=	"\033[92m"
DEFAULT						=	"\033[39m"
RED								=	"\033[31m"

################################################################################
################################################################################

.PHONY: all clean fclean re docker norm
