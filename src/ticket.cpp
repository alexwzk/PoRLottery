/*
 * ticket.cpp
 *
 *  Created on: Sep 27, 2014
 *      Author: zkwen
 */

#include "ticket.h"

uint8_t* TICKET::hashOfTicket() {
	uint8_t* hashvaluePt = new digest, *leafPt = nullptr;
	size_t length_count = 0, leaf_size = 0, path_size = 0;
	length_count += HASH_SIZE;
	length_count += this->seed.size();
	path_size = LEAF_SIZE
			+ mkproofs.at(HEAD)->returnSiblings().size() * HASH_SIZE; // each sibling should have some # of siblings
	length_count += path_size * mkproofs.size();
	path_size =
			LAMBDA + fps_signs.at(HEAD)->returnSiblings().size() * HASH_SIZE;
	length_count += path_size * fps_signs.size();
	uint8_t* buffer = new uint8_t[length_count];

	length_count = 0;
	memcpy(buffer + length_count, this->pubkey, HASH_SIZE);
	length_count += HASH_SIZE;
	// coutest pubkey hex
//	COMMON::printHex(buffer, length_count);
	memcpy(buffer + length_count, this->seed.data(), SEED_LENGTH);
	length_count += SEED_LENGTH;
	// coutest seed hex
	pmc::printHex(buffer, length_count);
	pathsToBuffer(mkproofs, buffer, length_count);
	pathsToBuffer(fps_signs, buffer, length_count);
	SHA1(buffer, length_count, hashvaluePt);
	return hashvaluePt;
}

int TICKET::pathsToBuffer(std::vector<PATH*> paths, uint8_t* buffer,
		size_t &length_count) {
	size_t leaf_size = 0;
	uint8_t* leafPt;
	for (PATH* pathPt : paths) {
		leafPt = pathPt->releaseLeaf(leaf_size);
		memcpy(buffer + length_count, (const uint8_t*) leafPt, leaf_size);
		length_count += leaf_size;
		// coutest leaf hex
//		COMMON::printHex(buffer, length_count);
		for (uint8_t* nodePt : pathPt->returnSiblings()) {
			memcpy(buffer + length_count, nodePt, HASH_SIZE);
			length_count += HASH_SIZE;
			// coutest siblings
//			COMMON::printHex(buffer, length_count);
		}
	}
	return FINE;
}

std::ostream& operator<<(std::ostream &out_obj, TICKET &tic) {
	using namespace std;
	using namespace pmc;
	size_t tmp_size = 0, leaf_size = 0;
	uint8_t* leafPt = nullptr;

	out_obj.write((const char *) tic.pubkey, HASH_SIZE);
	// coutest tic.pubkey
	cout << "write tic pubkey: ";
	printHex(tic.pubkey, HASH_SIZE);

	out_obj.write(tic.seed.data(), SEED_LENGTH);
	// coutest tic.seed
	cout << "write tic seed: ";
	printHex((const uint8_t*) tic.seed.data(), SEED_LENGTH);

	tmp_size = tic.mkproofs.size();
	out_obj.write((const char *) &tmp_size, sizeof(size_t));
	for (PATH* it : tic.mkproofs) {
		leafPt = it->releaseLeaf(leaf_size);
		out_obj.write((const char *) leafPt, leaf_size);
		tmp_size = it->returnSiblings().size();
		out_obj.write((const char *) &tmp_size, sizeof(size_t));
		for (uint8_t* jt : it->returnSiblings()) {
			out_obj.write((const char *) jt, HASH_SIZE);
		}
	}

	tmp_size = tic.fps_signs.size();
	// coutest # of fps_sign
	//cout << "# of fps_sign: " << tmp_size << endl;
	out_obj.write((const char *) &tmp_size, sizeof(size_t));
	for (PATH* it : tic.fps_signs) {
		leafPt = it->releaseLeaf(leaf_size);
		out_obj.write((const char *) leafPt, leaf_size);
		tmp_size = it->returnSiblings().size();
		out_obj.write((const char *) &tmp_size, sizeof(size_t));
		for (uint8_t* jt : it->returnSiblings()) {
			out_obj.write((const char *) jt, HASH_SIZE);
		}
	}
	return out_obj;
}

std::istream& operator>>(std::istream &in, TICKET & tic) {
	using namespace std;
	using namespace pmc;
	cout << "Reading in ticket file." << endl;

	size_t num_paths = -1, path_len = -1;
	leaf tmp_leaf;
	uint8_t fps_leaf[LAMBDA];
	digest tmp_digest;
	char seed_buffer[SEED_LENGTH];
	PATH* pathPt = nullptr;

	in.read((char*) tic.pubkey, HASH_SIZE);
	// coutest pubkey
	cout << "public key: ";
	printHex(tic.pubkey, HASH_SIZE);
	in.read(seed_buffer, SEED_LENGTH);
	tic.seed.assign(seed_buffer, SEED_LENGTH);
	// coutest seed
	cout << "seed: ";
	cout << stringToHex(tic.seed) << endl;

	in.read((char *) &num_paths, sizeof(size_t));
	for (size_t i = 0; i < num_paths; i++) {
		in.read((char *) tmp_leaf, LEAF_SIZE);
		try {
			pathPt = new PATH(tmp_leaf, LEAF_SIZE);
		} catch (bad_alloc& err) {
			cerr << err.what() << " malloc pathPt @  Verifier 1st constructor ."
					<< endl;
			exit(MALLOC_ERR);
		}
		in.read((char *) &path_len, sizeof(size_t));
		for (size_t j = 0; j < path_len; j++) {
			in.read((char *) tmp_digest, HASH_SIZE);
			pathPt->pushDigestPt(tmp_digest);
		}
		tic.mkproofs.push_back(pathPt);
		//coutest mkproof
		/*cout << "leaf: ";
		 printHex(pathPt->releaseLeafPt(), LEAF_SIZE);
		 cout << "proofs: " << endl;
		 int counter = 0;
		 for(uchar* it : pathPt->returnSiblings()){
		 cout << "layer " << counter++ << ":";
		 printHex(it, HASH_SIZE);
		 }*/
	}

	in.read((char *) &num_paths, sizeof(size_t));
	cout << "num of fps signature: " << num_paths << endl;
	for (size_t i = 0; i < num_paths; i++) {
		in.read((char *) fps_leaf, LAMBDA);
		try {
			pathPt = new PATH(fps_leaf, LAMBDA);
		} catch (bad_alloc& err) {
			cerr << err.what()
					<< " malloc pathPt(2) @  Verifier 1st constructor ."
					<< endl;
			exit(MALLOC_ERR);
		}
		in.read((char *) &path_len, sizeof(size_t));
		for (size_t j = 0; j < path_len; j++) {
			in.read((char *) tmp_digest, HASH_SIZE);
			pathPt->pushDigestPt(tmp_digest);
		}
		tic.fps_signs.push_back(pathPt);
		//coutest fps scheme
		/*cout << "fps leaf: ";
		 printHex(pathPt->releaseLeafPt(), LAMBDA);
		 cout << "fps seckey: " << endl;
		 int counter = 1;
		 for(uchar* it : pathPt->returnSiblings()){
		 cout << "layer " << counter++ << ":";
		 printHex(it, HASH_SIZE);
		 }*/
	}
	cout << "Reading ticket file ends here. " << endl;
	return in;
}
