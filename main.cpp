//============================================================================
// Name        : main.cpp
// Author      : Zikai Alex Wen
// Version     :
// Copyright   :
// Description : Launch Run File
//============================================================================

#include "src/dealer.h"
#include "src/user.h"
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<DEALER> dealerPtr;

int main(int argc, char *argv[]) {
	using namespace std;
	//dealerPtr testo (new DEALER(argv[1])); //the path of database
	DEALER* testo = new DEALER(argv[1]);
	USER* teste = new USER(argv[2]);	//the public key
	testo->outSource(argv[2],argv[3]);
	teste->readPathsFile(argv[3]);	//stores path
	teste->getNewPuzzle(argv[4]);	//puzzle id
	teste->releaseTicket(argv[5]);	//ticket path
	/*TICKET* testa = new TICKET;
	if (testa == nullptr) {
		cout << "ticket not ready" << endl;
		return -1;
	}*/
	/*int it_index = -1;
	cout << "pubkey: " << COMMON::stringToHex(testa->pubkey) << endl;
	cout << "seed: " << COMMON::stringToHex(testa->seed) << endl;
	for (auto it : testa->mkproofs) {
		cout << "item: ";
		COMMON::printHex((const uchar*)it->returnLeafPt(),LEAF_SIZE);
		cout << " and its hash siblings are: " << endl;
		it_index = 0;
		for (auto jt : it->returnSiblings()) {
			cout << "layer " << (it_index++) << " ";
			COMMON::printHex(jt,HASH_SIZE);
		}
	}*/
	//delete testa;
	delete teste;
	delete testo;
	return 0;
}
