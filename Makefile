NAME							=	webserv


CC								=	c++
LCFLAGS						=	-fsanitize=address
HEADERFLAGS				=	-I src/core -I include -I src/utils -I src/http \
										-I src/config -I src/misc
MAXFDS						= $(shell ulimit -n)
MAXCLIENTS				= $(shell expr $(MAXFDS) - 4)

ifeq ($(shell test $(MAXCLIENTS) -gt 1000; echo $$?),0)
	MAXCLIENTS			= 1000
endif

CFLAGS						=	$(LCFLAGS) $(HEADERFLAGS) -D MAX_CLIENTS=$(MAXCLIENTS) \
											-std=c++98 -Wall -Wextra -Werror -g -pedantic
LFLAGS						=	$(LCFLAGS)

################################################################################
################################################################################

SRC								=	$(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_DIR						=	src/
SRC_FILES					=	main.cpp

CORE							=	$(addprefix $(CORE_DIR), $(CORE_FILES))
CORE_DIR					=	src/core/
CORE_FILES				=	Socket.cpp Webserver.cpp CgiConnector.cpp  \
										Receive.cpp

HTTP							=	$(addprefix $(HTTP_DIR), $(HTTP_FILES))
HTTP_DIR					=	src/http/
HTTP_FILES				=	Request.cpp Response.cpp Status.cpp

CONFIG						=	$(addprefix $(CONFIG_DIR), $(CONFIG_FILES))
CONFIG_DIR				=	src/config/
CONFIG_FILES			=	ConfigFile.cpp Line.cpp Location.cpp Config.cpp \
										ConfigParsing.cpp LocationGet.cpp ConfigGet.cpp

UTILS							=	$(addprefix $(UTILS_DIR), $(UTILS_FILES))
UTILS_DIR					=	src/utils/
UTILS_FILES				=	GetContentDisposition.cpp Trim.cpp Last.cpp

MISC							=	$(addprefix $(MISC_DIR), $(MISC_FILES))
MISC_DIR					=	src/misc/
MISC_FILES				=	Signals.cpp

ALL_SRC						=	$(SRC) $(CORE) $(UTILS) $(HTTP) $(CONFIG) $(MISC)

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
	@echo $(LGREEN)"compiled: "$(LGREY)$^

$(NAME): $(ALL_OBJ_DIR) $(ALL_OBJ)
	@$(CC) $(ALL_OBJ) -o $(NAME) $(LFLAGS) $(LCFLAGS)
	@echo $(GREEN)"webserv compiled and ready!"$(DEFAULT)

clean:
	@rm -rfv $(patsubst %/,%,$(OBJ_DIR)) | grep '\.o$$' | sed "s/^/$(CLEANING) /"
	@echo $(RED)"repository cleaned of ofiles!"$(DEFAULT)

fclean:
	@rm -rfv $(patsubst %/,%,$(OBJ_DIR)) | grep '\.o$$' | sed "s/^/$(CLEANING) /"
	@rm -rfv $(NAME) | sed "s/^/$(CLEANING) /"
	@echo $(RED)"repository cleaned entirely!"$(DEFAULT)

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
	@mkdir -p $(ALL_OBJ_DIR)

norm:
	@cpplint $(shell find src -type f -name "*.cpp") \
						$(shell find src -type f -name "*.hpp") \
						$(shell find include -type f -name "*.hpp")

################################################################################
################################################################################

GREEN							=	"\033[32m"
LGREEN						=	"\033[92m"
DEFAULT						=	"\033[39m"
RED								=	"\033[94m"
LGREY							= "\033[37m"
CLEANING					= `printf "\033[36mcleaning:\033[37m"`

################################################################################
################################################################################

.PHONY: all clean fclean re norm linux nginx docker_clean
