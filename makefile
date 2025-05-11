configure-debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -G "Ninja" --preset debug -S . -B ./build/debug

build-debug: configure-debug
	cmake --build --target example --preset debug

run-debug: build-debug
	./build/debug/examples/example

configure-release:
	cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" --preset release -S . -B ./build/release

build-release: configure-release
	cmake --build --target example --preset release

run-release: build-release
	./build/release/examples/example

