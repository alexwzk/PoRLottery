//============================================================================
// Name        : main.cpp
// Author      : Zikai Alex Wen
// Version     :
// Copyright   :
// Description : Launch Run File
//============================================================================
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <openssl/sha.h>
#include <sys/time.h>

#include "src/merkle.h"

int main(int argc, char *argv[]) {
	using namespace std;

//	unsigned char test[5][LEAF_SIZE];
	leaf test[5];

	memcpy(test[0], "123", LEAF_SIZE); // Should be LEAF_SIZE otherwise the rest of char array won't be null
	memcpy(test[1], "234", LEAF_SIZE);
	memcpy(test[2], "567", LEAF_SIZE);
	memcpy(test[3], "567", LEAF_SIZE);
	memcpy(test[4], "567", LEAF_SIZE);
//	memcpy(test[2], "123" , 4);
	MERKLE testo(test, 5);
	testo.buildPath(0);
	/*cout << testo.returnHeight() << endl;
	 cout << COMMON::isEven(12) << endl;
	 cout << COMMON::isEven(11) << endl;
	 */
	return 0;
}
