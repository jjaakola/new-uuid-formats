
clean:
	rm -rf build

build:
	mkdir -p build

.PHONY := build-debug
build-debug:
	cd build && cmake -DCMAKE_BUILD_TYPE=DEBUG && make && cd ..

.PHONY := build-release
build-release:
	cd build && cmake -DCMAKE_BUILD_TYPE=RELEASE && make && cd ..

.PHONY := all
all: build-release


# Checks and formatting
.PHONY := format-check
format-check:
	find . -name "*.[h|c]" -not -path "./build/*" | xargs clang-format --dry-run -Werror

.PHONY := format
format:
	find . -name "*.[h|c]" -not -path "./build/*" | xargs clang-format -i
