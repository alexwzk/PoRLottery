#include "verifier.h"

VERIFIER::VERIFIER(std::string root_file, std::string tic_file) {
	using namespace std;
	ifstream inticket, inroot;
	try {
		inticket.open(tic_file);
	} catch (ifstream::failure& err) {
		cerr << err.what()
				<< " Open the ticket file @ Verifier 1st constructor." << endl;
		exit(FILE_ERR);
	}
	try {
		inroot.open(root_file);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " Open root file @ Verifier 1st constructor."
				<< endl;
		exit(FILE_ERR);
	}
	try {
		root_digestPt = new digest;
	} catch (std::bad_alloc& err) {
		std::cerr << err.what()
				<< " @ Verifier 1st constructor for root_digest." << std::endl;
		exit(MALLOC_ERR);
	}
	inroot.read((char *) root_digestPt, HASH_SIZE);
	// coutest root hex
//	cout << "root: ";
//	COMMON::printHex(root_digestPt, HASH_SIZE);
	inroot.close();

	try {
		tic_verifyPt = new TICKET;
	} catch (std::bad_alloc& err) {
		std::cerr << err.what()
				<< " malloc tic_verify @ Verifier 1st constructor."
				<< std::endl;
		exit(MALLOC_ERR);
	}
	inticket >> (*tic_verifyPt);
	inticket.close();
}

VERIFIER::~VERIFIER() {
	delete root_digestPt;
	for (auto it : tic_verifyPt->mkproofs) {
		delete it;
	}
	delete tic_verifyPt;
}

int VERIFIER::getPuzzleID(std::string puz_id) {
	puzzle_id = puz_id;
	return FINE;
}

bool VERIFIER::verifyAllChallenges() {
	using namespace std;
	using namespace COMMON;
	leaf empty_leaf;
	PATH* now_signPt, init_signPt;
	size_t r_i, chaleng_times;
	std::string prefix, inputs, uchar_str;
	chaleng_times = tic_verifyPt->mkproofs.size();
	prefix = puzzle_id + tic_verifyPt->pubkey;
	inputs = prefix + tic_verifyPt->seed;
	memset(empty_leaf, 0, LAMBDA);
	init_signPt = now_signPt = new PATH(empty_leaf);
	r_i = computeR_i(tic_verifyPt->pubkey, inputs, num_subset, num_all);

	//TODO Add write FPS root, validate paths
	for(int i = 0; i < i_ink; i++){

	}
	delete init_signPt;
	return true;
}
