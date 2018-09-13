CFLAGS = --std=c++11 `pkg-config --cflags python2` -fPIC
LFLAGS = `pkg-config --libs python2`

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
		CFLAGS += -DNDEBUG -g -pg -march=native -fno-omit-frame-pointer
endif

# Enable address sanitizer
ifeq ($(MODE), ADDRESS)
    CFLAGS += -DDEBUG -fno-omit-frame-pointer -g -D_LIBCPP_DEBUG -fsanitize=address
endif

# Enable memory sanitizer
ifeq ($(MODE), MEMORY)
    CFLAGS += -DDEBUG -fno-omit-frame-pointer -g -D_LIBCPP_DEBUG -fsanitize=memory
endif

#CXX = clang++ $(CFLAGS)
CXX = g++ $(CFLAGS)

srcs = $(wildcard *.cpp)
objs = $(srcs:.cpp=.o)
deps = $(srcs:.cpp=.d)

headers = $(wildcard *.h)

all: lib

format:
		clang-format -i *.h *.cpp

check:
		clang-check -analyze $(headers) -- -Wall -x c++ $(CFLAGS)
		clang-check -analyze $(srcs) -- -Wall -x c++ $(CFLAGS)
		
tidy:
		clang-tidy -checks=*,-llvm-header-guard $(srcs) -header-filter=.* -- -Wall -x c++ $(CFLAGS)

%.o: %.cpp
		$(CXX) -MMD -MP -c $<

clean:
		rm *.o *.d *.so $(executables)

lib: $(objs)
		$(CXX) -shared $(objs) $(CFLAGS) $(LFLAGS) -o SlidingTilePuzzleSolver.so

exec: $(objs)
		$(CXX) $(objs) $(CFLAGS) $(LFLAGS) -o a.out


-include $(deps)
