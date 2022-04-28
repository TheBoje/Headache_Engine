# OF_ROOT =
# PROJECT_ROOT = .
# PROJECT_EXTERNAL_SOURCE_PATHS =
# PROJECT_EXCLUSIONS =
PROJECT_LDFLAGS=-lvoro++ -L/usr/local/lib -Wl,-rpath=./libs
# PROJECT_DEFINES = -DSILENT
PROJECT_CFLAGS+=-g -Wall -lvoro++ -I/usr/local/include/voro++ -I$(PROJECT_ROOT)/include -I$(PROJECT_ROOT)/include/Interface -I$(PROJECT_ROOT)/include/Renderer -I$(PROJECT_ROOT)/include/Utils -I$(PROJECT_ROOT)/include/3D -I$(PROJECT_ROOT)/include/Animation
PROJECT_OPTIMIZATION_CFLAGS_RELEASE =-O2
PROJECT_OPTIMIZATION_CFLAGS_DEBUG =-O2