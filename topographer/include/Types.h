///////////////////////////////////////////////////////////////
//                                                           //
//   Types.h                                                 //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_TYPES_H
#define TOPOGRAPHER_TYPES_H

#include <vector>
#include <string>

#define ENCODING_TABLE_FILE "encoding.tbl"
#define TRANSLATION_CLASS Translation_SG_Android_ant08
#define MERGER_CLASS Merger_SG_Android_ant08

#define LEFT_NAME_BRACKET BArr({ 0x81, 0x79 })
#define RIGHT_NAME_BRACKET BArr({ 0x81, 0x7A })

namespace topographer
{
    // Specific type for byte arrays, since they are used instead
    //   of the usual string type.
    typedef std::vector<uint8_t> BArr;

    // Arguments that are passed to the program on its launch.
    //   inputFile, string, path to the BIN file
    //   translationFile, string, path to the associated translation file.
    //   outputPath, string, path to the output directory
    //   useTranslation, bool, whether or not to translate the contents
    //     of the BIN file
    struct Arguments
    {
        std::string inputFile = "";
        std::string translationFile = "";
        std::string outputPath = "";
        bool useTranslation = false;
    };

    // Number of bits per pointer, can be either 16 or 32. It is
    //   adjusted automatically in order to generate proper Atlas
    //   scripts with the correct pointer size.
    enum PointerSz
    {
        psz16b = 16,
        psz32b = 32,
    };

    // Type of the the rule in the encoding table.
    enum RuleType
    {
        rtRaw,         // Just raw data
        rtControlCode, // Control code in the form "<CODE>"
    };

    // The encoding rule is any line in the encoding table. For
    //   example the rule "1337=<NICE>".
    //   type, RuleType, the type of the replacement rule. Default: rtRaw
    //   original, BArr, original byte sequence to look for
    //   replacement, BArr, string to replace original byte
    //     sequence with
    struct EncodingRule
    {
        RuleType type = rtRaw;
        BArr original = {};
        BArr replacement = {};
    };

    // Type of the token.
    enum TokenType
    {
        ttName,         // Token with a character name
        ttRaw,          // Token with raw text
        ttControlCode,  // Token with control code
    };

    // Token is a part of the GameString..
    //   type, TokenType, type of the token. Default: ttRaw
    //   data, BArr, the text data for this token
    struct StringToken
    {
        TokenType type = ttRaw;
        BArr data;
    };

    // An index that stores the position of the key string
    //   when mapping key strings and inner strings. A key string
    //   is any string that starts with '【' (basically a dialog). An
    //   inner string is any other string between current key and the
    //   next key. Used mainly in the StringList class.
    //   index, size_t, the index of the key string in all the strings
    //   numInnerStrings, uint8_t, number of inner strings of the key
    struct KeyIndex
    {
        size_t index = 0;
        uint8_t numInnerStrings = 0;
    };

    // Pointer data that is used when mapping strings and
    //   writing Atlas scripts.
    //   address, uint32_t, the actual address of the pointer. The
    //     writing location in the Atlas script
    //   stringAddress, uint32_t, the address of the beginning of
    //     the string this pointer normally (in the Japanese game)
    //     associates with
    struct Pointer
    {
        uint32_t address;
        uint32_t stringAddress;
    };
}

#endif
