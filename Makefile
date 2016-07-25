FLAGZ=`pkg-config --libs sdl2` -lGL -lglut -lGLU -lGLEW -DGLEW_STATIC -g


output:\
    	ResourceManager.o\
	main.o
	    g++\
		$(FLAGZ)\
		ResourceManager.o\
		main.o\
		-o fiddle

main.o:
	g++ $(FLAGZ) -c main.cpp

ResourceManager.o: ResourceManager/ResourceManager.cpp ResourceManager/ResourceManager.h
	g++ -c ResourceManager/ResourceManager.cpp

clean:
	rm *.o
