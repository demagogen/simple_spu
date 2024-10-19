.PHONY: all processor simple_asm simple_disasm clean

BOLD_RED 	  = \033[1;31m
BOLD_GREEN 	  = \033[1;32m
BOLD_YELLOW	  = \033[1;33m
BOLD_BLUE	  = \033[1;34m
BOLD_MAGENTA  = \033[1;35m
BOLD_CYAN	  = \033[1;36m
BOLD_WHITE	  = \033[1;37m
DEFAULT_STYLE = \033[0m

all: processor simple_asm simple_disasm compilation_end_point

processor:
	@printf "$(BOLD_CYAN)START CREATING processor OUTFILE$(DEFAULT_STYLE)\n"

	@cd processor && make
	@cd ../

	@printf "$(BOLD_GREEN)processor outfile created$(DEFAULT_STYLE)\n\n"

simple_asm:
	@printf "$(BOLD_CYAN)START CREATING simple_asm OUTFILE$(DEFAULT_STYLE)\n"

	@cd simple_asm && make
	@cd ../

	@printf "$(BOLD_GREEN)simple_asm outfile created$(DEFAULT_STYLE)\n\n"

simple_disasm:
	@printf "$(BOLD_CYAN)START CREATING simple_disasm OUTFILE$(DEFAULT_STYLE)\n"

	@cd simple_disasm && make
	@cd ../

	@printf "$(BOLD_GREEN)simple_disasm outfile created$(DEFAULT_STYLE)\n\n"

compilation_end_point:
	@printf "$(BOLD_MAGENTA)Compilation of simple_spu ended\nMy congratulations!$(DEFAULT_STYLE)\n\n"

clean:
	@printf "$(BOLD_CYAN)start cleaning$(DEFAULT_STYLE)\n\n"

	@printf "$(BOLD_MAGENTA)START CLEANING processor BUILD$(DEFAULT_STYLE)\n"
	@cd processor && make clean
	@cd ../
	@printf "$(BOLD_MAGENTA)processor BUILD CLEANED$(DEFAULT_STYLE)\n\n"

	@printf "$(BOLD_MAGENTA)START CLEANING simple_asm BUILD$(DEFAULT_STYLE)\n"
	@cd simple_asm && make clean
	@cd ../
	@printf "$(BOLD_MAGENTA)simple_asm BUILD CLEANED$(DEFAULT_STYLE)\n\n"

	@printf "$(BOLD_MAGENTA)START CLEANING simple_disasm BUILD$(DEFAULT_STYLE)\n"
	@cd simple_disasm && make clean
	@cd ../
	@printf "$(BOLD_MAGENTA)simple_disasm BUILD CLEANED$(DEFAULT_STYLE)\n\n"

	@rm spu_debug.log

	@printf "$(BOLD_CYAN)build cleaned$(DEFAULT_STYLE)\n"

