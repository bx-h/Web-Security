#include "Des.h"
using namespace std;

int main() {
	string s = "helloHBX";
	Des *des = new Des;
	bitset<64> plain = des->charToBitset(s.c_str());
	cout << "origin: \n" << plain << endl;
	des->generateKeys();
	bitset<64> cipher = des->encrypt(plain);
	cout << "cipher: \n" << cipher << endl;
	bitset<64> plain2 = des->decrypt(cipher);
	cout << "decrypt from cypher: \n"<< plain2 << endl;
	return 0;
}