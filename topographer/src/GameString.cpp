///////////////////////////////////////////////////////////////
//                                                           //
//   GameString.cpp                                          //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "GameString.h"

using namespace std;
using namespace topographer;

// Construct GameString with some kind of the default TokenType.
//   defaultTokenType, TokenType, this type is used when appending
//     to the last token and the token itself doesn't exist
GameString::GameString(TokenType defaultTokenType)
{
    _defaultTT = defaultTokenType;
}

GameString::~GameString() { }

// Appends data to the last token in this GameString. If there are
//   no tokens prior to this, then a new token will be added with the
//   defaultTokenType type specified in the GameString's constructor.
//   data, string, the data to append
void GameString::AppendToLastToken(const string & data)
{
    AppendToLastToken(BArr(data.begin(), data.end()));
}

// Appends data to the last token in this GameString. If there are
//   no tokens prior to this, then a new token will be added with the
//   defaultTokenType type specified in the GameString's constructor.
//   data, BArr, data to append
void GameString::AppendToLastToken(const BArr & data)
{

    if (data.empty())
    {
        return;
    }

    // Creates a first token with default token type and specified data
    if (_tokens.empty())
    {
        _tokens.push_back({
            _defaultTT,
            data
        });

        return;
    }

    // Literally appends data to the last token's data
    _tokens.back().data.insert(
        _tokens.back().data.end(),
        data.begin(),
        data.end()
    );
}

// Adds a new empty token with the defaultTokenType type specified in
//   the GameString's constructor.
void GameString::AddNewDefaultToken()
{
    AddNewToken(_defaultTT);
}

// Adds a new empty token.
//   tokenType, TokenType, type of the new token to add
void GameString::AddNewToken(TokenType tokenType)
{
    _tokens.push_back({
        tokenType,
        { }
    });
}

// Adds a new token with data
//   tokenType, TokenType, type of the new token to add
//   data, string, data to put into the token
void GameString::AddNewTokenWithData(TokenType tokenType, const string & data)
{
    AddNewTokenWithData(tokenType, BArr(data.begin(), data.end()));
}

// Adds a new token with data
//   tokenType, TokenType, type of the new token to add
//   data, BArr, data to put into the token
void GameString::AddNewTokenWithData(TokenType tokenType, const BArr & data)
{
    _tokens.push_back({
        tokenType,
        data
    });
}

// Clears the tokens
void GameString::Clear()
{
    _tokens.clear();
}

// Returns a constant reference to the vector of tokens.
// This is useful when accessing tokens directly.
const std::vector<StringToken> & GameString::GetTokens() const
{
    return _tokens;
}
