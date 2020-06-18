all:
	cd kernelmodule && make
	cd app && make

install_mod:
	cd kernelmodule && ./install_mod.sh

run_app:
	sudo app/vmm



