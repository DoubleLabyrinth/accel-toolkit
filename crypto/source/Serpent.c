/*********************************************************************
* Filename:   Serpent.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/

namespace accel::algorithm::crypto::Serpent {

	bool KeySchedule(const unsigned __int8 srcKey[], unsigned __int8 srcKeyLength, unsigned __int8 out_K[33][16]) {
		if (srcKeyLength > 32 || srcKeyLength == 0)
			return false;

		unsigned __int8 temp_Key[32] = { };
		for (int i = 0; i < srcKeyLength; ++i) 
			temp_Key[i] = srcKey[i];

		if (srcKeyLength != 32) 
			temp_Key[srcKeyLength] = 0x80;


	}

}