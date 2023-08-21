NAME							=	webserv

CC								=	c++
LCFLAGS						=	-fsanitize=address
HEADERFLAGS				=	-I src/core -I include -I src/utils -I src/http \
										-I src/config
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
CORE_FILES				=	Socket.cpp TcpServer.cpp CgiConnector.cpp InOutHandler.cpp

HTTP							=	$(addprefix $(HTTP_DIR), $(HTTP_FILES))
HTTP_DIR					=	src/http/
HTTP_FILES				=	Request.cpp Response.cpp Status.cpp

CONFIG						=	$(addprefix $(CONFIG_DIR), $(CONFIG_FILES))
CONFIG_DIR				=	src/config/
CONFIG_FILES			=	ConfigFile.cpp Line.cpp Location.cpp Config.cpp \
										ConfigParsing.cpp LocationGet.cpp ConfigGet.cpp

UTILS							=	$(addprefix $(UTILS_DIR), $(UTILS_FILES))
UTILS_DIR					=	src/utils/
UTILS_FILES				= IsNumber.cpp

ALL_SRC						=	$(SRC) $(CORE) $(UTILS) $(HTTP) $(CONFIG)

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
	@echo $(GREEN)"linked "$@$(DEFAULT)

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

linux: $(REDIRECT)
	./docker/linux/docker_run.sh

nginx: $(REDIRECT)
	rm -f ./docker/nginx/nginx.conf
	cp ./conf/webserv.conf ./docker/nginx/nginx.conf
	./docker/nginx/docker_run.sh

docker_clean:
	-docker kill $(docker ps -q)
	docker system prune --all --volumes  -f


$(ALL_OBJ_DIR):
	mkdir -p $(ALL_OBJ_DIR)

norm:
	@cpplint $(shell find src -type f -name "*.cpp") \
						$(shell find src -type f -name "*.hpp") \
						$(shell find include -type f -name "*.hpp")

################################################################################
################################################################################

GREEN							=	"\033[32m"
LGREEN						=	"\033[92m"
DEFAULT						=	"\033[39m"
RED								=	"\033[31m"

################################################################################
################################################################################

.PHONY: all clean fclean re norm linux nginx docker_clean
