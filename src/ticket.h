/*
 * ticket.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PRM_TICKET_H_
#define PRM_TICKET_H_

#include <utilstrencodings.h>

#include "path.h"

template<unsigned int FILE_LFBYTES, unsigned int FPS_LFBYTES>
class TICKET {
public:
	uint256 pubkey;
	unsigned int seed;
	std::vector< PATH<FILE_LFBYTES> > mkproofs;
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
		pubkey.SetNull();
		seed = 0;
		mkproofs.clear();
		signatures.clear();
	}

	std::string seedToString() const{
		return itostr(seed);
	}

	uint256 getHash() const{
		return SerializeHash(*this);
	}

	TICKET<FILE_LFBYTES, FPS_LFBYTES>& operator=(
			const TICKET<FILE_LFBYTES, FPS_LFBYTES>& t) {
		pubkey = t.pubkey;
		seed = t.seed;
		LogPrintf("copy mkproofs...\n");
		mkproofs = t.mkproofs;
		LogPrintf("copy signatures...\n");
		signatures = t.signatures;
		return (*this);
	}

};

#endif /* PRM_TICKET_H_ */
