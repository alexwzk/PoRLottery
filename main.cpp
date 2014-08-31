//============================================================================
// Name        : main.cpp
// Author      : Zikai Alex Wen
// Version     :
// Copyright   :
// Description : Launch Run File
//============================================================================

#include "src/dealer.h"
#include "src/user.h"
#include "src/verifier.h"

int main(int argc, char *argv[]) {
	//command line: ./SimplePoR data.in pubickeystring1 pathsout.txt puzzleid1 ticketout.txt
	using namespace std;
	DEALER* testo = new DEALER(argv[1]);// the database file
	USER* teste = new USER(argv[2]);	//the public key
	testo->outSource(argv[2],argv[3]);
	teste->readPathsFile(argv[3]);	//stores path
	teste->getNewPuzzle(argv[4]);	//puzzle id
	teste->releaseTicket(argv[5]);	//ticket path
	digest root;
	memcpy(root,testo->releaseRoot(),HASH_SIZE);
	VERIFIER* testa = new VERIFIER(root,argv[5]);
	cout << testa->verifyAllChallenges() << endl;
	delete testa;
	delete teste;
	delete testo;
	return 0;
}
