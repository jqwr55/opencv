NONE         =\033[0m
BLACK        =\033[0;30m
RED          =\033[0;31m
GREEN        =\033[0;32m
BROWN_ORANGE =\033[0;33m
BLUE         =\033[0;34m
PURPLE       =\033[0;35m
CYAN         =\033[0;36m
LIGHT_GRAY   =\033[0;37m
DARK_GRAY    =\033[1;30m
LIGHT_RED    =\033[1;31m
LIGHT_GREEN  =\033[1;32m
YELLOW       =\033[1;33m
LIGHT_BLUE   =\033[1;34m
LIGHT_PURPLE =\033[1;35m
LIGHT_CYAN   =\033[1;36m
WHIE         =\033[1;37m

strip_cpp_postfix 			= $(subst .cpp,,$(1))
change_cpp_to_ob_postfix 	= $(subst .cpp,.o,$(1))
strip_str 					= $(subst $(1),,$(2))

MAKE_FILE 		= ./Makefile
PATH_TO_SRC 	= .
BIN_PATH		= .
SRC_FILE_PATHS 	= $(wildcard $(PATH_TO_SRC)/*.cpp)
SRC_FILES 		= $(subst $(PATH_TO_SRC)/,,$(SRC_FILE_PATHS))

OBJ_FILES 		= $(call strip_cpp_postfix,$(SRC_FILES))
OBJ_FILE_PATHS 	= $(addprefix ./bins/,$(OBJ_FILES))

# $1 src file name
# $2 src paths
# $3 dependencies
# $4 out path
# $5 flags
define make-target-build-path
$4: $2 $3
	@printf "$(GREEN)compiling $1 $(RED)dependencies{$3} $(NONE)\n"
	g++ $5 $2 -o $4
endef

# $1 target to run(ex run_main0)
# $2 build dependency target
# $3 path to exe
define make-target-run
$1: $2
	$3
endef

# 1 target
# 2 deps
# 3 path to bins
MAKE_TARGET_RUNS =																\
	$(foreach i, $(shell seq $(words $(1))), 									\
		$(eval IT_TARGET 	= $(word $(i),$(1)))								\
		$(eval IT_DEP    	= $(word $(i),$(2)))								\
		$(eval IT_EXE_PATh  = $(word $(i),$(3)))								\
		$(eval																	\
			$(call make-target-run,												\
				run_$(IT_TARGET),												\
				$(IT_DEP),													\
				$(IT_EXE_PATh)														\
			)																	\
		)																		\
	)

# 1 target names
# 2 target src files
# 3 target obj files
# 4 cflag
MAKE_TARGETS_COMPILE_FILE = 													\
	$(foreach i, $(shell seq $(words $(1))), 									\
		$(eval IT_TARGET = $(word $(i),$(1)))									\
		$(eval IT_SRC    = $(word $(i),$(2)))									\
		$(eval IT_OBJ    = $(word $(i),$(3)))									\
		$(eval 																	\
			$(call make-target-build-path, 										\
				$(IT_TARGET),													\
				$(IT_SRC),														\
				$(MAKE_FILE), 													\
				$(IT_OBJ),														\
				$(4),															\
			)																	\
		)																		\
	)

all: $(OBJ_FILE_PATHS)
	@printf "$(GREEN)all built$(NONE)\n"

$(call MAKE_TARGETS_COMPILE_FILE,$(OBJ_FILE_PATHS),$(SRC_FILE_PATHS),$(OBJ_FILE_PATHS), -g -std=c++17 -g -I ./ `pkg-config --cflags --libs opencv4`)
$(call MAKE_TARGET_RUNS,$(OBJ_FILES),$(OBJ_FILE_PATHS),$(OBJ_FILE_PATHS))

purge:
	rm $(addprefix ./bins/,$(OBJ_FILES))