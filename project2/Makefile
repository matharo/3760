CFLAGS = -std=c++11 `pkg-config --cflags python3` -fPIC
LFLAGS = `pkg-config --libs python3`

MODE ?= 'RELEASE'

# Release mode
ifeq ($(MODE), RELEASE)
    CFLAGS += -DNDEBUG -O3 -march=native
endif

# Debug mode
ifeq ($(MODE), DEBUG)
    CFLAGS += -DDEBUG -fno-omit-frame-pointer -g -D_LIBCPP_DEBUG
endif

# Profile mode
ifeq ($(MODE), PROFILE)
		CFLAGS += -DNDEBUG -g -march=native -fno-omit-frame-pointer
endif

# Enable address sanitizer
ifeq ($(MODE), ADDRESS)
    CFLAGS += -DDEBUG -fno-omit-frame-pointer -g -D_LIBCPP_DEBUG -fsanitize=address
endif

# Enable memory sanitizer
ifeq ($(MODE), MEMORY)
    CFLAGS += -DDEBUG -fno-omit-frame-pointer -g -D_LIBCPP_DEBUG -fsanitize=memory
endif

CXX = g++ $(CFLAGS)


pysrcs = $(wildcard *.py.cpp)
srcs = $(filter-out $(pysrcs), $(wildcard *.cpp))

pyobjs = $(pysrcs:.cpp=.o)
pydeps = $(pysrcs:.cpp=.d)
pylibs = $(pysrcs:.py.cpp=.so)
objs = $(srcs:.cpp=.o)
deps = $(srcs:.cpp=.d)

headers = $(wildcard *.h)

format:
		echo $(srcs)
		echo $(pysrcs)
		clang-format -i *.h *.cpp

check:
		clang-check -analyze $(headers) -- -Wall -x c++ $(CFLAGS)
		clang-check -analyze $(srcs) $(pysrcs) -- -Wall -x c++ $(CFLAGS)
		
tidy:
		clang-tidy -checks=*,-llvm-header-guard $(srcs) $(pysrcs) -header-filter=.* -- -Wall -x c++ $(CFLAGS)

%.o: %.cpp
		$(CXX) -MMD -MP -c $<

%.so: %.py.o $(objs) $(pyobjs)
		if [[ "$*" == "Pente" ]]; then \
				$(CXX) -shared PenteState.o Player.o $< $(CFLAGS) $(LFLAGS) -o $@; \
		else \
				$(CXX) -shared PenteState.o Player.o $*.o $< $(CFLAGS) $(LFLAGS) -o $@; \
		fi
		
clean:
		rm *.o *.d *.pyc $(pylibs)

		
all: $(pylibs)

-include $(deps)
