all:
	gcc -o build.exe src/*.c -static -Ofast -Os -Oz  

shut:
	gcc src/*.c test/*.c -g -o shut.exe