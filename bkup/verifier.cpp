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
	} catch (bad_alloc& err) {
		cerr << err.what() << "malloc root_digest @ Verifier 1st constructor."
				<< endl;
		exit(MALLOC_ERR);
	}
	inroot.read((char *) root_digestPt, HASH_SIZE);
	// coutest root hex
//	cout << "root: ";
//	COMMON::printHex(root_digestPt, HASH_SIZE);
	inroot.close();

	try {
		tic_verifyPt = new TICKET;
	} catch (bad_alloc& err) {
		cerr << err.what() << " malloc tic_verify @ Verifier 1st constructor."
				<< endl;
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

std::string VERIFIER::returnPuzzleID() {
	return puzzle_id;

}

bool VERIFIER::verifyAllChallenges() {
	using namespace std;
	using namespace pmc;

	bool passed = true;
	uint8_t empty_leaf[LAMBDA];
	digest hashvalue;
	RANDENGINE rand_engine;
	list<size_t> unrevealed_v;
	size_t r_i = 0, chaleng_times = 0, leaf_size = 0;
	string prefix, inputs, uchar_str;
	PATH *now_signPt = nullptr, *init_signPt = nullptr;
	uint8_t *leafPt = nullptr;

	chaleng_times = tic_verifyPt->mkproofs.size();
	for (size_t i = 0; i < (1 << HEIGHT_FPS); i++) {
		unrevealed_v.push_back(i);
	}

	uchar_str.assign((char*) tic_verifyPt->pubkey, HASH_SIZE);
	prefix = puzzle_id + uchar_str;

	//sigma_initial
	memset(empty_leaf, 0, LAMBDA);
	init_signPt = now_signPt = new PATH(empty_leaf, LAMBDA);
	//r_0
	inputs = prefix + tic_verifyPt->seed;
	r_i = computeR_i(tic_verifyPt->pubkey, inputs, num_subset, num_all);
	//coutest r_0
	cout << "Verify r_0: " << r_i << endl;
//	cout << stringToHex(inputs) << endl;
//	cout << "Compare: " << computeI_inL(inputs,num_subset) << endl;

	for (int i = 0; i < i_ink; i++) {
		//puz || pk || sigma_{i-1} || F[r_i]
		leafPt = tic_verifyPt->mkproofs[i]->releaseLeaf(leaf_size);
		uchar_str.assign((char*) leafPt, leaf_size);
		inputs = prefix + now_signPt->returnPathAsStr() + uchar_str;

		//Compute the signature's random seed
		SHA1((uint8_t*) inputs.data(), inputs.size(), hashvalue);

		//Verification
		passed &= FPS::verifySignature(tic_verifyPt->fps_signs[i], hashvalue,
		 tic_verifyPt->pubkey, unrevealed_v, rand_engine);
		passed &= MERKLE::verifyPath(tic_verifyPt->mkproofs[i], r_i,
				root_digestPt);
		if (!passed) {
			cout << "Failed the verification at the No. " << i << " round."
					<< endl;
			return passed;
		}

		//Compute r_{i+1}
		now_signPt = tic_verifyPt->fps_signs[i];
		inputs = prefix + now_signPt->returnPathAsStr();
		r_i = computeR_i(tic_verifyPt->pubkey, inputs, num_subset, num_all);
		//coutest
		cout << "Verify r_" << (i + 1) << ": " << r_i << endl;
//		cout << stringToHex(inputs) << endl;
//		cout << "Compare: " << computeI_inL(inputs,num_subset) << endl;
	}
	delete init_signPt;
	return passed;
}
