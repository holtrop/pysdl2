all:
	python setup.py build

install:
	python setup.py install

test:
	./tests/test-sdl
