IF EXIST vextor2d.o del vector2d.o
IF EXIST libvector2d.so del libvector2d.so
IF EXIST collision.exe del collision.exe
gcc -c -Wall -Werror -fpic -Ic:\users\dfarrell\Dropbox\C vector2d.c
gcc -shared -o libvector2d.so vector2d.o
gcc -Lc:\users\dfarrell\Dropbox\C  -Wall -Ic:\users\dfarrell\Dropbox\C collision.c -o collision.exe libvector2d.so
IF EXIST collision.exe collision