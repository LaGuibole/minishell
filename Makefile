# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: guillaumephilippe <guillaumephilippe@st    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/07 11:03:15 by guphilip          #+#    #+#              #
#    Updated: 2025/04/26 14:55:56 by guillaumeph      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLUE = \033[1;34m
GREEN =	\033[1;32m
RESET = \033[0m

SOURCES = \
	./src/builtins/cd.c \
	./src/builtins/echo.c \
	./src/builtins/env.c \
	./src/builtins/exit.c \
	./src/builtins/export.c \
	./src/builtins/pwd.c \
	./src/builtins/unset.c \
	./src/clean/clean_cmd.c \
	./src/core/display_prompt.c \
	./src/env/env_helpers.c \
	./src/env/ft_envp.c \
	./src/exec/execute_builtin.c \
	./src/exec/execute_cmd.c \
	./src/exec/execute_pipeline.c \
	./src/exec/handle_redirections.c \
	./src/exec/pipe_utils.c \
	./src/exec/redir_input.c \
	./src/exec/redir_output.c \
	./src/minishell.c \
	./src/parse/cmd.c \
	./src/parse/parameters.c \
	./src/parse/parse.c \
	./src/parse/parsing_command.c \
	./src/parse/redir.c \
	./src/pathing/cmd_path2.c \
	./src/pathing/cmd_path.c \
	./src/utils/builtin_utils/cd_helpers.c \
	./src/utils/builtin_utils/export_helpers.c \
	./src/utils/builtin_utils/unset_helpers.c \
	./src/utils/cmd_utils/clear_exit.c \
	./src/utils/debug/debug.c \
	./src/utils/errors/env_errors.c \
	./src/utils/heredoc_utils/create_heredoc.c \
	./src/utils/heredoc_utils/ft_mktemp.c \
	./src/utils/signals/signal_handler.c
OBJECTS = $(SOURCES:.c=.o)
OBJECTS := $(addprefix build/, $(OBJECTS))

CC = cc
CFLAGS = -g -Wall -Wextra -Werror $(INCLUDES) 
LFLAGS = -L/usr/local/Cellar/readline/8.2.13/lib -lreadline
NAME = minishell

LIBFT_GIT = https://github.com/LaGuibole/Libft_Boosted.git

LIBFT_PATH = ./libs/Libft

LIBFT = $(LIBFT_PATH)/libft.a

INCLUDES = \
	-Iincludes \
	-Ilibs/Libft/includes \
	-I /usr/local/Cellar/readline/8.2.13/include 
all: $(NAME)
	
build/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@if [ ! -d "$(LIBFT_PATH)" ]; then \
		printf "Cloning lib \"%s\" into \"%s\"..." $(LIBFT_GIT) $(LIBFT_PATH);\
		git clone $(LIBFT_GIT) $(LIBFT_PATH);\
		rm -rf $(LIBFT_PATH)/.git; \
	fi
	@make -sC $(LIBFT_PATH)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(LFLAGS) -o $(NAME)
	@echo "🎉 Executable$(BLUE) $(NAME) $(RESET)created."

clean:
	@rm -rf build
	@make -sC $(LIBFT_PATH) clean

fclean: clean
	@rm -f $(NAME)
	@make -sC $(LIBFT_PATH) fclean

removeLibft:
	@rm -rf $(LIBFT_PATH)
	@echo "$(GREEN)Libft$(RESET) remove"

updateSources:
	@bash -c '\
		IGNORED_DIRS=("libs" "src/dontcopy"); \
		FIND_CMD="find -type f -name '\''*.c'\''"; \
		for dir in "$${IGNORED_DIRS[@]}"; do \
			FIND_CMD+=" ! -path './\''$$dir/*'\''"; \
		done; \
		eval $$FIND_CMD | sort | awk '\''{print "\t" $$0 " \\"}'\'' \
		| sed '\''$$ s/ \\$$//'\'' > .sources_block; \
		cat .sources_block; \
		echo "SOURCES = \\" > .sources_full; \
		cat .sources_block >> .sources_full; \
		awk '\'' \
			FNR==NR { lines[NR] = $$0; next } \
			/^SOURCES =/ { \
				for (i=1; i<=length(lines); i++) print lines[i]; \
				in_block=1; next \
			} \
			in_block && /^[^\t]/ { in_block=0 } \
			!in_block { print } \
		'\'' .sources_full Makefile > Makefile.tmp; \
		mv Makefile.tmp Makefile; \
		rm -f .sources_block .sources_full; \
		echo "✅ Makefile mis à jour avec les fichiers sources." \
	'
	
re: fclean all

.PHONY: all clean fclean re
