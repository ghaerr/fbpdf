MUPDFINC = -I../mupdf/include
MUPDFLIB = -L../mupdf/build/release -lmupdf -lmupdf-third -lmupdf-pkcs7 -lmupdf-threads
NANOXINC = -I../microwindows/src/include
NANOXLIB = -L../microwindows/src/lib -lnano-X
DRAW = nxdraw.o
#DRAW = draw.o
CC = cc
CFLAGS = -Wall -O2 $(MUPDFINC) $(NANOXINC)

all: fbpdf
xall: fbpdf fbdjvu
%.o: %.c doc.h
	$(CC) -c $(CFLAGS) $<
clean:
	-rm -f *.o fbpdf fbdjvu fbpdf2

# pdf support using mupdf
fbpdf: fbpdf.o mupdf.o $(DRAW)
	$(CC) -o $@ $^ $(LDFLAGS) $(MUPDFLIB) $(NANOXLIB) -lm

# djvu support
fbdjvu: fbpdf.o djvulibre.o draw.o
	$(CXX) -o $@ $^ $(LDFLAGS) -ldjvulibre -ljpeg -lm -lpthread

# pdf support using poppler
poppler.o: poppler.c
	$(CXX) -c $(CFLAGS) `pkg-config --cflags poppler-cpp` $<
fbpdf2: fbpdf.o poppler.o draw.o
	$(CXX) -o $@ $^ $(LDFLAGS) `pkg-config --libs poppler-cpp`
