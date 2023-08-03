# TODO use variables

clean:
	rm -f copier mtcopier copier_files/*.o mtcopier_files/*.o *.o

copier: copier_files/main.o copier_files/reader.o copier_files/writer.o
	g++ -Wall -Werror -std=c++20 -g -o $@ $^

mtcopier: mtcopier_files/main.o mtcopier_files/reader.o mtcopier_files/writer.o
	g++ -Wall -Werror -std=c++20 -g -o $@ $^

copier_files/%.o: %.cpp
	g++ -Wall -Werror -std=c++20 -g -c $^

mtcopier_files/%.o: %.cpp
	g++ -Wall -Werror -std=c++20 -g -c -lpthread $^