
all:
	cd ./src/icp/ && qmake ./icp.pro && make

run:
	./src/icp/icp

clean:
	cd ./src/icp && rm *.o icp Makefile;
	
doxygen:
	doxygen ./doc/doxyConfig

pack:
	zip -r xbalaz09-xpavlo14.zip .
