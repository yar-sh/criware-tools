///////////////////////////////////////////////////////////////
//                                                           //
//   GameString.h                                            //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_GAME_STRING_H
#define TOPOGRAPHER_GAME_STRING_H

#include "Types.h"

namespace topographer
{

    // A string in the game. Can be used for any language. The
    //   string consists of so called "tokens". You can refer to
    //   TokenType enum to see what each token type represents.
    class GameString
    {
    public:
        // Construct GameString with some kind of the default TokenType.
        //   defaultTokenType, TokenType, this type is used when appending
        //     to the last token and the token itself doesn't exist
        GameString(TokenType defultTokenType);

        ~GameString();

        // Appends data to the last token in this GameString. If there are
        //   no tokens prior to this, then a new token will be added with the
        //   defaultTokenType type specified in the GameString's constructor.
        //   data, string, the data to append
        void AppendToLastToken(const std::string & data);

        // Appends data to the last token in this GameString. If there are
        //   no tokens prior to this, then a new token will be added with the
        //   defaultTokenType type specified in the GameString's constructor.
        //   data, BArr, data to append
        void AppendToLastToken(const BArr & data);

        // Adds a new empty token with the defaultTokenType type specified in
        //   the GameString's constructor.
        void AddNewDefaultToken();

        // Adds a new empty token.
        //   tokenType, TokenType, type of the new token to add
        void AddNewToken(TokenType tokenType);

        // Adds a new token with data
        //   tokenType, TokenType, type of the new token to add
        //   data, string, data to put into the token
        void AddNewTokenWithData(TokenType tokenType, const std::string & data);

        // Adds a new token with data
        //   tokenType, TokenType, type of the new token to add
        //   data, BArr, data to put into the token
        void AddNewTokenWithData(TokenType tokenType, const BArr & data);

        // Clears the tokens
        void Clear();

        // Returns a constant reference to the vector of tokens.
        // This is useful when accessing tokens directly.
        const std::vector<StringToken> & GetTokens() const;

    private:
        // Vector of tokens
        std::vector<StringToken> _tokens;

        // Default token type when appending and there are no tokens
        TokenType _defaultTT;
    };
}

#endif
