#include "Des.h"
using namespace std;

/*-------- f函数-------------*/
bitset<48> Des::expand(bitset<32>R) {
	bitset<48> tmp;
	for (int i = 0; i < 48; ++i) {
		tmp[i] = R[E[i]-1];
	}
	return tmp;
}

bitset<32> Des::sBox(bitset<48>R) {
	bitset<32>output;
	int x = 0;
	for (int i = 0; i < 48; i = i + 6) {
		int row = R[i] * 2 + R[i + 5];
		int col = R[i + 1] * 8 + R[i + 2] * 4 + R[i + 3] * 2 + R[i + 4];
		int num = S_BOX[i/6][row][col];
		bitset<4> binary(num);
		output[x] = binary[0];
		output[x + 1] = binary[1];
		output[x + 2] = binary[2];
		output[x + 3] = binary[3];
		x = x + 4;
	}
	return output;

}

bitset<32> Des::pConvert(bitset<32>R) {
	bitset<32>tmp;
	for (int i = 0; i < 32; ++i) {
		tmp[i] = R[P[i] - 1];
	}
	return tmp;
}

bitset<32> Des::f(bitset<32>R, bitset<48>k) {
	bitset<48> expandR = expand(R);
	expandR = expandR ^ k;
	bitset<32> output = sBox(expandR);
	bitset<32> result = pConvert(output);
	return result;
}


/*------产生子密钥---------*/


bitset<56> Des::pc_1Convert(bitset<64>R) {
	bitset<56>tmp;
	for (int i = 0; i < 56; ++i) {
		tmp[i] = R[PC_1[i]-1];
	}
	return tmp;
}

bitset<48> Des::pc_2Convert(bitset<56>R) {
	bitset<48>tmp;
	for (int i = 0; i < 48; ++i) {
		tmp[i] = R[PC_2[i]-1];
	}
	return tmp;
}

bitset<28> Des::leftShift(bitset<28> k, int shift) {
	bitset<28> tmp = k;
	if (shift == 2) {
		tmp <<= 2;
		tmp[0] = k[26];
		tmp[1] = k[27];
	}
	else if (shift == 1) {
		tmp <<= 1;
		tmp[0] = k[27];
	}
	return tmp;

}

void Des::generateKeys() {
	bitset<56> realkey = pc_1Convert(key);
	bitset<28> left;
	bitset<28> right;

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 28; ++j) {
			left[j] = realkey[j];
		}
		for (int j = 28; j < 56; ++j) {
			right[j - 28] = realkey[j];
		}

		left = leftShift(left, shiftBits[i]);
		right = leftShift(right, shiftBits[i]);

		bitset<56>tmp;
		for (int k = 0; k < 28; ++k) {
			tmp[k] = left[k];
		}
		for (int k = 28; k < 56; ++k) {
			tmp[k] = right[k - 28];
		}

		subKey[i] = pc_2Convert(tmp);

	}

}



/*------------加密-------------*/

bitset<64> Des::ipConvert(bitset<64>R) {
	bitset<64> tmp;
	for (int i = 0; i < 64; ++i) {
		tmp[i] = R[IP[i]-1];
	}
	return tmp;
}

bitset<64> Des::ip_1Convert(bitset<64>R) {
	bitset<64> tmp;
	for (int i = 0; i < 64; ++i) {
		tmp[i] = R[IP_1[i]-1];
	}
	return tmp;
}

bitset<64> Des::encrypt(bitset<64>& plain) {
	bitset<64> current = ipConvert(plain);
	bitset<32> left;  //L0
	bitset<32> right;  //R0 
	bitset<32> tmp;
	bitset<64> output;
	bitset<64> cipher;
	//L0R0
	for (int i = 0; i < 32; ++i) {
		left[i] = current[i];
	}
	for (int i = 32; i < 64; ++i)
	{
		right[i - 32] = current[i];
	}
	// 迭代
	for (int round = 0; round < 16; ++round) {
		tmp = right;
		right = left ^ f(right, subKey[round]);
		left = tmp;
	}
	// R0L0
	for (int i = 0; i < 32; ++i) {
		output[i] = right[i];
	}
	for (int i = 32; i < 64; ++i) {
		output[i] = left[i - 32];
	}

	//逆置换
	cipher = ip_1Convert(output);

	return cipher;
}

bitset<64> Des::decrypt(bitset<64>& cipher) {
	bitset<64> current = ipConvert(cipher);
	bitset<32> left;  //L0
	bitset<32> right;  //R0 
	bitset<32> tmp;
	bitset<64> output;
	bitset<64> plain;
	//L0R0
	for (int i = 0; i < 32; ++i) {
		left[i] = current[i];
	}
	for (int i = 32; i < 64; ++i)
	{
		right[i - 32] = current[i];
	}
	// 迭代
	for (int round = 0; round < 16; ++round) {
		tmp = right;
		right = left ^ f(right, subKey[15-round]);
		left = tmp;
	}
	// R0L0
	for (int i = 0; i < 32; ++i) {
		output[i] = right[i];
	}
	for (int i = 32; i < 64; ++i) {
		output[i] = left[i - 32];
	}

	//逆置换
	plain = ip_1Convert(output);

	return plain;

}

bitset<64> Des::charToBitset(const char s[8]) {
	bitset<64> bits;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			bits[i * 8 + j] = ((s[i] >> j) & 1);
		}
	}
	return bits;
}

