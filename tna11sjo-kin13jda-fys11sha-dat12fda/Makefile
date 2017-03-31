SRC = src

all: 
	cd $(SRC); make


install: all
	mv src/clientmain bin/clientmain
	mv src/servermain bin/servermain
	mv src/Database.db bin/Database.db

clean: 
	rm ./bin/*
	cd $(SRC); make clean
