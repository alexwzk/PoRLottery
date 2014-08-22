#include "dealer.h"

DEALER::DEALER(char *filenamePt) {
	std::vector<zk_leaf> file_segmts;
	FILE *inputs;
	inputs = fopen(filenamePt, "r");
	if (inputs == NULL) {
		printf("Cannot open file @ DEALER 1st constructor. \n");
		exit(FILE_ERR);
	}

}
