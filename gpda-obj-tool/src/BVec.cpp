///////////////////////////////////////////////////////////////
//                                                           //
//   BVec.cpp                                                //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "BVec.h"

using namespace std;
using namespace gpdaobjtool;

BVec::BVec() {}

BVec::BVec(BVec::const_iterator begin, size_t length)
{
    assign(begin, begin + length);
}

BVec::~BVec() {}

#if _MSC_VER >= 1900
string BVec::UTF16LEtoUTF8string() const
{
	size_t length = size();
	u16string utf16(length / 2, ' ');
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> codecvt;

	for (size_t i = 0; i < length; i += 2)
	{
		utf16[i / 2] = ((char16_t)((this->at(i + 1) << 8) | (this->at(i) << 0)));
	}

	wstring t(utf16.begin(), utf16.end());

	return codecvt.to_bytes(t);
}
#else
string BVec::UTF16LEtoUTF8string() const
{
    size_t length = size();
    u16string utf16(length / 2, ' ');
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> codecvt;

    for(size_t i = 0; i < length; i += 2)
    {
        utf16[i / 2] = ((char16_t)((this->at(i + 1) << 8) | (this->at(i) << 0)));
    }

    return codecvt.to_bytes(utf16);
}

#endif

#if _MSC_VER >= 1900
u16string BVec::UTF8toUTF16LEstring() const
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	wstring t = convert.from_bytes(string(begin(), end()));
	u16string utf16(t.begin(), t.end());
	u16string ret;

	for (char16_t c : utf16)
	{
		ret.push_back((uint8_t)(c >> 8) | (uint16_t)(c << 8));
	}

	return ret;
}
#else
u16string BVec::UTF8toUTF16LEstring() const
{
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> convert;

    u16string utf16 = convert.from_bytes(string(begin(), end()));
    u16string ret;

    for(char16_t c : utf16)
    {
        ret.push_back((uint8_t)(c >> 8) | (uint16_t)(c << 8));
    }

    return ret;
}
#endif

#if _MSC_VER >= 1900
void BVec::UTF16LEtoUTF8()
{
    size_t length = size();
    u16string utf16(length / 2, ' ');
    wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> codecvt;

    for(size_t i = 0; i < length; i += 2)
    {
        utf16[i / 2] = ((char16_t)((this->at(i + 1) << 8) | (this->at(i) << 0)));
    }

    clear();
	
	wstring t(utf16.begin(), utf16.end());
    for(uint8_t c : codecvt.to_bytes(t))
    {
        push_back(c);
    }
}
#else
void BVec::UTF16LEtoUTF8()
{
	size_t length = size();
	u16string utf16(length / 2, ' ');
	wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> codecvt;

	for (size_t i = 0; i < length; i += 2)
	{
		utf16[i / 2] = ((char16_t)((this->at(i + 1) << 8) | (this->at(i) << 0)));
	}

	clear();
	for (uint8_t c : codecvt.to_bytes(utf16))
	{
		push_back(c);
	}
}
#endif

#if _MSC_VER >= 1900
void BVec::UTF8toUTF16LE()
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	wstring t = convert.from_bytes(string(begin(), end()));
	u16string utf16(t.begin(), t.end());

	clear();

	for (char16_t c : utf16)
	{
		push_back((uint8_t)(c));
		push_back((uint8_t)(c >> 8));
	}
}
#else
void BVec::UTF8toUTF16LE()
{
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> convert;

    u16string utf16 = convert.from_bytes(string(begin(), end()));

    clear();

    for(char16_t c : utf16)
    {
        push_back((uint8_t)(c));
        push_back((uint8_t)(c >> 8));
    }
}
#endif

string BVec::ToHexString() const
{
    size_t length = size();
    string ret(length * 2, ' ');

    for (size_t i = 0; i < length; i++) {
        ret[2 * i] = HEXMAP[(this->at(i) & 0xF0) >> 4];
        ret[2 * i + 1] = HEXMAP[this->at(i) & 0x0F];
    }

    return ret;
}

string BVec::ToRawString() const
{
    size_t length = size();
    string ret(length, ' ');

    for (size_t i = 0; i < length; i++)
    {
        ret[i] = (char)this->at(i);
    }

    return ret;
}

uint16_t BVec::ToUint16_t() const
{
    return (at(0) << 8) | (at(1) << 0);
}

BVec BVec::FromUTF16LE(const std::u16string & input)
{
    BVec ret;

    for(char16_t c : input)
    {
        ret.push_back((uint8_t)(c >> 8));
        ret.push_back((uint8_t)(c));
    }

    return ret;
}

BVec BVec::FromUTF8(const std::string & input)
{
    BVec ret;

    ret.assign(input.begin(), input.end());

    return ret;
}

BVec BVec::FromHexString(const string & input)
{
    BVec ret;

    for (size_t i = 0; i < input.size(); i += 2)
    {
        ret.push_back((uint8_t)strtol(input.substr(i, 2).c_str(), NULL, 16));
    }

    return ret;
}
