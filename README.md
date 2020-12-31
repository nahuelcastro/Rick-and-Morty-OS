Rick & Morty OS is a group project, focused on System Programming, carried out for the subject "Organization Of The Computer II" at the University of Buenos Aires. 

This project consists of a set of exercises in which the concepts of System Programming seen in the theoretical and practical classes are applied. The exercises are inspired by the Rick and Morty series. You can read the complete statements in the enunciado.pdf file. The Bochs program was used as a test environment, simulating an IBM-PC computer, booting our OS from a floppy disk.

------------

## Download and Install
#### Install Bochs
- Download from: https://sourceforge.net/projects/bochs/files/bochs/2.6.9/  the file: ***bochs-2.6.9.tar.gz***
- Unzip: `tar -xvvzf bochs-2.6.9.tar.gz`
- In the unzipped folder do:
```bash
$ ./configure --enable-debugger --enable-disasm --disable-docbook --enable-readline LDFLAGS='-pthread' --prefix=/home/<USER>/bochs/
$ make
$ make install
```
- Packages that may be missing to install:
```bash
$ sudo apt-get install xorg-dev
$ sudo apt-get install libx11-dev
$ sudo apt-get install libxrandr-dev
$ sudo apt-get install libgtk2.0-dev
```

-  Use bochs from any path
- - Add to file **/home/< user >/.bashrc**: `export PATH+=":/home/< user >/bochs/bin/"`
- -  Load changes in the current console: `$ source ~/.bashrc`

#### Clone repository or download
- Clone with HHTPS:`$ git clone https://github.com/nahuelcastro/Rick-and-Morty-OS.git`

### Run 
In **Rick-and-Morty-OS/src**
```bash
$ make
$ bochs -q
```
