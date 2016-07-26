FLAGZ=`pkg-config --libs sdl2` -lGL -lglut -lGLU -lGLEW -DGLEW_STATIC -g


output:\
    	lodepng.o\
    	ResourceManager.o\
	main.o
	    g++\
		$(FLAGZ)\
		lodepng.o\
		ResourceManager.o\
		main.o\
		-o fiddle

main.o:
	g++ $(FLAGZ) -c main.cpp

lodepng.o: lodepng/lodepng.h lodepng/lodepng.cpp
	g++ -c lodepng/lodepng.cpp

ResourceManager.o: ResourceManager/ResourceManager.cpp ResourceManager/ResourceManager.h
	g++ -c ResourceManager/ResourceManager.cpp

clean:
	rm *.o
