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
	if (argc < 2) {
		cerr << "Invalid command. try --help for more tips." << endl;
		return INVALID_ERR;
	}
	if (strcmp(argv[1], "--help") == SAME) {
		cout << "You are testing the simple PoR lottery programme. "
				<< "Welcome to the help manual:" << endl;
		cout << "cmd1: --setup \"database filepath\" \"public key string\" "
				<< "\"outsourced data filepath\" -r \"file receiving root digest\""
				<< endl;
		cout
				<< "This creates a merkle tree then assign a client his\\her part of data. "
				<< "optional command -r following with a file path will ask the dealer to "
				<< "release its database's root digest. " << endl;
		cout
				<< "cmd2: --scratchoff \"userdata filepath\" \"public key string\" "
				<< "\"puzzle id string\" \"release ticket filepath\"" << endl;
		cout << "The 2nd command kicks off scratch-off process "
				<< "which acts as a user to solve the puzzle by accessing owned data."
				<< endl;
		cout
				<< "cmd3: --verify \"root digest file\" \"ticket filepath\" \"puzzle id string\" "
				<< endl;
		cout << "The 3rd command verifies a released ticket "
				<< "(all challenged segments carry a valid Merkle proof."
				<< endl;
		return FINE;
	} else if (strcmp(argv[1], "--setup") == SAME) {
		if (argc == 5) {
			DEALER* dealer = new DEALER(argv[2]);
			dealer->outSource(argv[3], argv[4]);
			delete dealer;
			return FINE;
		} else if (argc == 7 && strcmp(argv[5], "-r") == SAME) {
			DEALER* dealer = new DEALER(argv[2]);
			dealer->outSource(argv[3], argv[4]);
			dealer->releaseRoot(argv[6]);
			delete dealer;
			return FINE;
		}
		cerr << "Invalid number of input strings for the setup mode." << endl;
		return INVALID_ERR;
	} else if (strcmp(argv[1], "--scratchoff") == SAME) {
		if (argc != 6) {
			cerr << "Invalid number of input strings for the scratch-off mode."
					<< endl;
			return INVALID_ERR;
		}
		USER* user = new USER(argv[3]);
		user->storeFile(argv[2]);
		user->getNewPuzzle(argv[4]);
		user->releaseTicket(argv[5]);
		delete user;
		return FINE;
	} else if (strcmp(argv[1], "--verify") == SAME) {
		if (argc != 5) {
			cerr << "Invalid number of input strings to initiate the verifier."
					<< endl;
			return INVALID_ERR;
		}
		VERIFIER* verifier = new VERIFIER(argv[2], argv[3]);
		verifier->getPuzzleID(argv[4]);
		bool result = verifier->verifyAllChallenges();
		cout << "Result of verification: " << std::boolalpha << result << endl;
		delete verifier;
		return FINE;
	}
	cerr << "Invalid command. try --help for more tips." << endl;
	return INVALID_ERR;
}
