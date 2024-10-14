BE SURE TO ADD CROSS COMPILER PATH FIRST BY TYPING export PATH="$PWD/cross/bin:$PATH" IN THE TERMINAL

For Problem 2, we used diskfs. So please do the following:
	run "make clean" and then "make" in the Linux terminal.
	Install "disk.img" by running "make run" in the Linux terminal
	Once inside qemu, execute "mount ata 0 diskfs" to mount diskfs on ata 0
	Copy the contents from cdromfs at atapi 2 to ata 0 by running "install atapi 2 ata 0".
	Then execute "run bin/named_pipe_test.exe".

