/* This program copies all bytes from a file starting from n-th index to the beginning of the file */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

int check_argument(char * arg, int file_size);
int get_file_size(int file);
int open_file(char *file);
int seek_file(int file, int offset);
int get_write_size(int file_size, int index);
int read_file(int file, char *buffer, int size);
int write_file(int file, char *buffer, int size);

int check_argument(char * arg, int file_size) {
	int index;
	index = atoi(arg);
	if(index <= 0 || index >= file_size) {
		printf("Blogas argumentas\n");
		exit(255);
	}
	
	return index;
}

int get_file_size(int file) {
	int result;
	struct stat sb;
	result = fstat(file, &sb);
	if(result == -1) {
		perror("Fstat");
		exit(1);
	}
	
	return sb.st_size;
}

int open_file(char *file) {
	int result;
	result = open(file, O_RDWR);
	if(result == -1) {
		perror(file);
		exit(1);
	}
	
	return result;
}

int seek_file(int file, int offset) {
	int result;
	result = lseek(file, offset, SEEK_SET);
	if(result== -1) {
		perror("Nepavyko seek");
		exit(1);
	}
	
	return result;
}

int get_write_size(int file_size, int index) {
	int write_size;
	if((file_size - index) > index) {
		write_size = index;
	} else {
		write_size = file_size - index;
	}
	
	return write_size;
}

int read_file(int file, char *buffer, int size) {
	int result;
	result = read(file, buffer,  size);
	/*if(result > 0) {
		perror("data.bin read");
		exit(1);
	}*/
	
	return result;
}

int write_file(int file, char *buffer, int size) {
	int result;
	result = write(file, buffer, size);
	if(result == -1) {
		perror("data.bin write");
		exit(1);
	}
	
	return result;
}

int main(int argc, char * argv[]) {
	int file, index, write_size, file_size;
	char *data;
	
	file = open_file("data.bin");
	file_size = get_file_size(file);
	index = check_argument(argv[1], file_size);
	
	seek_file(file, index);
	write_size = get_write_size(file_size, index);
	
	data = (char*)malloc(write_size);
	read_file(file, data, write_size);
	
	seek_file(file, 0);
	write_file(file, data, write_size);
	
	close(file);
	free(data);
	
	return 0;
}
