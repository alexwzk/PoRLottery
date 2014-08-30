//============================================================================
// Name        : main.cpp
// Author      : Zikai Alex Wen
// Version     :
// Copyright   :
// Description : Launch Run File
//============================================================================

#include "src/merkle.h"

int main(int argc, char *argv[]) {
	using namespace std;
	//	unsigned char test[5][LEAF_SIZE];
	leaf test[5];
	vector<uchar *> testf;
	memcpy(test[0], "123", LEAF_SIZE); // Should be LEAF_SIZE otherwise the rest of char array won't be null
	memcpy(test[1], "234", LEAF_SIZE);
	memcpy(test[2], "567", LEAF_SIZE);
	memcpy(test[3], "567", LEAF_SIZE);
	memcpy(test[4], "567", LEAF_SIZE);
	for(int i = 0; i < 5; i++){
		testf.push_back(test[i]);
	}
	//	memcpy(test[2], "123" , 4);
	MERKLE testo(testf);
	testo.buildPath(0);

	/*
	 DEALER testo(argv[1]); //the path of database
	 USER teste(argv[2]);
	 teste.getMKProofs(testo.createSource(argv[2]));
	 teste.getNewPuzzle(argv[3]); // a test unpredictable puzzle id
	 TICKET *testa = teste.releaseTicket();
	 if (testa == nullptr) {
	 cout << "ticket not ready" <<endl;
	 return -1;
	 }
	 int it_index = -1;
	 cout << "pubkey: " << COMMON::stringToHex(testa->pubkey) << endl;
	 cout << "seed: " << COMMON::stringToHex(testa->seed) << endl;
	 for (auto it : testa->mkproofs) {
	 cout << "item: " << it.item.data();
	 cout << " and its hash siblings are: " << endl;
	 it_index = 0;
	 for (auto jt : it.siblings) {
	 cout << "layer " << (it_index++) << " ";
	 COMMON::printHash(jt.data());
	 }
	 }
	 */
	return 0;
}
