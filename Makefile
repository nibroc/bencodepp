CXXFLAGS+=-Wall -Werror -Wextra -Weffc++ \
	    -Wstrict-aliasing -pedantic -std=c++11
LDLIBS+=

# -fpic

CXX=clang++

SRCS=encode.cpp decode.cpp value.cpp
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
DEPS=.depend
LIBRARY=bencode.a

release: CXXFLAGS+=-O2
release: build

debug: CXXFLAGS+=-O0 -g -fno-omit-frame-pointer
debug: LDFLAGS+=-g
debug: build

build: depend $(OBJS)
	ar rvs $(LIBRARY) $(OBJS)
	$(CXX) $(CXXFLAGS) -o bencode_test bencode_test.cpp $(LIBRARY) $(LDFLAGS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > ./.depend

clean:
	$(RM) $(RMFLAGS) $(OBJS) .depend bencode_test $(LIBRARY)

tests: build
	./bencode_test

-include $(DEPS)
