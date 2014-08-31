#include "verifier.h"

VERIFIER::VERIFIER(digest root, std::string tic_file) {
	using namespace std;
	ifstream inticket;
	size_t buf_nsize = -1, buf_osize = -1, chalng_times = -1, path_len = -1;
	char* bufferPt = nullptr;
	leaf tmp_leaf;
	digest tmp_digest;
	PATH* pathPtr;
	try {
		inticket.open(tic_file);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at Verifier 2nd constructor."
				<< endl;
		exit(FILE_ERR);
	}
	root_digest = new digest;
	memcpy(root_digest, root, HASH_SIZE);
	cout << "root: ";
	COMMON::printHex(root_digest, HASH_SIZE);
	tic_verify = new TICKET;

	inticket.read((char *) &buf_nsize, sizeof(size_t));
	bufferPt = new char[buf_nsize];
	inticket.read(bufferPt, buf_nsize);
	tic_verify->pubkey.append(bufferPt, bufferPt + buf_nsize);
	cout << "pk: " << tic_verify->pubkey << endl;
	buf_osize = buf_nsize;
	inticket.read((char *) &buf_nsize, sizeof(size_t));
	if (buf_nsize > buf_osize) {
		delete[] bufferPt;
		bufferPt = new char[buf_nsize];
	}
	inticket.read(bufferPt, buf_nsize);
	tic_verify->seed.append(bufferPt, bufferPt + buf_nsize);
	cout << "sd: " << tic_verify->seed << endl;
	delete[] bufferPt;
	inticket.read((char *) &chalng_times, sizeof(size_t));
	cout << "ct: " << chalng_times << endl;
	for (size_t i = 0; i < chalng_times; i++) {
		cout << i << ": " << endl;
		inticket.read((char *) tmp_leaf, LEAF_SIZE);
		cout << "leaf: ";
		COMMON::printHex(tmp_leaf, LEAF_SIZE);
		pathPtr = new PATH(tmp_leaf);
		inticket.read((char *) &path_len, sizeof(size_t));
		for (size_t j = 0; j < path_len; j++) {
			inticket.read((char *) tmp_digest, HASH_SIZE);
			cout << "hash: ";
			COMMON::printHex(tmp_digest, HASH_SIZE);
			pathPtr->pushDigestPt(tmp_digest);
		}
		tic_verify->mkproofs.push_back(pathPtr);
	}
	inticket.close();
}

VERIFIER::~VERIFIER() {
	delete root_digest;
	for (auto it : tic_verify->mkproofs) {
		delete it;
	}
	delete tic_verify;
}

size_t VERIFIER::computesUElem(int k_i) {
	std::string str_ri, str_ui;
	digest hashv;
	int r_i = -1;
	str_ri = puzzle_id + tic_verify->pubkey + std::to_string(k_i) + tic_verify->seed;
	SHA1((uchar *)str_ri.data(),str_ri.size(),hashv);
	r_i = COMMON::hashToNumber(hashv) % num_subset;
	str_ui = tic_verify->pubkey + std::to_string(r_i);
	SHA1((uchar *)str_ui.data(),str_ui.size(),hashv);
	return COMMON::hashToNumber(hashv) % num_total;
}

bool VERIFIER::validatePath(PATH* p, size_t u_i) {
	size_t remain_ri = u_i;
	leaf pleaf;
	digest pdigest;
	uchar mkvalue[HASH_SIZE * 2];
	memcpy(pleaf, p->returnLeafPt(), LEAF_SIZE);
	SHA1(pleaf, LEAF_SIZE, pdigest);
	for (auto it : p->returnSiblings()) {
		if (COMMON::isEven(remain_ri)) {
			memcpy(mkvalue, pdigest, HASH_SIZE);
			memcpy(mkvalue + HASH_SIZE, it, HASH_SIZE);
		} else {
			memcpy(mkvalue, it, HASH_SIZE);
			memcpy(mkvalue + HASH_SIZE, pdigest, HASH_SIZE);
		}
		SHA1(mkvalue, HASH_SIZE * 2, pdigest);
		remain_ri /= 2;	//int division
	}
	if(strncmp((const char *) root_digest, (const char *) pdigest, HASH_SIZE) != 0){
		return false;
	}else{
		return true;
	}
}

bool VERIFIER::verifyAllChallenges(){
	int chlng_times = tic_verify->mkproofs.size();
	for(int i = 0; i < chlng_times; i++){
		if(!validatePath(tic_verify->mkproofs[i],computesUElem(i))){
			return false;
		}
	}
	return true;
}
