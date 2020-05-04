all: exe run

exe: art_gallery.cpp RgbImage.cpp
	g++ -o exe art_gallery.cpp RgbImage.cpp -framework GLUT -framework OpenGL -Wno-deprecated

run: exe
	./exe

clean:
	rm exe