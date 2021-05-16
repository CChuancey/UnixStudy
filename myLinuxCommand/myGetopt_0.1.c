#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv) {
	int opt;
	char* optstring = "ab:c";
	while((opt=getopt(argc,argv,optstring))!=-1) {
		switch(opt){
			case 'a':

				break;
			case 'b':
				puts(optarg);
				break;
			case 'c':
				
				break;

			default:
				fprintf(stderr,"Usage...\n");
				exit(EXIT_FAILURE);
		}
	}




	return 0;
}
