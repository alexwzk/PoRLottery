#include "dealer.h"

DEALER::DEALER(std::string filenamePt) {
	using namespace std;
	//Parse the file and divide them into segments
	ifstream inputs;
	vector<uchar *> file_segmts;
	char* inbufferPt;
	try {
		inputs.open(filenamePt, ifstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at Dealer 1st constructor."
				<< endl;
	}
	while (inputs) {
		inbufferPt = new char[LEAF_SIZE];
		inputs.read(inbufferPt, LEAF_SIZE);
		file_segmts.push_back((uchar *) inbufferPt);
	}
	if (!inputs.eof()) {
		//File size can't be divided by the LEAF_SIZE
		inbufferPt = new char[LEAF_SIZE];
		inputs.read(inbufferPt, inputs.gcount());
		for (int i = inputs.gcount(); i < LEAF_SIZE; i++) {
			inbufferPt[i] = 0;
		}
		file_segmts.push_back((uchar *) inbufferPt);
	}

	//Pass the processed segments to the Merkle tree object
	//The Merkle tree will make a new copy of the passed-in segments
	mktreePt = new MERKLE(file_segmts);

	//Delete the memory of file_segmts
	for (auto it : file_segmts) {
		delete[] it;
	}

}
int DEALER::createSubset() {
	if (!can_update) {
		return INVALID_ERR;
	}

	std::string tmp_str;
	digest hashvalue;

	uarray_pk.clear();
	// Hash(pk||i)
	for (size_t i = 0; i < num_subset; i++) {
		tmp_str = pubkey + std::to_string(i);
		std::cout << "The pk||i : " << tmp_str << std::endl;
		SHA1((uchar *) tmp_str.data(), tmp_str.size(), hashvalue);
		//Extract small number from the hash
		uarray_pk.push_back(COMMON::hashToNumber(hashvalue) % num_all);
		//Coutest generate random number within num_all
		std::cout << "The random number is : "
				<< COMMON::hashToNumber(hashvalue) << " mod by " << num_all
				<< " turns out: " << COMMON::hashToNumber(hashvalue) % num_all
				<< std::endl;
	}

	//Coutest subset_pk
	std::cout << "The size of subset_{pk}: " << uarray_pk.size() << std::endl;
	for (auto i : uarray_pk) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	can_update = false;
	return 0;
}

std::vector<PATH*> DEALER::createSource(std::string usr_pubkey) {
	if (pubkey.compare(usr_pubkey) != 0) {
		pubkey = usr_pubkey;
		can_update = true;
		createSubset();
		pathPts.clear();
		for (auto it : uarray_pk) {
			pathPts.push_back(mktreePt->buildPath(it));
		}
	}
	return pathPts;
}

DEALER::~DEALER() {
	delete mktreePt;
}
