#include "dealer.h"

DEALER::DEALER(std::string infile) {
	using namespace std;
	//Parse the file and divide them into segments
	ifstream inputs;
	vector<uchar *> file_segmts;
	char* inbufferPt = nullptr;
	try {
		inputs.open(infile, ifstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " Opening file @ Dealer 1st constructor."
				<< endl;
		exit(FILE_ERR);
	}

	while (inputs) {
		try {
			inbufferPt = new char[LEAF_SIZE];
		} catch (bad_alloc& err) {
			cerr << err.what()
					<< " assigning memory to the inbufferPt @ Dealer 1st constructor"
					<< endl;
			exit(MALLOC_ERR);
		}
		inputs.read(inbufferPt, LEAF_SIZE);
		file_segmts.push_back((uchar *) inbufferPt);
	}

	if (!inputs.eof()) {
		//File size can't be divided by the LEAF_SIZE
		try {
			inbufferPt = new char[LEAF_SIZE];
		} catch (bad_alloc& err) {
			cerr << err.what()
					<< " assigning memory to the inbufferPt(2) @ Dealer 1st constructor"
					<< endl;
			exit(MALLOC_ERR);
		}
		inputs.read(inbufferPt, inputs.gcount());
		for (int i = inputs.gcount(); i < LEAF_SIZE; i++) {
			inbufferPt[i] = 0;
		}
		file_segmts.push_back((uchar *) inbufferPt);
	}

	inputs.close();

	//Pass the processed segments to the Merkle tree object
	//The Merkle tree will make a new copy of the passed-in segments
	try {
		mktreePt = new MERKLE(file_segmts, LEAF_SIZE);
	} catch (bad_alloc& err) {
		cerr << err.what()
				<< " assigning memory to the mktreePt @ Dealer 1st constructor"
				<< endl;
		exit(MALLOC_ERR);
	}

	//Delete the memory of file_segmts
	for (auto it : file_segmts) {
		delete[] it;
	}
}

DEALER::~DEALER() {
	delete mktreePt;
}

int DEALER::createSubset() {
	uarray_pk.clear();
	for (size_t i = 0; i < num_subset; i++) {
		uarray_pk.push_back(COMMON::computeU_i(pubkey, i, num_all));
	}

	//coutest subset_pk
	std::cout << "The size of subset_{pk}: " << uarray_pk.size() << std::endl;
	std::cout << "The U_i: ";
	for (auto i : uarray_pk) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	return FINE;
}

int DEALER::outSource(digest usr_pubkey, std::string outfile) {
	using namespace std;
	ofstream outpaths;
	PATH* pathPt = nullptr;
	uchar* leafPt = nullptr;
	size_t tmp_size = 0, leaf_size = 0;

	memcpy(pubkey,usr_pubkey,HASH_SIZE);
	try {
		outpaths.open(outfile, ofstream::trunc | ofstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at Dealer outSource." << endl;
		exit(FILE_ERR);
	}

	createSubset();

	tmp_size = uarray_pk.size();
	outpaths.write((const char*) &tmp_size, sizeof(tmp_size));
	for (auto it : uarray_pk) {
		pathPt = mktreePt->newPath(it);
		leafPt = pathPt->releaseLeaf(leaf_size);
		outpaths.write((const char*) leafPt, leaf_size);
		tmp_size = pathPt->returnSiblings().size();
		outpaths.write((const char*) &tmp_size, sizeof(tmp_size));
		for (auto it : pathPt->returnSiblings()) {
			outpaths.write((const char*) it, HASH_SIZE);
		}
		delete pathPt;
	}

	outpaths.close();
	return FINE;
}

int DEALER::writeRoot(std::string outfile) {
	using namespace std;
	ofstream ofile_operat;
	uchar* rootdigestPt = mktreePt->releaseRootPt();	//Method Delegate

	//coutest root digest hex
	cout << "The root digest is: ";
	COMMON::printHex(rootdigestPt, HASH_SIZE);
	try {
		ofile_operat.open(outfile, ofstream::trunc | ofstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at Dealer releaseRoot." << endl;
		exit(FILE_ERR);
	}
	cout << "The root digest will be exported to file: " << outfile << endl;

	ofile_operat.write((const char *) rootdigestPt, sizeof(digest));
	ofile_operat.close();
	return FINE;
}
