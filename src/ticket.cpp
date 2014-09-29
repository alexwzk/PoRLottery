/*
 * ticket.cpp
 *
 *  Created on: Sep 27, 2014
 *      Author: zkwen
 */

#include "ticket.h"

uchar* TICKET::hashOfTicket() {
	uchar* hashvaluePt = new digest;
	size_t length_count = 0;
	length_count += this->pubkey.size();
	length_count += this->seed.size();
	size_t size_path = LEAF_SIZE
			+ mkproofs.at(HEAD)->returnSiblings().size() * HASH_SIZE; // each sibling should have some # of siblings
	length_count += size_path * mkproofs.size();
	size_path =
	LAMBDA + fps_signs.at(HEAD)->returnSiblings().size() * HASH_SIZE;
	length_count += size_path * fps_signs.size();
	uchar* buffer = new uchar[length_count];

	length_count = 0;
	memcpy(buffer + length_count, this->pubkey.data(), this->pubkey.size());
	length_count += this->pubkey.size();
	// coutest pubkey hex
//	COMMON::printHex(buffer, length_count);
	memcpy(buffer + length_count, this->seed.data(), this->seed.size());
	length_count += this->seed.size();
	// coutest seed hex
//	COMMON::printHex(buffer, length_count);
	for (PATH* mkproofPt : mkproofs) {
		memcpy(buffer + length_count, mkproofPt->releaseLeafPt(), LEAF_SIZE);
		length_count += LEAF_SIZE;
		// coutest leaf hex
//		COMMON::printHex(buffer, length_count);
		for (uchar* nodePt : mkproofPt->returnSiblings()) {
			memcpy(buffer + length_count, nodePt, HASH_SIZE);
			length_count += HASH_SIZE;
			// coutest Merkle proofs hex
//			COMMON::printHex(buffer, length_count);
		}
	}
	for (PATH* seckeyPt : fps_signs) {
		memcpy(buffer + length_count, seckeyPt->releaseLeafPt(), LAMBDA);
		length_count += LAMBDA;
		// coutest seckey leaf hex
//		COMMON::printHex(buffer, length_count);
		for (uchar* nodePt : seckeyPt->returnSiblings()) {
			memcpy(buffer + length_count, nodePt, HASH_SIZE);
			length_count += HASH_SIZE;
			// coutest seckey nodes hex
//			COMMON::printHex(buffer, length_count);
		}
	}
	SHA1(buffer, length_count, hashvaluePt);
	return hashvaluePt;
}

std::ostream& operator<<(std::ostream &out_obj, TICKET &tic) {
	using namespace std;
	using namespace COMMON;
	size_t tmp_size;
	tmp_size = tic.pubkey.size();
	out_obj.write((const char *) &tmp_size, sizeof(size_t));
	out_obj.write(tic.pubkey.data(), tmp_size);
	// coutest tic.pubkey
	cout << "write tic pubkey: ";
	printHex((const uchar*)tic.pubkey.data(),HASH_SIZE);
	cout << endl;
	tmp_size = tic.seed.size();
	out_obj.write((const char *) &tmp_size, sizeof(size_t));
	out_obj.write(tic.seed.data(), tmp_size);
	//coutest tic.seed
	cout << "write tic seed: ";
	printHex((const uchar*) tic.seed.data(),SEED_LENGTH);
	cout << endl;
	tmp_size = tic.mkproofs.size();
	out_obj.write((const char *) &tmp_size, sizeof(size_t));
	for (PATH* it : tic.mkproofs) {
		out_obj.write((const char *) it->releaseLeafPt(), LEAF_SIZE);
		tmp_size = it->returnSiblings().size();
		out_obj.write((const char *) &tmp_size, sizeof(size_t));
		for (uchar* jt : it->returnSiblings()) {
			out_obj.write((const char *) jt, HASH_SIZE);
		}
	}

	tmp_size = tic.fps_signs.size();
	//coutest # of fps_sign
	cout << "# of fps_sign: " << tmp_size << endl;
	out_obj.write((const char *) &tmp_size, sizeof(size_t));
	for (PATH* it : tic.fps_signs) {
		out_obj.write((const char *) it->releaseLeafPt(), LAMBDA);
		tmp_size = it->returnSiblings().size();
		out_obj.write((const char *) &tmp_size, sizeof(size_t));
		for (uchar* jt : it->returnSiblings()) {
			out_obj.write((const char *) jt, HASH_SIZE);
		}
	}
	return out_obj;
}

std::istream& operator>>(std::istream &in, TICKET & tic) {
	using namespace std;
	using namespace COMMON;
	cout << "Reading in ticket file." << endl;
	size_t buf_nsize = -1 /*new buffer size*/,
			buf_osize = -1 /*old buffer size*/, num_paths = -1, path_len = -1;
	leaf tmp_leaf;
	digest tmp_digest;
	char* bufferPt = nullptr;
	PATH* pathPt = nullptr;
	in.read((char *) &buf_nsize, sizeof(size_t));
	try {
		bufferPt = new char[buf_nsize];
	} catch (bad_alloc& err) {
		cerr << err.what()
				<< " malloc bufferPt @ Verifier 1st constructor." << endl;
		exit(MALLOC_ERR);
	}
	in.read(bufferPt, buf_nsize);
	tic.pubkey.assign(bufferPt, bufferPt + buf_nsize);
	// coutest pubkey
	cout << "public key: ";
	printHex((const uchar*) tic.pubkey.data(),HASH_SIZE);
	cout << endl;
	buf_osize = buf_nsize;
	in.read((char *) &buf_nsize, sizeof(size_t));
	if (buf_nsize > buf_osize) {
		delete[] bufferPt;
		try {
			bufferPt = new char[buf_nsize];
		} catch (bad_alloc& err) {
			cerr << err.what()
					<< " malloc bufferPt(2) @ Verifier 1st constructor."
					<< endl;
			exit(MALLOC_ERR);
		}
	}
	in.read(bufferPt, buf_nsize);
	tic.seed.assign(bufferPt, bufferPt + buf_nsize);
	// coutest seed
	cout << "seed: ";
	printHex((const uchar*) tic.seed.data(), SEED_LENGTH);
	cout << endl;
	in.read((char *) &num_paths, sizeof(size_t));
	for (size_t i = 0; i < num_paths; i++) {
		in.read((char *) tmp_leaf, LEAF_SIZE);
		try {
			pathPt = new PATH(tmp_leaf);
		} catch (bad_alloc& err) {
			cerr << err.what()
					<< " malloc pathPt @  Verifier 1st constructor ."
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
		cout << "leaf: ";
		printHex(pathPt->releaseLeafPt(), LEAF_SIZE);
		cout << "proofs: " << endl;
		int counter = 0;
		for(uchar* it : pathPt->returnSiblings()){
			cout << "layer " << counter++ << ":";
			printHex(it, HASH_SIZE);
		}
	}

	in.read((char *) &num_paths, sizeof(size_t));
	cout << "num of fps signature: " << num_paths << endl;
	for (size_t i = 0; i < num_paths; i++) {
		in.read((char *) tmp_leaf, LAMBDA);
		try {
			pathPt = new PATH(tmp_leaf);
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
		cout << "fps leaf: ";
		printHex(pathPt->releaseLeafPt(), LAMBDA);
		cout << "fps seckey: " << endl;
		int counter = 1;
		for(uchar* it : pathPt->returnSiblings()){
			cout << "layer " << counter++ << ":";
			printHex(it, HASH_SIZE);
		}
	}
	cout << "Reading ticket file ends here. " << endl;
	delete[] bufferPt;
	return in;
}
