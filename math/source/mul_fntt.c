#include "../arithmetic.h"
#include "../def_asm.h"
#include "../num_theory.h"
#include <memory.h>

typedef union __3coeff_t {
    struct {
        coeff_t l;
        coeff_t m;
        coeff_t h;
    };
    coeff_t value[3];
} _3coeff_t;

#if defined(_M_X64) || defined(__x86_64__)      // for 64 bit

// primitive root is 10

static const _3coeff_t _MUL_FNTT_P = {          // 0x7ffffffffffffffffffffffffffffffef * 2**61 + 1
        0xe000000000000001, 0xfffffffffffffffd, 0xffffffffffffffff
};

static const _3coeff_t _MUL_FNTT_W[62] = {
        { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 },
        { 0xe000000000000000, 0xfffffffffffffffd, 0xffffffffffffffff },
        { 0x268f06ae82aa14ff, 0x98db721822bab6ee, 0xa5659ab8416d3156 },
        { 0x8b3c2b87eab36016, 0xf9744cd60cb1e349, 0x17b77c40f74bd409 },
        { 0xbd72934c4dae4ef8, 0x63d5bc22f4959bfc, 0x7084fb48a47536b5 },
        { 0x8f47fee74ce322ec, 0x896a4641b2c0a80f, 0x2edfecc60c731576 },
        { 0x1c1798c86f734e14, 0xe93674afbed1abdd, 0x944818b5bcd42428 },
        { 0x1f930db162777b6a, 0xd47b3719494592cb, 0x825de92ae1327d74 },
        { 0xf37706ab0e8a11ab, 0xc380271a56dfe0c5, 0x186c18182cad4b4c },
        { 0xf217b0506eb79384, 0x12a625ffd098e3cd, 0xd3004ff8cda7270f },
        { 0x7594509567deb3cf, 0xe79c39e5f36a9aba, 0x8bd21ebceb9a3c9f },
        { 0xfc257eda37737521, 0xc1bae6ef2e5d67c6, 0x80ed240d66e77c77 },
        { 0x60d9698b73868334, 0xded7cc38fb9e1e7d, 0x2fe489fc54023ae7 },
        { 0x30c90fe0f123bd3f, 0x028f6f64edb7ff98, 0x70c10bc1ef19e180 },
        { 0x079771f82e66c3ff, 0x5293f66d1c6f067e, 0xd97e1e5df0309152 },
        { 0xd6e7598a265527d4, 0xc52a47d72f19153c, 0xe691d11a6286ffaf },
        { 0x4816cc31b2da67db, 0x732d352cc7d1eb7b, 0x70d844b446b0981c },
        { 0x68f93c644d4fa3e8, 0x7f0f82be1be12efe, 0xc5755c14702d6c97 },
        { 0x1ef4936eee55e2c8, 0x002ed9f725404a77, 0xfb9f92f7f57ed56d },
        { 0x1bccd749153d629d, 0x288fabbe24515700, 0x959fc6daca9f2bfa },
        { 0xfb51604220b743be, 0xa0c58bff349daaf2, 0x256e2fc29a22d60b },
        { 0x4e0087f050d039a1, 0xcca42a519983a5af, 0x999b77f9b672f13b },
        { 0xa2f775f2e8fcb9ad, 0xbb7461377832e0a0, 0x73e0dd19d6658525 },
        { 0x89ea615ac4eb234a, 0x974e6d5c68e84f13, 0x6ecd4cd67c578f7a },
        { 0x6ea272b12cc069d4, 0x050179b756846ee2, 0x0f6bd6d5ba31ecba },
        { 0x5c1d7ab0310777c8, 0x1a385e58b871beb7, 0xff2d1744fd5cd7cc },
        { 0x2874a2c923a512b6, 0x13b2055bc530a59d, 0x0add30f1db9d59e2 },
        { 0xb47f7cc5c066a3dc, 0x955c5610b76a00c6, 0x151ede288eba0db3 },
        { 0x216c1912b3e70f7a, 0x01b0661ae0412869, 0x116c6dec4542c506 },
        { 0xc63f7bdd2326343f, 0xacff8fabe11d4b77, 0x4e611f01e70a70eb },
        { 0x4cf7d0d512b39e06, 0xd5188d5cd8fb1d4a, 0x194714fca1401c16 },
        { 0x02c6b3c43ed5b93e, 0x512550274bea47e2, 0x958ed543cfa9743d },
        { 0x72323dfdd5cb3b90, 0xeac158a58c3c1994, 0x351f4fd9dadf79b9 },
        { 0x9c4331208529ae24, 0x6a382edf28073a78, 0x292e0463d6e5e874 },
        { 0x2787706cb01ce6b9, 0x9068533e9e1a9a15, 0x9c4c6789691f1a97 },
        { 0x16586940a1ec0866, 0x1b942eab9b45f5cd, 0x81028bec131431d4 },
        { 0x88d750d668490095, 0xee185b64e215288c, 0x00325ec69d457934 },
        { 0x1157a3536ee94f13, 0x069f3cc1ee802a66, 0x7d6ee7df60b18c68 },
        { 0xfabb9e1e8a162262, 0x2a8e039f8eaa26cc, 0x6818ead7f1d49349 },
        { 0xfb342082bf1b3230, 0x8936703c1c69035e, 0x2eab724b0923cfcb },
        { 0xacbb0a168b8d50a2, 0x70bcbe874b1263c7, 0x08d366f71e7b8cb5 },
        { 0xe42c8203c987ac02, 0xa85f6f2b0005c11b, 0x730ce590d11f78c6 },
        { 0xb68ba2e297304b42, 0x7eb52c3720554f5a, 0xc6d1353da665d732 },
        { 0x25c8eba4607fa9b6, 0x5052c3c7f149a587, 0xdc434b4e2beab7a3 },
        { 0xd911c49595183a59, 0xd39f5193f16bfa08, 0x76e6aa0e83b9ea3a },
        { 0x8340fcad8bb3491e, 0x763419b22ddecceb, 0x0e395763b5b03daf },
        { 0x275a09661ea7109b, 0x85dccac29b3d423b, 0xd84a72de24b32519 },
        { 0xc088472f52ddb4d6, 0x63c9030d0606c5d5, 0xa5393d7426d48228 },
        { 0x890ea6510f5e98cb, 0x0cd78bbc81a7e799, 0x064af89256255313 },
        { 0xcd2bd79bd23ecb3b, 0x67b3c164f9a7f457, 0x01cfcf34724ad317 },
        { 0xa9ef2fc90ee9b1fe, 0x018c1b461ce323ec, 0xa7cb7ccccb0d9d8d },
        { 0x2adce751aebc2e81, 0xc80691c8cc0a0958, 0x5349c13686977022 },
        { 0xcd5e307c40aa2b41, 0x250220f39f211695, 0x6703b15aea021829 },
        { 0xe26fb1e35021da0f, 0xd71ede7b9bf7f06b, 0xd8488e28e17e5994 },
        { 0x4e35cadd9e178d2a, 0x78f5d385dad5b1ff, 0x2e75c74ef91d74a6 },
        { 0x2071656699ae7abd, 0xff45d720e34b9ba8, 0x66371c884e437be8 },
        { 0x90e72a7e23d8bc31, 0x56774839b300103a, 0xf73678ea0ca0a585 },
        { 0x19da00a2cfb1b17a, 0x514504db9b04fb96, 0x5ed66fc059a74dd9 },
        { 0xb59e8fb6938c9e09, 0x562bc9f31308d4ad, 0x95f0d303c29a3a0d },
        { 0x7b1967aff3ad4a7e, 0xaac3aa54a415aabc, 0x4291908ac5544404 },
        { 0x465fc9f824de030b, 0x0d36a5cc801dcf6b, 0x1a720e7bca57c6b2 },
        { 0xbd2fcc0d5d4fd98b, 0x3ef4d55ac8cc8239, 0x6653473ac2d36c17 }
};

static const _3coeff_t _MUL_FNTT_W_Reciprocal[62] = {
        { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 },
        { 0xe000000000000000, 0xfffffffffffffffd, 0xffffffffffffffff },
        { 0xb970f9517d55eb02, 0x67248de7dd45490f, 0x5a9a6547be92cea9 },
        { 0x488551fe5af7cc25, 0xad0a316f2747feaa, 0x54f3f989a2ec5425 },
        { 0xca44d1916337caa0, 0x628d25f252378a6b, 0xd16411ba49909bb0 },
        { 0x69a0abefc20ab17f, 0x3362f978e09915e7, 0x003927d1dd6a4787 },
        { 0xbaa54894d3347c5a, 0x0c9b25778fb70cac, 0x3f6415748d9b0eb0 },
        { 0xc222ad6526147c31, 0x668c81ccf022b09b, 0x8a1587e9c7599376 },
        { 0x012221b22746f3d5, 0x138cc20305852476, 0x41fb5b53e135c3f7 },
        { 0xc7470dda754f7792, 0x93a852c09960b9ee, 0x26eac05195eb80da },
        { 0xb224a7a29ce0b98e, 0x54846ab5852b97cc, 0x16da8f01d1914390 },
        { 0x8e5086566383f7c0, 0x0fd4097801649f2f, 0xcc23a5daa9aeb2b7 },
        { 0x41cd87476d6fe68a, 0xa355eac5563bd1c6, 0x3e02a613265327c7 },
        { 0xf305beec64237b6f, 0x4a9d6809ddfa2369, 0x22cb4e631d9f259e },
        { 0xb086b687c8e174d9, 0x6b39b78cb430f1d5, 0x7188eb6cdea48352 },
        { 0x8624e019b6edb0d3, 0xfdfc2d8948a36b6a, 0xf2972998b3db386c },
        { 0xc20415240fe0c002, 0x14b5ef8571581838, 0x47c1382d4d29c88f },
        { 0xeb4344d8d5eeb5d6, 0xf8456c68ce5fdf8d, 0xef930cb22d9a3c99 },
        { 0x9a23a33285456d30, 0xb98b1c963063c2cd, 0xffeaad63af679775 },
        { 0x005215a11c6db6d3, 0x54283accd0612248, 0xe61d8620734371d7 },
        { 0x1b99232f8c5d03f6, 0x8bb3d2fefb122110, 0x013385be29ef1b71 },
        { 0x6fa5c133a4cb4689, 0x19793b3ef4c8d844, 0xd5cc01b683f12254 },
        { 0xca812a6c42e51cec, 0x905605a2ca9354f7, 0x916ba09b21b50550 },
        { 0x489f01df9b783027, 0x7d963e881570c089, 0x6afcc68839bdac56 },
        { 0xc3665b7b47ebeec2, 0x15ea605cda5014b1, 0x75822492d919f9ec },
        { 0xa8f7658c0cb8a034, 0x66fe2eacf1d04223, 0x0fc57baad24da3b8 },
        { 0x33485d530511a168, 0xfd3d929acb330f70, 0xcfaf07884879e255 },
        { 0xbe0776e4fb831759, 0x747ee301895cb224, 0x9c39bca900e44419 },
        { 0x506e8718221ce2f1, 0xb90eba6a43524e66, 0x316dd3b6bddc1ef7 },
        { 0xfc9961d1154e4c2b, 0x424d3e3c244c8da1, 0xf46a349ff3830514 },
        { 0x152fce96fad53843, 0x30d2ac7f085adb5d, 0xcc3f6d6712ff1e2c },
        { 0x0a61c9539f276ae9, 0x478ab6df144ff7e5, 0xb281a3e26b8ca529 },
        { 0x6e7d347487ab2aa0, 0x5c28cdb0185420d5, 0xd985df3e3ceb8ca8 },
        { 0xe8dbe088135fd4ec, 0x336a5f9b13e5f3d0, 0x91aca002a4a3a480 },
        { 0xd9a1b7f0bd9ed9f3, 0x2159f349fe4129cd, 0xcc70346818dc9e82 },
        { 0x47c3b95d08ba40d9, 0x8afce836dca10f8e, 0x9d1a354e6f89a2e3 },
        { 0xd2e4f40b030cbe2b, 0x46b7592ccadbec23, 0xf0a074543fa99bd2 },
        { 0x9d553458c8f0fee4, 0x964849aefa87bfc3, 0xb3484c9156b89684 },
        { 0x9fae4f167d453766, 0x8138f7fefe7f0ab1, 0x2937ebf38c5253d0 },
        { 0x10deff4f650a864b, 0x30b44b3e16a7286e, 0xd4f5d987eb817cc4 },
        { 0x8155d6ef56389519, 0xf09152bbd7a76af4, 0xb6b6a86d83445384 },
        { 0xdca4a8ecbd746cc0, 0xe2900adf1064b193, 0xfd2af47ed7c08570 },
        { 0x8f4e640bf3f18ef1, 0xfdb056050d5ce5e3, 0x9710d7235fdb677b },
        { 0x75b19d3e71702fb5, 0xe1e61ce4666c9dc8, 0x52e39f9abcb5d5ea },
        { 0x6371adf47b988905, 0x9116959866dd732e, 0xf39cbbab6d1e9a81 },
        { 0xd58f02d93850914c, 0xdf85f0430adeed2e, 0x2ff85a47956db83b },
        { 0x0c7987fa572473d0, 0x2649038094893958, 0xdf24edd33d389f67 },
        { 0xbecfff4e7f36eb8c, 0x80183b3afe446f4f, 0x572806af7676eb5e },
        { 0x1e69f997d952fefa, 0x97f0b60afeab0f6a, 0x7c71e1e0f1d47705 },
        { 0x010ed94ec02f1321, 0xf4e29ea05b7577fe, 0xbb2c2517ff616725 },
        { 0xfeb15a69d93665db, 0xe8e3133f5918113d, 0x455abc5396a511d4 },
        { 0x29b8eb13859d4069, 0x2afc67b8df4a9ab3, 0x2662f8d129b2c71e },
        { 0x7454a3179fe37d70, 0xccb8744c32a068ba, 0x0ea2b2b6f7945818 },
        { 0xc2544a71c2a6da1e, 0x489f4cb266573b7b, 0x66aadd6d1d4dfac9 },
        { 0x7c9019e638f03f05, 0xff400fc3f03cf2a8, 0xd809dbfbec001743 },
        { 0x68a0f6f375b507f0, 0xfed353c86ac690fd, 0xd97c92d4a664be40 },
        { 0xafcfcb9a2ca412da, 0x7e2be48cd5394d4a, 0x2d42e9b2a2a85174 },
        { 0x4b21d445aaaf66f8, 0x48050a1560ac425a, 0x2a77bfc46e1b81dc },
        { 0x948ce7e000844aad, 0xf657d533482d4e6c, 0x6d204ce94ba264ac },
        { 0xa3467a01f24a4917, 0x29d1a87231df6b14, 0x1730cca804a3de05 },
        { 0x0750b93c981a1965, 0xd78d02f182adbb12, 0x1251fd169c3138fe },
        { 0x5128bd9953f64157, 0xf6fe7dba1237cb14, 0xbb5381fc1d7fc4f3 }
};

static const _3coeff_t _MUL_FNTT_N_Reciprocal[62] = {
        { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 },
        { 0xf000000000000001, 0xfffffffffffffffe, 0x7fffffffffffffff },
        { 0x6800000000000001, 0xfffffffffffffffe, 0xbfffffffffffffff },
        { 0x2400000000000001, 0xfffffffffffffffe, 0xdfffffffffffffff },
        { 0x0200000000000001, 0xfffffffffffffffe, 0xefffffffffffffff },
        { 0xf100000000000001, 0xfffffffffffffffd, 0xf7ffffffffffffff },
        { 0xe880000000000001, 0xfffffffffffffffd, 0xfbffffffffffffff },
        { 0xe440000000000001, 0xfffffffffffffffd, 0xfdffffffffffffff },
        { 0xe220000000000001, 0xfffffffffffffffd, 0xfeffffffffffffff },
        { 0xe110000000000001, 0xfffffffffffffffd, 0xff7fffffffffffff },
        { 0xe088000000000001, 0xfffffffffffffffd, 0xffbfffffffffffff },
        { 0xe044000000000001, 0xfffffffffffffffd, 0xffdfffffffffffff },
        { 0xe022000000000001, 0xfffffffffffffffd, 0xffefffffffffffff },
        { 0xe011000000000001, 0xfffffffffffffffd, 0xfff7ffffffffffff },
        { 0xe008800000000001, 0xfffffffffffffffd, 0xfffbffffffffffff },
        { 0xe004400000000001, 0xfffffffffffffffd, 0xfffdffffffffffff },
        { 0xe002200000000001, 0xfffffffffffffffd, 0xfffeffffffffffff },
        { 0xe001100000000001, 0xfffffffffffffffd, 0xffff7fffffffffff },
        { 0xe000880000000001, 0xfffffffffffffffd, 0xffffbfffffffffff },
        { 0xe000440000000001, 0xfffffffffffffffd, 0xffffdfffffffffff },
        { 0xe000220000000001, 0xfffffffffffffffd, 0xffffefffffffffff },
        { 0xe000110000000001, 0xfffffffffffffffd, 0xfffff7ffffffffff },
        { 0xe000088000000001, 0xfffffffffffffffd, 0xfffffbffffffffff },
        { 0xe000044000000001, 0xfffffffffffffffd, 0xfffffdffffffffff },
        { 0xe000022000000001, 0xfffffffffffffffd, 0xfffffeffffffffff },
        { 0xe000011000000001, 0xfffffffffffffffd, 0xffffff7fffffffff },
        { 0xe000008800000001, 0xfffffffffffffffd, 0xffffffbfffffffff },
        { 0xe000004400000001, 0xfffffffffffffffd, 0xffffffdfffffffff },
        { 0xe000002200000001, 0xfffffffffffffffd, 0xffffffefffffffff },
        { 0xe000001100000001, 0xfffffffffffffffd, 0xfffffff7ffffffff },
        { 0xe000000880000001, 0xfffffffffffffffd, 0xfffffffbffffffff },
        { 0xe000000440000001, 0xfffffffffffffffd, 0xfffffffdffffffff },
        { 0xe000000220000001, 0xfffffffffffffffd, 0xfffffffeffffffff },
        { 0xe000000110000001, 0xfffffffffffffffd, 0xffffffff7fffffff },
        { 0xe000000088000001, 0xfffffffffffffffd, 0xffffffffbfffffff },
        { 0xe000000044000001, 0xfffffffffffffffd, 0xffffffffdfffffff },
        { 0xe000000022000001, 0xfffffffffffffffd, 0xffffffffefffffff },
        { 0xe000000011000001, 0xfffffffffffffffd, 0xfffffffff7ffffff },
        { 0xe000000008800001, 0xfffffffffffffffd, 0xfffffffffbffffff },
        { 0xe000000004400001, 0xfffffffffffffffd, 0xfffffffffdffffff },
        { 0xe000000002200001, 0xfffffffffffffffd, 0xfffffffffeffffff },
        { 0xe000000001100001, 0xfffffffffffffffd, 0xffffffffff7fffff },
        { 0xe000000000880001, 0xfffffffffffffffd, 0xffffffffffbfffff },
        { 0xe000000000440001, 0xfffffffffffffffd, 0xffffffffffdfffff },
        { 0xe000000000220001, 0xfffffffffffffffd, 0xffffffffffefffff },
        { 0xe000000000110001, 0xfffffffffffffffd, 0xfffffffffff7ffff },
        { 0xe000000000088001, 0xfffffffffffffffd, 0xfffffffffffbffff },
        { 0xe000000000044001, 0xfffffffffffffffd, 0xfffffffffffdffff },
        { 0xe000000000022001, 0xfffffffffffffffd, 0xfffffffffffeffff },
        { 0xe000000000011001, 0xfffffffffffffffd, 0xffffffffffff7fff },
        { 0xe000000000008801, 0xfffffffffffffffd, 0xffffffffffffbfff },
        { 0xe000000000004401, 0xfffffffffffffffd, 0xffffffffffffdfff },
        { 0xe000000000002201, 0xfffffffffffffffd, 0xffffffffffffefff },
        { 0xe000000000001101, 0xfffffffffffffffd, 0xfffffffffffff7ff },
        { 0xe000000000000881, 0xfffffffffffffffd, 0xfffffffffffffbff },
        { 0xe000000000000441, 0xfffffffffffffffd, 0xfffffffffffffdff },
        { 0xe000000000000221, 0xfffffffffffffffd, 0xfffffffffffffeff },
        { 0xe000000000000111, 0xfffffffffffffffd, 0xffffffffffffff7f },
        { 0xe000000000000089, 0xfffffffffffffffd, 0xffffffffffffffbf },
        { 0xe000000000000045, 0xfffffffffffffffd, 0xffffffffffffffdf },
        { 0xe000000000000023, 0xfffffffffffffffd, 0xffffffffffffffef },
        { 0xe000000000000012, 0xfffffffffffffffd, 0xfffffffffffffff7 },
};

#elif defined(_M_IX86) || defined(__i386__)     // for 32 bit

// primitive root is 11

static const _3coeff_t _MUL_FNTT_P = {          // 0x3fffffffffffffffc * 2**30 + 1
        0x00000001, 0xffffffff, 0xffffffff
};

static const _3coeff_t _MUL_FNTT_W[31] = {
        { 0x00000000, 0x00000000, 0x00000000 },
        { 0x00000000, 0xffffffff, 0xffffffff },
        { 0xf4242244, 0x46244b4a, 0x3cb4349a },
        { 0xa4bc5333, 0xd5cc89e1, 0xd1a060dd },
        { 0xd49ed9d2, 0xd83c0bfa, 0x8a505b33 },
        { 0x4ffda344, 0x92c394b5, 0x69b5f6be },
        { 0x60231e57, 0xb2544d57, 0xdbac5115 },
        { 0x00747649, 0xa4a220fd, 0xf4b8466a },
        { 0xa6b35162, 0x37d91548, 0x4f286a4d },
        { 0x809421e5, 0x75d68c06, 0x5d7ece47 },
        { 0x018f39a1, 0x0d723e54, 0xdeb4c620 },
        { 0x37194baa, 0x615f3ac8, 0x0c4be19a },
        { 0xdb59447a, 0x8180a5b2, 0x54998f8f },
        { 0x0e4bff5d, 0xe447ac45, 0x8908d877 },
        { 0x4f6c4f21, 0x993702ff, 0x5c1bba82 },
        { 0x6fb9c238, 0x89e57c29, 0xc802fa71 },
        { 0xb2a5dcb4, 0xcb22f24f, 0x3d939b77 },
        { 0x8047b333, 0x97c5da09, 0x9ebeca7b },
        { 0x5acdea6e, 0xccb15a6b, 0x59acdecb },
        { 0x5681b382, 0xfea20e9b, 0x3ac840b1 },
        { 0x28cc279e, 0xd200d62f, 0xb90d8cd3 },
        { 0xa6e53b39, 0x7582bac5, 0x7f548a8a },
        { 0xfc0cb416, 0x246ae74f, 0x1578b31e },
        { 0x2e747501, 0x3044c6e7, 0xd0a79d7b },
        { 0xb5ab3c03, 0x67993b78, 0xbf6074a8 },
        { 0xd434ee26, 0x4ab45270, 0xe99b1c2d },
        { 0x105814bd, 0xb0433e11, 0x1819e9e9 },
        { 0x497e992b, 0x7483e5aa, 0xa06a4eac },
        { 0x3066672d, 0x532a59b1, 0x26c560fa },
        { 0xe3fd9f76, 0x2c1cb23c, 0x3a81801c },
        { 0x873d7ace, 0xf16b49f5, 0xec12797c }
};

static const _3coeff_t _MUL_FNTT_W_Reciprocal[31] = {
        { 0x00000000, 0x00000000, 0x00000000 },
        { 0x00000000, 0xffffffff, 0xffffffff },
        { 0x0bdbddbd, 0xb9dbb4b4, 0xc34bcb65 },
        { 0xc0167547, 0x53f89789, 0x873fbcf0 },
        { 0x7ab95006, 0xd08a0f92, 0x90afe136 },
        { 0xcb93ddf9, 0x786a6435, 0xf7f233b5 },
        { 0xe49086b2, 0x9a2c6d9b, 0x699e95bd },
        { 0xa757790d, 0xccff8af4, 0x273dbe8d },
        { 0xe3af8bf5, 0xc2a8e40d, 0x1464a6aa },
        { 0x92e7853e, 0x626fd476, 0xe18ac5ee },
        { 0xf69fbdbd, 0xe3509326, 0xd5f09809 },
        { 0x2bcf6f31, 0x3e5b4135, 0x638c09ef },
        { 0xdddd301b, 0xc7279aa9, 0x8afbe17f },
        { 0x67de142f, 0x3e3a9661, 0x5f74e509 },
        { 0xedb4cc9e, 0x214f8e89, 0xe9ad5f0f },
        { 0x79f6f343, 0x060551ce, 0x4360b650 },
        { 0x076dfb7a, 0x1a3e8308, 0xc2168d4a },
        { 0xac636031, 0x0c85cedb, 0x5059c649 },
        { 0x2b7bb682, 0xaae2c853, 0x4470213a },
        { 0x1202856d, 0x1660eaea, 0x9aa72517 },
        { 0xbd008e04, 0x5a6c6a90, 0x7bb0d851 },
        { 0xa03bbbef, 0xbd4c9b74, 0x444989c0 },
        { 0xcd9bd5e9, 0x3266e7bb, 0x22874a9c },
        { 0xb2afb2ce, 0x7f74fcfa, 0xf4041cdc },
        { 0x1b9bf093, 0xaccb622b, 0x668b09db },
        { 0xdafac5c6, 0x9baf6baf, 0xe31c10c9 },
        { 0x3dabb6d4, 0xcaefaba2, 0xe20c0d19 },
        { 0x7efdf6d0, 0xad4f33c4, 0x5ec4c080 },
        { 0x8978646c, 0x71dd2ddf, 0xe61c62bb },
        { 0xa7c91c3d, 0xa48cccbb, 0x3b045257 },
        { 0x2339ea58, 0x972706a8, 0x040c231a }
};

static const _3coeff_t _MUL_FNTT_N_Reciprocal[31] = {
        { 0x00000000, 0x00000000, 0x00000000 },
        { 0x80000001, 0xffffffff, 0x7fffffff },
        { 0x40000001, 0xffffffff, 0xbfffffff },
        { 0x20000001, 0xffffffff, 0xdfffffff },
        { 0x10000001, 0xffffffff, 0xefffffff },
        { 0x08000001, 0xffffffff, 0xf7ffffff },
        { 0x04000001, 0xffffffff, 0xfbffffff },
        { 0x02000001, 0xffffffff, 0xfdffffff },
        { 0x01000001, 0xffffffff, 0xfeffffff },
        { 0x00800001, 0xffffffff, 0xff7fffff },
        { 0x00400001, 0xffffffff, 0xffbfffff },
        { 0x00200001, 0xffffffff, 0xffdfffff },
        { 0x00100001, 0xffffffff, 0xffefffff },
        { 0x00080001, 0xffffffff, 0xfff7ffff },
        { 0x00040001, 0xffffffff, 0xfffbffff },
        { 0x00020001, 0xffffffff, 0xfffdffff },
        { 0x00010001, 0xffffffff, 0xfffeffff },
        { 0x00008001, 0xffffffff, 0xffff7fff },
        { 0x00004001, 0xffffffff, 0xffffbfff },
        { 0x00002001, 0xffffffff, 0xffffdfff },
        { 0x00001001, 0xffffffff, 0xffffefff },
        { 0x00000801, 0xffffffff, 0xfffff7ff },
        { 0x00000401, 0xffffffff, 0xfffffbff },
        { 0x00000201, 0xffffffff, 0xfffffdff },
        { 0x00000101, 0xffffffff, 0xfffffeff },
        { 0x00000081, 0xffffffff, 0xffffff7f },
        { 0x00000041, 0xffffffff, 0xffffffbf },
        { 0x00000021, 0xffffffff, 0xffffffdf },
        { 0x00000011, 0xffffffff, 0xffffffef },
        { 0x00000009, 0xffffffff, 0xfffffff7 },
        { 0x00000005, 0xffffffff, 0xfffffffb }
};

#endif

void _3coeff_addmod(_3coeff_t* a, const _3coeff_t* b) {
    uint8_t c_flag;
    c_flag = _addcarry_coeff(0, a->l, b->l, &a->l);
    c_flag = _addcarry_coeff(c_flag, a->m, b->m, &a->m);
    c_flag = _addcarry_coeff(c_flag, a->h, b->h, &a->h);

    if (c_flag) {
        uint8_t borrow;
        borrow = _subborrow_coeff(0, a->l, _MUL_FNTT_P.l, &a->l);
        borrow = _subborrow_coeff(borrow, a->m, _MUL_FNTT_P.m, &a->m);
        borrow = _subborrow_coeff(borrow, a->h, _MUL_FNTT_P.h, &a->h);
        return;
    }

    if (a->h > _MUL_FNTT_P.h ||
        a->h == _MUL_FNTT_P.h && a->m > _MUL_FNTT_P.m ||
        a->h == _MUL_FNTT_P.h && a->m == _MUL_FNTT_P.m && a->l >= _MUL_FNTT_P.l) {
        c_flag = _subborrow_coeff(0, a->l, _MUL_FNTT_P.l, &a->l);
        c_flag = _subborrow_coeff(c_flag, a->m, _MUL_FNTT_P.m, &a->m);
        _subborrow_coeff(c_flag, a->h, _MUL_FNTT_P.h, &a->h);
    }


}

void _3coeff_submod(_3coeff_t* a, const _3coeff_t* b) {
    uint8_t c_flag;
    c_flag = _subborrow_coeff(0, a->l, b->l, &a->l);
    c_flag = _subborrow_coeff(c_flag, a->m, b->m, &a->m);
    c_flag = _subborrow_coeff(c_flag, a->h, b->h, &a->h);

    if (c_flag) {
        c_flag = _addcarry_coeff(0, a->l, _MUL_FNTT_P.l, &a->l);
        c_flag = _addcarry_coeff(c_flag, a->m, _MUL_FNTT_P.m, &a->m);
        _addcarry_coeff(c_flag, a->h, _MUL_FNTT_P.h, &a->h);
    }
}

void _3coeff_mulmod(_3coeff_t* a, const _3coeff_t* b) {
    coeff_t product[6] = {};

    coeff_t temp[2];
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            temp[0] = _mul_coeff(a->value[j], b->value[i], temp + 1);
            if (_addcarry_coeff(_addcarry_coeff(0, product[i + j], temp[0], product + i + j),
                                product[i + j + 1],
                                temp[1],
                                product + i + j + 1)) {
                coeff_t* block = product + i + j + 2;
                while (_addcarry_coeff(1, *block, 0, block))
                    ++block;
            }
        }
    }

    accelc_uintx_mod(product, 6, _MUL_FNTT_P.value, 3);
    *a = *(_3coeff_t*)product;
}

void _3coeff_powmod(_3coeff_t* a, coeff_t b) {
    _3coeff_t ret = { 1 };

    while(b) {
        if (b % 2 == 1) {
            _3coeff_mulmod(&ret, a);
        }
        b /= 2;
        _3coeff_mulmod(a, a);
    }

    *a = ret;
}

static int _FNTT_base2_extend_impl(const coeff_t* __restrict src, size_t len,
                                   _3coeff_t* __restrict dst,
                                   const _3coeff_t* w, size_t gap) {
    if (len == 2) {
        if (gap == 1) {
            _3coeff_t right_part = { src[gap], 0, 0 };
            dst[0].l = src[0]; dst[0].m = 0; dst[0].h = 0;
            dst[2].l = src[0]; dst[2].m = 0; dst[2].h = 0;
            _3coeff_addmod(dst, &right_part);
            _3coeff_submod(dst + 2, &right_part);

            _3coeff_mulmod(&right_part, w);
            dst[1].l = src[0]; dst[1].m = 0; dst[1].h = 0;
            dst[3].l = src[0]; dst[3].m = 0; dst[3].h = 0;
            _3coeff_addmod(dst + 1, &right_part);
            _3coeff_submod(dst + 3, &right_part);
        } else {
            _3coeff_t right_part = { src[gap], 0, 0 };
            dst[0].l = src[0]; dst[0].m = 0; dst[0].h = 0;
            dst[4].l = src[0]; dst[4].m = 0; dst[4].h = 0;
            _3coeff_addmod(dst, &right_part);
            _3coeff_submod(dst + 4, &right_part);

            _3coeff_mulmod(&right_part, w);
            dst[2].l = src[0]; dst[2].m = 0; dst[2].h = 0;
            dst[6].l = src[0]; dst[6].m = 0; dst[6].h = 0;
            _3coeff_addmod(dst + 2, &right_part);
            _3coeff_submod(dst + 6, &right_part);
        }
    } else {
        _3coeff_t* buffer = malloc(sizeof(_3coeff_t) * len * 2);
        if (buffer == 0)
            return 0;

        _3coeff_t* dst_even = buffer;
        _3coeff_t* dst_odd = buffer + 1;

        _3coeff_t next_w = *w;
        _3coeff_mulmod(&next_w, w);
        if (1 != _FNTT_base2_extend_impl(src, len / 2,
                                         dst_even,
                                         &next_w,
                                         gap * 2)) {
            free(buffer);
            return 0;
        }
        if (1 != _FNTT_base2_extend_impl(src + gap, len / 2,
                                         dst_odd,
                                         &next_w,
                                         gap * 2)) {
            free(buffer);
            return 0;
        }

        if (gap == 1) {
            for (size_t k = 0, pair = len; k < pair; ++k) {
                _3coeff_t right_part = *w;
                _3coeff_powmod(&right_part, k);
                _3coeff_mulmod(&right_part, dst_odd + 2 * k);

                dst[k] = dst_even[2 * k];
                dst[k + pair] = dst_even[2 * k];
                _3coeff_addmod(dst + k, &right_part);
                _3coeff_submod(dst + k + pair, &right_part);
            }
        } else {
            for (size_t k = 0, pair = len; k < pair; ++k) {
                _3coeff_t right_part = *w;
                _3coeff_powmod(&right_part, k);
                _3coeff_mulmod(&right_part, dst_odd + 2 * k);

                dst[2 * k] = dst_even[2 * k];
                dst[2 * (k + pair)] = dst_even[2 * k];
                _3coeff_addmod(dst + 2 * k, &right_part);
                _3coeff_submod(dst + 2 * (k + pair), &right_part);
            }
        }

        free(buffer);
    }
    return 1;
}

static int _FNTT_base2_impl(const _3coeff_t* __restrict src, size_t len,
                            _3coeff_t* __restrict dst,
                            const _3coeff_t* w, size_t gap) {
    if (len == 2) {
        if (gap == 1) {
            dst[0] = src[0];
            dst[1] = src[0];
            _3coeff_addmod(dst, src + gap);
            _3coeff_submod(dst + 1, src + gap);
        } else {
            dst[0] = src[0];
            dst[2] = src[0];
            _3coeff_addmod(dst, src + gap);
            _3coeff_submod(dst + 2, src + gap);
        }
    } else {
        _3coeff_t* buffer = malloc(sizeof(_3coeff_t) * len);
        if (buffer == 0)
            return 0;

        _3coeff_t* dst_even = buffer;
        _3coeff_t* dst_odd = buffer + 1;

        _3coeff_t next_w = *w;
        _3coeff_mulmod(&next_w, w);
        if (1 != _FNTT_base2_impl(src, len / 2,
                                  dst_even,
                                  &next_w,
                                  gap * 2)) {
            free(buffer);
            return 0;
        }
        if (1 != _FNTT_base2_impl(src + gap, len / 2,
                                  dst_odd,
                                  &next_w,
                                  gap * 2)) {
            free(buffer);
            return 0;
        }

        if (gap == 1) {
            for (size_t k = 0, pair = len / 2; k < pair; ++k) {
                _3coeff_t right_part = *w;
                _3coeff_powmod(&right_part, k);
                _3coeff_mulmod(&right_part, dst_odd + 2 * k);

                dst[k] = dst_even[2 * k];
                dst[k + pair] = dst_even[2 * k];
                _3coeff_addmod(dst + k, &right_part);
                _3coeff_submod(dst + k + pair, &right_part);
            }
        } else {
            for (size_t k = 0, pair = len / 2; k < pair; ++k) {
                _3coeff_t right_part = *w;
                _3coeff_powmod(&right_part, k);
                _3coeff_mulmod(&right_part, dst_odd + 2 * k);

                dst[2 * k] = dst_even[2 * k];
                dst[2 * (k + pair)] = dst_even[2 * k];
                _3coeff_addmod(dst + 2 * k, &right_part);
                _3coeff_submod(dst + 2 * (k + pair), &right_part);
            }
        }

        free(buffer);
    }
    return 1;
}

int accelc_uintx_multo_FNTT(const coeff_t* multiplier, const coeff_t* multiplicand,
                            size_t length,
                            coeff_t* __restrict product) {
    if (length == 0)
        return 0;

    if ((length & (length - 1)) != 0)
        return 0;

    if (length == 1) {
        product[0] = _mul_coeff(*multiplier, *multiplicand, product + 1);
        return 1;
    }

    int index = _bsf_coeff(length);
    index++;

    _3coeff_t* buffer = malloc(sizeof(_3coeff_t) * length * 4);
    if (buffer == 0)
        return 0;

    _3coeff_t* multiplier_ntt = buffer;
    _3coeff_t* multiplicand_ntt = buffer + 2 * length;

    if (1 != _FNTT_base2_extend_impl(multiplier, length, multiplier_ntt, _MUL_FNTT_W + index, 1)) {
        free(buffer);
        return 0;
    }

    if (1 != _FNTT_base2_extend_impl(multiplicand, length, multiplicand_ntt, _MUL_FNTT_W + index, 1)) {
        free(buffer);
        return 0;
    }

    length *= 2;
    for (size_t i = 0; i < length; ++i)
        _3coeff_mulmod(multiplier_ntt + i, multiplicand_ntt + i);

    if (1 != _FNTT_base2_impl(multiplier_ntt, length, multiplicand_ntt, _MUL_FNTT_W_Reciprocal + index, 1)) {
        free(buffer);
        return 0;
    }

    memset(product, 0, sizeof(coeff_t) * length);

    uint8_t carry;
    size_t max = length - 2;    // multiplicand_ntt[length - 1] must be 0
    for (size_t i = 0; i < max; ++i) {
        _3coeff_mulmod(multiplicand_ntt + i, _MUL_FNTT_N_Reciprocal + index);

        carry = _addcarry_coeff(0, product[i], multiplicand_ntt[i].l, product + i);
        carry = _addcarry_coeff(carry, product[i + 1], multiplicand_ntt[i].m, product + i + 1);
        _addcarry_coeff(carry, product[i + 2], multiplicand_ntt[i].h, product + i + 2);
    }

    _3coeff_mulmod(multiplicand_ntt + max, _MUL_FNTT_N_Reciprocal + index);
    carry = _addcarry_coeff(0, product[max], multiplicand_ntt[max].l, product + max);
    carry = _addcarry_coeff(carry, product[max + 1], multiplicand_ntt[max].m, product + max + 1);

    free(buffer);

    return 1;
}