#! /usr/bin/env bash

touch Makefile
touch config.mk

COUNTER=1

echo 'CXX=g++-6' > config.mk
echo 'CXXFLAGS=-Werror -Wall -Wextra -fno-exceptions -Wno-format -fno-rtti -std=c++1y' >> config.mk
echo 'OUTPUT="./bin"' >> config.mk
echo 'SILENT_MKDIR=' >> config.mk


echo 'include config.mk' > Makefile
echo 'all: $(OUTPUT) build' >> Makefile
echo 'build:' >> Makefile

for i in $(ls -d */); do echo '	@$(CXX) $(CXXFLAGS) -o $(OUTPUT)/'"${i%%/}"' '"${i%%/}"'/src/*.cpp -I '"${i%%/}"'/include/' >> Makefile \
&& echo '	@echo' "$COUNTER"'. CC '"${i%%/}" >> Makefile \
&& (( COUNTER++ )) \
; done

echo 'start:' >> Makefile
echo '	$(SILENT_MKDIR)mkdir $(OUTPUT)' >> Makefile
echo 'clean:' >> Makefile
echo '	rm -rf $(OUTPUT)' >> Makefile

make clean
make start
make build

for i in $(ls bin/); do ./bin/$i; done
