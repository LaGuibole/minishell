# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/07 11:03:15 by guphilip          #+#    #+#              #
#    Updated: 2025/04/08 19:55:28 by guphilip         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COLORS

BLUE				= 		\033[1;34m
GREEN				= 		\033[1;32m
YELLOW				=		\033[1;33m
CYAN				=		\033[1;36m
RESET				= 		\033[0m

# COMMANDS

RM					= /usr/bin/rm -fr
ECHO				= /usr/bin/echo
RM_LINE				= @tput cuu1 && tput el

# FLAGS && COMPILATION

CC 					=	cc
CFLAGS				=	-Wall -Wextra -Werror -g
LFLAGS				=	-lreadline
IFLAGS				=	-I$(INC_DIR)

# DIRECTORIES

SRC_DIR				=	src
INC_DIR				=	includes
LIB_DIR				=	libs
OBJ_DIR				=	obj

# MINISHELL

SRC_FILES			=	minishell core/display_prompt env/ft_envp
SRCS				=	$(addprefix $(SRC_DIR)/, $(addsuffix .c, $(SRC_FILES)))
OBJS				=	$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_FILES)))

# TARGET

NAME				=	minishell
LIB_TARGET			=	$(LIBFT_TARGET)

# LIBFT_GIT

LIBFT_GIT			= 		https://github.com/LaGuibole/Libft_Boosted.git
LIBFT_DIR			= 		Libft_Boosted
LIBFT_PATH			= 		$(LIB_DIR)/$(LIBFT_DIR)
LIBFT_H				= 		libft.h
LIBFT_H_PATH		= 		$(LIBFT_PATH)/includes/$(LIBFT_H)
LIBFT_INC_H			= 		$(INC_DIR)/$(LIBFT_H)
LIBFT_TARGET		= 		libft.a
LIBFT_TARGET_PATH	= 		$(LIBFT_PATH)/$(LIBFT_TARGET)

all:				$(NAME)

clean:
					@$(RM) -r $(OBJ_DIR)
					@$(RM) $(LIBFT_TARGET)
					@$(RM) $(LIBFT_INC_H)

fclean:
					@$(RM) $(NAME)

re: fclean all

cleanlibs:
							@$(MAKE) clean --no-print-directory -C $(LIBFT_PATH)

fcleanlibs: fclean
							@$(RM) -r $(LIBFT_PATH)

relibs: fcleanlibs $(LIBFT_INC_H) $(MLX_INC_H)

norminette:
	@norminette $(SRC_DIR) $(INC_DIR) | grep -Ev '^Notice|OK!$$' > .norme_errors;\
	if [ -s .norme_errors ]; then \
		cat .norme_errors; \
		$(ECHO) -e '\033[1;31mNorminette KO!'; \
	else \
		$(ECHO) -e '\033[1;32mNorminette OK!'; \
	fi;\
	rm -f .norme_errors

.PHONY: all re clean fclean cleanlibs fcleanlibs relibs norminette

$(LIBFT_INC_H): $(LIBFT_TARGET)
							@cp $(LIBFT_H_PATH) $@

$(LIBFT_TARGET):
							@if [ ! -d "$(LIBFT_PATH)/.git" ]; then \
								printf "Cloning lib \"%s\" into \"%s\"..." $(LIBFT_GIT) $(LIBFT_PATH);\
								git clone $(LIBFT_GIT) $(LIBFT_PATH);\
							fi
							@printf "\nMaking \"%s\"..." $@
							@$(MAKE) --no-print-directory -C $(LIBFT_PATH)
							@cp $(LIBFT_TARGET_PATH) ./
							@printf "\n✅ Done copying archive $(GREEN) \"%s\" $(RESET)... \n\n" $@

# MINISHELL TARGET

$(NAME): $(LIBFT_INC_H) $(OBJS)
					@$(CC) $(CFLAGS) $(IFLAGS) -o $(NAME) $(OBJS) $(LIBFT_TARGET) $(LFLAGS)
					@echo "🎉 Executable$(BLUE) $(NAME) $(RESET)created. \n"

$(OBJ_DIR):
					mkdir -p $(OBJ_DIR)

# .O COMPILATION

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
							@echo "⚙️ Compiling $< into $@ ...\n"
							@${RM_LINE}
							@mkdir -p $(dir $@)
							@$(CC) -I$(IFLAGS) $(CFLAGS) -c $< -o $@
							@${RM_LINE}
