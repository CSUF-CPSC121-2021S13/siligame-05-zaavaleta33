 ## Unittest name
UTNAME		:= unittest.cc
# Flags added to compilation step
COMPILE_FLAGS		:= -lm -lX11 -lpthread
# Flags added to unittest compilation step
UT_COMPILE_FLAGS	:= -lm -lX11 -lpthread
# Flags added for mac compilation, if different from COMPILE_FLAGS
MAC_COMPILE_FLAGS	:= -lm -I/opt/X11/include -lpthread -lX11 -lstdc++ -I/usr/X11R6/include -L/usr/X11R6/lib
# Flags added for mac unittest compilation step, if different from UT_COMPILE_FLAGS
MAC_UT_COMPILE_FLAGS := -lm -lpthread -lX11 -I/usr/X11R6/include -L/usr/X11R6/lib
# Space-separated list of implementation files that should not be style/format
# checked, i.e. library definitions from cpputils.
OTHER_IMPLEMS	:= cpputils/graphics/image.cc
# Space-separated list of header files (e.g., algebra.hpp)
HEADERS      		:= player.h opponent.h game_element.h game.h
# Space-separated list of implementation files (e.g., algebra.cpp)
IMPLEMS       		:= player.cc opponent.cc game_element.cc game.cc
# File containing main (e.g., main.cpp)
DRIVER        		:= main.cc
# Expected name of executable file
EXEC_FILE      		:= main
# Flags to pass to clang-format, for example, --style=Google. Use quotes around
# multiple flags. Optional.
CLANG_FORMAT_FLAGS	:= "--style=Google"
