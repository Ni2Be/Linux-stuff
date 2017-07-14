#Makefile

CXX = gcc
CXXFLAGS = -std=c99
RM = rm -f

FILES = dnc ww wg_s wg_c
CFILE = .c
doit:
	$(foreach var,$(FILES),$(CXX) $(CXXFLAGS) $(var)$(CFILE) -o $(var);)

clean:
	$(foreach var,$(FILES),$(RM) $(var);)
	