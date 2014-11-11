/*
 * ticket.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_TICKET_H_
#define PMC_TICKET_H_

#include <utilstrencodings.h>

#include "path.h"

template<unsigned int PMC_LFBYTES, unsigned int FPS_LFBYTES>
class TICKET {
public:
	uint256 pubkey;
	unsigned int seed;
	std::vector< PATH<PMC_LFBYTES> > mkproofs;
	std::vector< PATH<FPS_LFBYTES> > signatures;

	TICKET(){
		clear();
	}
	ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
			READWRITE(pubkey);
			READWRITE(seed);
			READWRITE(mkproofs);
			READWRITE(signatures);
    }

	void clear(){
		pubkey.SetHex("0");
		seed = 0;
		mkproofs.clear();
		mkproofs.reserve(CHALNG_CONST);
		for(int i = 0; i < CHALNG_CONST; i++){
			mkproofs[i].vhashes.reserve(RUN_PMCDEPTH);
		}
		signatures.clear();
		signatures.reserve(CHALNG_CONST);
		for(int i = 0; i < CHALNG_CONST; i++){
			signatures[i].vhashes.reserve(RUN_PMCDEPTH);
		}
	}

	std::string seedToString() const{
		return itostr(seed);
	}

	uint256 getHash() const{
		return SerializeHash(*this);
	}

	TICKET<PMC_LFBYTES, FPS_LFBYTES>& operator=(
			const TICKET<PMC_LFBYTES, FPS_LFBYTES>& t) {
		pubkey = t.pubkey;
		seed = t.seed;
		LogPrintf("copy mkproofs...\n");
		mkproofs = t.mkproofs;
		LogPrintf("copy signatures...\n");
		signatures = t.signatures;
		return (*this);
	}

};

#endif /* PMC_TICKET_H_ */
