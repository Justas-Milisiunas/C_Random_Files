#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char * argv[]) {
	int file, file_size, reiksme;
	struct stat sb;
	char *file_in_memory;
	
	if(argc != 3) {
		printf("Naudojimas:\n%s failas (reiksmes skaicius)\n", argv[0]);
		exit(255);
	}
	
	file = open(argv[1], O_RDWR);
	if(file == -1) {
		perror(argv[1]);
		exit(1);
	}
	
	fstat(file, &sb);
	file_size = sb.st_size;
	
	reiksme = atoi(argv[2]);
	if((reiksme == 0 && *argv[2] != '0') || reiksme > file_size || reiksme < 0) {
		printf("Blogas antras argumentas\n");
		exit(255);
	}
	
	file_in_memory = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
	if(file_in_memory == MAP_FAILED) {
		perror("Nepavyko prijungti failo prie atminties");
		exit(1);
	}
	
	printf("Reiksme: %c\n", file_in_memory[atoi(argv[2])]);
	
	munmap(file_in_memory, file_size);
	close(file);
	return 0;
}