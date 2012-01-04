all:
	python setup.py build

install:
	python setup.py install

wininst:
	python setup.py bdist_wininst

test:
	./tests/test-sdl

clean:
	-rm -rf build
