AM_CPPFLAGS = $(LIBASSUAN_CFLAGS) $(LIBGCRYPT_CFLAGS)
LDADD = $(LIBASSUAN_LIBS) $(LIBGCRYPT_LIBS)
AM_CXXFLAGS = $(CXXFLAGS) -std=c++17

bin_PROGRAMS = assuan-toy
assuan_toy_SOURCES = main.cc gpghelper.h gpghelper.cc sexp.h sexp.cc util.h util.cc
