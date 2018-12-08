import math
import hashlib


class MD5:

    y = []
    # CV 的迭代直接用ABCD的迭代来表示即可
    A = 0x67452301
    B = 0xEFCDAB89
    C = 0x98BADCFE
    D = 0x10325476
    GroupNUM = 0
    S = [7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
         5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
         4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
         6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21]

    T = [0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
         0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
         0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
         0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
         0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
         0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
         0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
         0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
         0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
         0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
         0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
         0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
         0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
         0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
         0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
         0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391]

    # CV = []

    '''
    类变量，类中共享
    '''

    def __init__(self, s):
        self.cut(s)
        pass

    def tobits(self, s):
        '''
        :param s: string
        :return: string
        ord: ASCII
        convert string to 01 string
        '''
        result = ""
        for c in s:
            bits = bin(ord(c))[2:]
            bits = '00000000'[len(bits):] + bits  # 补全8位
            result = result + bits

        return result

    def frombits(self, bits):
        '''
        :param bits: string
        :return: string
        not used in this project
        '''
        chars = []
        for b in range(int(len(bits) / 8)):
            byte = bits[b * 8:(b + 1) * 8]
            chars.append(chr(int(''.join([str(bit) for bit in byte]), 2)))
        return ''.join(chars)

    def padding(self, s):
        '''
        :param s: string
        :return: string after padding and add tail
        '''
        bits = self.tobits(s)
        padnum = 448 - len(bits) % 512
        if padnum <= 0:
            padnum = padnum + 512
        padbit = bits + '1' + '0' * (padnum - 1)
        addtail = padbit + padbit[0:64]
        return addtail

    def cut(self, s):
        '''

        :param s: string
        :return y: string array
        '''
        pads = self.padding(s)
        MD5.GroupNUM = int(len(pads) / 512)
        for i in range(MD5.GroupNUM):
            MD5.y.append(pads[i * 512: (i + 1) * 512])

    def F(self, b, c, d):
        '''
        :param b: int(base = 2)
        :param c: int(base = 2)
        :param d: int(base = 2)
        :return:
        '''
        return (b & c) | (~b & d)

    def G(self, b, c, d):
        return (b & d) | (c & ~d)

    def H(self, b, c, d):
        return b ^ c ^ d

    def I(self, b, c, d):
        return c ^ (b | ~d)

    FUNC = [F, G, H, I]


    def HDM5(self, Y):
        '''

        :param Y: y[i]
        :return:
        '''
        X = []
        for i in range(16):
            X.append(Y[32 * i: 32 * (i + 1)])  # 512bit Y 切分为 16 个 32bit X

        for i in range(4):
            f = MD5.FUNC[i]
            for j in range(16):
                if i == 0:
                    k = j
                elif i == 1:
                    k = (1 + 5 * j) % 16
                elif i == 2:
                    k = (5 + 3 * j) % 16
                else:
                    k = (7 * j) % 16
                # t = MD5.T[i * 16 + j]
                # r = MD5.A + f(self, MD5.B, MD5.C, MD5.D) + int(X[k], 2) + MD5.T[i * 16 + j]
                # nr = r % pow(2, 32)
                MD5.A = MD5.B + ((MD5.A + f(self, MD5.B, MD5.C, MD5.D) + int(X[k], 2) + MD5.T[i * 16 + j]) << MD5.S[i * 16 + j])
                MD5.A = MD5.A % pow(2, 32)
                temp = MD5.A
                MD5.A = MD5.D
                MD5.D = MD5.C
                MD5.C = MD5.B
                MD5.B = temp

    def getResult(self):
        for i in range(MD5.GroupNUM):
            self.HDM5(MD5.y[i])

        result = bin(MD5.A).__str__()[2:] + bin(MD5.B).__str__()[2:] + bin(MD5.C).__str__()[2:] + bin(MD5.D).__str__()[2:]

        return result


if __name__ == "__main__":
    string = 'hbxhhh'
    md5 = MD5(string)
    md5.getResult()
    print(hex(MD5.A))
    print(hex(MD5.B))
    print(hex(MD5.C))
    print(hex(MD5.D))


