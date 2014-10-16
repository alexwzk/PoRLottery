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
//				 their parts of to-be-stored data.
//
//============================================================================

/*
 #include "src/dealer.h"
 #include "src/user.h"
 #include "src/verifier.h"
 #include "src/fps.h"
 */

#include "src/fps.h"
#include "src/ticket.h"

#define LFSIZE 10
int main(int argc, const char *argv[]) {
	using namespace std;
	BUFFER<LFSIZE> tbuff;
	uint8_t d[LFSIZE];
	for (int i = 0; i < LFSIZE; i++) {
		d[i] = 'a' + i;
	}
	tbuff.assign(d, LFSIZE);
	/*tbuff.Serialize(cout,SER_DISK,CLIENT_VERSION);
	 cout << "\n";*/

	/*uint160 thash;
	PATH<LFSIZE, uint160> tpath;
	 thash.SetHex((char*)d);
	 tpath.setLeafValue(tbuff);
	 tpath.pushHashDigest(thash);
	 tpath.Serialize(cout,SER_DISK,CLIENT_VERSION); // path serialisation works!*/

	std::vector<BUFFER<LFSIZE>> tsegmts;
	for (int i = 0; i < 4; i++) {
		tsegmts.push_back(tbuff);
	}
	PATH<LFSIZE, uint160> tpath;
	MERKLE<LFSIZE, uint160> tmerkle(tsegmts);
	tpath = tmerkle.returnPath(0);
	uint160 troot = tmerkle.returnRoot();
	cout << boolalpha
			<< MERKLE<LFSIZE, uint160>::verifyPath(tpath, 0, troot)
			<< endl; /*successfully verified the path!*/

	FPS<uint160> tfps(16,1,1);
	PATH<FPS_LEAFSIZE,uint160> tsign;
	RANDENGINE<uint160> trand;
	std::list<size_t> tunrevealed;
	for(int i = 0; i < 16; i++){
		tunrevealed.push_back(i);
	}
	uint160 trootfps = tfps.returnPubkey();
	tsign = tfps.returnSign(trootfps);
	bool result = FPS<uint160>::verifySignature(tsign,trootfps,trootfps,tunrevealed,trand);
	cout << boolalpha << result << endl; /*Verify Signature done!*/

	TICKET<LFSIZE,uint160> ttic;
	ttic.pubkey = troot;
	ttic.seed = "zikaiwen";
	ttic.mkproofs.push_back(tpath);
	ttic.signatures.push_back(tsign);
	pmc::printHex(ttic.hashOfTicket().begin(),20); /*Ticket is set!*/

	/*	if (argc < 2) {
	 cerr << "Invalid command. try --help for more tips." << endl;
	 return INVALID_ERR;
	 }
	 if (strcmp(argv[1], "--help") == SAME) {
	 cout << "You are testing the simple PoR lottery programme. "
	 << "Welcome to the help manual:" << endl;
	 cout
	 << "cmd1: --interact \"database filepath\" \"outsourced data filepath\" "
	 << "\"puzzle id string\" \"release ticket filepath\" "
	 << "-r \"file receiving root digest\"" << endl;
	 cout
	 << "The User asks for an outsorced file and then generates a ticket."
	 << "optional command -r following with a file path will ask the dealer to "
	 << "release its database's root digest. " << endl;
	 cout
	 << "cmd2: --verify \"root digest file\" \"ticket filepath\" \"puzzle id string\" "
	 << endl;
	 cout << "The 2nd command verifies a released ticket." << endl;
	 return FINE;
	 } else if (strcmp(argv[1], "--interact") == SAME) {
	 if (argc == 6) {
	 USER user;
	 DEALER* dealer = new DEALER(argv[2]);
	 //TODO write user's pubkey to file
	 dealer->outSource(user.returnMyPubkey(), argv[3]);
	 user.storeFile(argv[3]);
	 user.getNewPuzzle(argv[4]);
	 user.writeTicket(argv[5]);
	 delete dealer;
	 return FINE;
	 } else if (argc == 8 && strcmp(argv[6], "-r") == SAME) {
	 USER user;
	 DEALER* dealer = new DEALER(argv[2]);
	 dealer->outSource(user.returnMyPubkey(), argv[3]);
	 user.storeFile(argv[3]);
	 user.getNewPuzzle(argv[4]);
	 user.writeTicket(argv[5]);
	 dealer->writeRoot(argv[7]);
	 delete dealer;
	 return FINE;
	 }
	 cerr << "Invalid number of input strings for the setup mode." << endl;
	 return INVALID_ERR;
	 } else if (strcmp(argv[1], "--verify") == SAME) {
	 if (argc != 5) {
	 cerr << "Invalid number of input strings to initiate the verifier."
	 << endl;
	 return INVALID_ERR;
	 }
	 VERIFIER* verifier = new VERIFIER(argv[2], argv[3]);
	 verifier->getPuzzleID(argv[4]);
	 bool result = verifier->verifyAllChallenges();
	 cout << "Result of verification: " << boolalpha << result << endl;
	 delete verifier;
	 return FINE;
	 } else if (strcmp(argv[1], "--test") == SAME) {
	 return FINE;
	 }
	 cerr << "Invalid command. try --help for more tips." << endl;
	 return INVALID_ERR;*/
}
