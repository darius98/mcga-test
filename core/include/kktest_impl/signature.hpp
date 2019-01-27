#ifndef KKTEST_CORE_KKTEST_IMPL_SIGNATURE_HPP_
#define KKTEST_CORE_KKTEST_IMPL_SIGNATURE_HPP_

namespace kktest {

constexpr const int kkTestSigSize = 32;

constexpr const unsigned char kkTestSignature[] =
        "\x43\x00\xaa\x4f\x56\x6e\x0c\x64\xeb\xa1\xf5\x1d\x7c\xaa\xbc\xe8"
        "\xbf\x03\x2d\x86\x40\x69\x98\x65\xa3\x79\x51\xb4\x8a\x33\xce\x97";

constexpr const char kkTestSignatureHex[] =
        "4300aa4f566e0c64eba1f51d7caabce8bf032d8640699865a37951b48a33ce97";

}

#endif
