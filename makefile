all: exe run

exe: tp5texturasAlunos.cpp RgbImage.cpp
	g++ -o exe tp5texturasAlunos.cpp RgbImage.cpp -framework GLUT -framework OpenGL -Wno-deprecated

run: exe
	./exe

clean:
	rm exe