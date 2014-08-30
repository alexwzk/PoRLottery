//============================================================================
// Name        : main.cpp
// Author      : Zikai Alex Wen
// Version     :
// Copyright   :
// Description : Launch Run File
//============================================================================

#include "src/dealer.h"
#include "src/user.h"

int main(int argc, char *argv[]) {
	using namespace std;
	DEALER testo(argv[1]); //the path of database
	USER teste(argv[2]);	//the public key
	teste.getMKProofs(testo.createSource(argv[2]));
	teste.getNewPuzzle(argv[3]); // a test unpredictable puzzle id
	TICKET *testa = teste.releaseTicket();
	if (testa == nullptr) {
		cout << "ticket not ready" << endl;
		return -1;
	}
	int it_index = -1;
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
	}
	return 0;
}
