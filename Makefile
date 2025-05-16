

all: x86 arm64 riscv64


x86: build-x86
	./dockcross-x64 bash -c 'cmake --build build-x86 --target ubench -j $$(nproc)'

arm64: build-arm64
	./dockcross-arm64 bash -c 'cmake --build build-arm64 --target ubench -j $$(nproc)'

riscv64: build-riscv64
	./dockcross-riscv64 bash -c 'cmake --build build-riscv64 --target ubench -j $$(nproc)'

# Generate the build directory
build-x86: dockcross-x64
	./dockcross-x64 cmake -B $@ -DARCH=x86_64

build-arm64: dockcross-arm64
	./dockcross-arm64 cmake -B $@ -DARCH=aarch64

build-riscv64: dockcross-riscv64
	./dockcross-riscv64 cmake -B $@ -DARCH=riscv64

## Get the cross-compilation toolchains
dockcross-x64:
	docker run --rm dockcross/linux-x64 > ./dockcross-x64
	chmod +x ./dockcross-x64

dockcross-arm64:
	docker run --rm dockcross/linux-arm64 > ./dockcross-arm64
	chmod +x ./dockcross-arm64

dockcross-riscv64:
	docker run --rm dockcross/linux-riscv64 > ./dockcross-riscv64
	chmod +x ./dockcross-riscv64

clean:
	rm -rf build*

clean-all:
	rm -rf build*
	rm -f dockcross-*