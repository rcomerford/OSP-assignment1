COPIER_DIR 	 := ./copier_files
MTCOPIER_DIR := ./mtcopier_files

.default: all

all: copier mtcopier

clean:
	rm -rf copier mtcopier $(COPIER_DIR)/*.o $(MTCOPIER_DIR)/*.o *.o

copier: $(COPIER_DIR)/main.o $(COPIER_DIR)/reader.o $(COPIER_DIR)/writer.o
	g++ -Wall -Werror -std=c++20 -g -o $@ $^

mtcopier: $(MTCOPIER_DIR)/main.o $(MTCOPIER_DIR)/reader.o $(MTCOPIER_DIR)/writer.o
	g++ -Wall -Werror -std=c++20 -g -o $@ $^

$(COPIER_DIR)/%.o: %.cpp
	g++ -Wall -Werror -std=c++20 -g -c $^

$(MTCOPIER_DIR)/%.o: %.cpp
	g++ -Wall -Werror -std=c++20 -g -c -lpthread $^