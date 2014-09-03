//============================================================================
// Name        : main.cpp
// Author      : Zikai Alex Wen
// Version     :
// Copyright   :
// Description : The simple PoR lottery is a primitive protocol for the Permacoin miners
//				 to prove that they are storing parts of the remote database on their machines.
//				 So far we have provided three command line tools to test the programme.
//		         The setup command slices the database up into file segments then computes a Merkle tree upon them.
//				 Withholding the client's public key the dealer is able to assign clients
//				 their parts of to-be-stored data." << endl;
//
//============================================================================

#include "src/dealer.h"
#include "src/user.h"
#include "src/verifier.h"

int main(int argc, const char *argv[]) {
	using namespace std;
	if (strcmp(argv[1], "--help") == SAME) {
		cout << "You are testing the simple PoR lottery programme. "
				<< "Welcome to the help manual:" << endl;
		cout << "Cmd1: --setup \"database filepath\" \"public key string\" "
				<< "\"outsourced data filepath\"" << endl;
		cout << "This creates a merkle tree then assign a client his\\her part of data." << endl;
		return FINE;
	}else if(strcmp(argv[1], "--setup") == SAME){
		if(argc != 5){
			cerr << "Invalid number of input strings for the setup mode." << endl;
			return INVALID_ERR;
		}
		DEALER* dealer = new DEALER(argv[2]); //argv[2]: pt to the database filepath
		dealer->outSource(argv[3],argv[4]);
		return FINE;
	}else{
		cerr << "Invalid command. try --help for more tips." << endl;
		return INVALID_ERR;
	}
	return FINE;
}
