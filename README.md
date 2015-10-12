for FTM-50S2

- Build
$ make build

- Make image
Step 1.
	Insert SDCard into socket.

Step 2. Mount
	$ sudo mount /dev/sdb1 mmc

Step 3. Make image
	$ make target

Step 4. Unmount
	$ sudo umount mmc

