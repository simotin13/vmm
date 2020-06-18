all:
	make -C app
	make -C kernelmodule

install_mod:
	cd kernelmodule && ./install_mod.sh

run_app:
	sudo app/vmm



