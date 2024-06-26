# Made with Python 3.11, on Ubuntu. Change as suitable
build:
	g++ stella_extension.cpp -o stella.so -shared -fPIC -I /usr/include/python3.11/

