///////////////////////////////////////////////////////////////
//                                                           //
//   BVec.h                                                  //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_BVEC_H
#define GDPA_OBJ_TOOL_BVEC_H

#include <vector>
#include <codecvt>
#include <locale>

namespace gpdaobjtool
{
    class BVec : public std::vector<uint8_t>
    {
    public:
        static constexpr char HEXMAP[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

        BVec();
        BVec(BVec::const_iterator begin, size_t length);
        ~BVec();

        std::string UTF16LEtoUTF8string() const;

        std::u16string UTF8toUTF16LEstring() const;

        void UTF16LEtoUTF8();

        void UTF8toUTF16LE();

        std::string ToHexString() const;

        std::string ToRawString() const;

        uint16_t ToUint16_t() const;

        static BVec FromUTF16LE(const std::u16string & input);

        static BVec FromUTF8(const std::string & input);

        static BVec FromHexString(const std::string & input);
    };
}


#endif
