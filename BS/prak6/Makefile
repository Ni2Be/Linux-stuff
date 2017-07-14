#Makefile

CXX = gcc
CXXFLAGS = -std=c99
RM = rm -f

FILES = race server client
CFILE = .c
doit:
	$(foreach var,$(FILES),$(CXX) $(CXXFLAGS) $(var)$(CFILE) -Wno-implicit -o $(var);)

clean:
	$(foreach var,$(FILES),$(RM) $(var);)
	