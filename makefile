configure:
	cmake -DCMAKE_BUILD_TYPE=Debug --preset debug -S . -B ./build/debug

build: configure
	cmake --build --target example --preset debug

run: build
	./build/debug/examples/example