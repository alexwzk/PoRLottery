//============================================================================
// Name        : main.cpp
// Author      : Zikai Alex Wen
// Version     :
// Copyright   :
// Description : Launch Run File
//============================================================================

#include "src/dealer.h"

int main(int argc, char *argv[]) {
	using namespace std;
	DEALER testo(argv[1]); //the path of database
	testo.createSource(argv[2]); // the public key
	return 0;
}
