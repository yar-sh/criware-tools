///////////////////////////////////////////////////////////////
//                                                           //
//   Merger_SG_Android_ant08.cpp                             //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "Merger_SG_Android_ant08.h"

using namespace std;
using namespace topographer;

Merger_SG_Android_ant08::~Merger_SG_Android_ant08() { }

// Merges two StringLists into one
//   into, &StringList, original and also output StringList (JAP)
//   from, &StringList, list with translation (ENG)
void Merger_SG_Android_ant08::Merge(StringList & into, const StringList & from)
{
    cout << "\t...Merging translations" << endl;

    size_t szKeyOriginal = into.GetKeyStrings().size();
    size_t szKeyTranslation = from.GetKeyStrings().size();

    size_t szDirectOriginal = into.GetDirectStrings().size();
    size_t szDirectTranslation = from.GetDirectStrings().size();

    bool sameKeyIndexSize = szKeyOriginal == szKeyTranslation;
    bool sameDirectSize = szDirectOriginal == szDirectTranslation;

    cout << "\tMerging scenario: "
         << " kO: " << szKeyOriginal << ", kT: " << szKeyTranslation
         << ", dO: " << szDirectOriginal << ", dT: " << szDirectTranslation << ": ";

    // Algorithm that matches strings when they are completely exact,
    //   that is, the same number of key strings and the same number
    //   of inner strings
    if (sameKeyIndexSize && sameDirectSize)
    {
        cout << "Exact-" << endl;

        for (size_t i = 0; i < szDirectOriginal; i++)
        {
            GameString gsO = into.GetDirectStrings()[i];
            GameString gsT = from.GetDirectStrings()[i];

            GameString gsN = _AssembleGameString(gsO, gsT);

            into.SetDirect(i, gsN);
        }

        return;
    }

    // Algorithm that matches strings when there is the same number of
    //   key strings, but different number of inner strings. When
    //   key string has a different number of inner strings, then it
    //   moves onto the next key string to process
    if (sameKeyIndexSize && !sameDirectSize)
    {
        cout << "ExactKeyDiffDirect-" << endl;

        for(size_t i = 0; i < szKeyOriginal; i++)
        {
            // Key indexes for Original and Translation
            KeyIndex kO = into.GetKeyStrings()[i];
            KeyIndex kT = from.GetKeyStrings()[i];

            // Number of innner strings of the block doesn't match
            if (kO.numInnerStrings != kT.numInnerStrings)
            {
                cout << "\t\tInvalid key index block: " << i + 1 << endl;
                continue;
            }

            for (size_t j = 0; j <= kO.numInnerStrings; j++)
            {
                GameString gsO = into.GetDirectStrings()[kO.index + j];
                GameString gsT = from.GetDirectStrings()[kT.index + j];

                GameString gsN = _AssembleGameString(gsO, gsT);

                into.SetDirect(kO.index + j, gsN);
            }
        }

        return;
    }

    // The last resort algorithm in the worst case scenario, when there
    //   is a different number of key strings. It starts in the normal
    //   order, then once it encounters a difference in number of inner
    //   strigns it attempts to match strings in the reverse order instead.
    cout << "DiffKey-" << endl;

    size_t keyIndexReached = 0;

    for(size_t i = 0; i < szKeyOriginal; i++)
    {
        // Key indexes for Original and Translation
        KeyIndex kO = into.GetKeyStrings()[i];
        KeyIndex kT = from.GetKeyStrings()[i];

        // Number of innner strings of the block doesn't match
        if (kO.numInnerStrings != kT.numInnerStrings)
        {
            cout << "\t\tInvalid key index block: " << i + 1 << endl;

            keyIndexReached = i;

            break;
        }

        for (size_t j = 0; j <= kO.numInnerStrings; j++)
        {
            GameString gsO = into.GetDirectStrings()[kO.index + j];
            GameString gsT = from.GetDirectStrings()[kT.index + j];

            GameString gsN = _AssembleGameString(gsO, gsT);

            into.SetDirect(kO.index + j, gsN);
        }
    }

    for(size_t i = 0; i < szKeyOriginal; i++)
    {
        // Also stop when overlap starts to happen
        if (i >= szKeyOriginal - keyIndexReached)
        {
            break;
        }

        // Key indexes for Original and Translation
        KeyIndex kO = into.GetKeyStrings()[szKeyOriginal - i];
        KeyIndex kT = from.GetKeyStrings()[szKeyTranslation - i];

        // Number of innner strings of the block doesn't match
        if (kO.numInnerStrings != kT.numInnerStrings)
        {
            cout << "\t\tInvalid key index block: " << i + 1 << endl;
            break;
        }

        for (size_t j = 0; j <= kO.numInnerStrings; j++)
        {
            GameString gsO = into.GetDirectStrings()[kO.index + j];
            GameString gsT = from.GetDirectStrings()[kT.index + j];

            GameString gsN = _AssembleGameString(gsO, gsT);

            into.SetDirect(kO.index + j, gsN);
        }
    }
}

GameString Merger_SG_Android_ant08::_AssembleGameString(const GameString & inJap, const GameString & inEng) const
{
    GameString ret(ttRaw);

    bool messageInsterted = false;
    for(auto const & token : inJap.GetTokens())
    {
        if (token.type == ttName)
        {
            ret.AddNewTokenWithData(token.type, token.data);
            continue;
        }

        if (token.type == ttRaw)
        {
            if (!messageInsterted)
            {
                messageInsterted = true;

                for(auto const & tokenEng : inEng.GetTokens())
                {
                    ret.AddNewTokenWithData(tokenEng.type, tokenEng.data);
                }
            }
            else if (token.data.size() < 3) // a hack to ignore all big chunks of original text, but still copy the random unicode symbols in the endings sequence
            {
                ret.AddNewTokenWithData(token.type, token.data);
            }

            continue;
        }

        if (token.type == ttControlCode)
        {
            // Ignore <DICT> and </DICT>
            if (token.data[token.data.size() - 2] == 'T')
            {
                continue;
            }

            ret.AddNewTokenWithData(token.type, token.data);
        }
    }

    return ret;
}
