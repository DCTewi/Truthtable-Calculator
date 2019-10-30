# Truth table and DNF, CNF caltulator
#
# For g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
# For g++ (MinGW.org GCC-6.3.0-1) 6.3.0
#
# Author: dctewi@dctewi.com
#

# Build target
target=Calculator

# Objects
obj=main.o \
	functions.o

# Compliler
CC:=g++

# Main recipe
$(target): $(obj)
	$(CC) $^ -o $@ -Wall -std=c++17 -O2

# Sub recipe
%.o: %.cpp
	$(CC) $< -o $@ -Wall -std=c++17 -O2 -c

# Clean
.PHONY:clean cleanw
clean:
	-rm $(obj) $(target)

cleanw:
	-del $(obj) $(target) $(target).exe
