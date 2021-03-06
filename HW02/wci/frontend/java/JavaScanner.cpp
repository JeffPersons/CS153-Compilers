/**
 * <h1>JavaScanner</h1>
 *
 * <p>The Java scanner.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <iostream>
#include "JavaScanner.h"
#include "JavaToken.h"
#include "JavaError.h"
#include "../Source.h"
#include "tokens/JavaWordToken.h"
#include "tokens/JavaNumberToken.h"
#include "tokens/JavaStringToken.h"
#include "tokens/JavaSpecialSymbolToken.h"
#include "tokens/JavaErrorToken.h"
#include "tokens/JavaCharToken.h"

namespace wci { namespace frontend { namespace java {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java::tokens;

JavaScanner::JavaScanner(Source *source) : Scanner(source)
{
}

Token *JavaScanner::extract_token() throw (string)
{
    skip_white_space();

    Token *token;
    char current_ch = current_char();
    string string_ch = " ";

    string_ch[0] = current_ch;

    // Construct the next token.  The current character determines the
    // token type.
    if (current_ch == Source::END_OF_FILE)
    {
        token = nullptr;
    }
    else if (isalpha(current_ch))
    {
        token = new JavaWordToken(source);
    }
    else if (isdigit(current_ch))
    {
        token = new JavaNumberToken(source);
    }
    /* This might actually be a char token
    else if (current_ch == '\'')
    {
        token = new JavaStringToken(source);
    }*/
    // changed to double quote
    else if(current_ch == '\''){
        token = new JavaCharToken(source);
    }
    else if (current_ch == '\"'){
        token = new JavaStringToken(source);
    }
    else if (JavaToken::SPECIAL_SYMBOLS.find(string_ch)
                != JavaToken::SPECIAL_SYMBOLS.end())
    {
        token = new JavaSpecialSymbolToken(source);
    }
    else
    {
        token = new JavaErrorToken(source, INVALID_CHARACTER, string_ch);
        next_char();  // consume character
    }

    return token;
}

void JavaScanner::skip_white_space() throw (string)
{
    char current_ch = current_char();
    char next_ch;
    // Add support for long comments utilizing /**/ and standard //
    while (isspace(current_ch) || ((current_ch == '/') && (peek_char() == '/' || peek_char() == '*')))  {

        // Start of a comment?
        if (current_ch == '/')
        {
            if(peek_char() == '*'){
                current_ch = next_char(); // consume *
                do
                {
                    current_ch = next_char();  // consume comment characters
                    // Found closing '}'?
                    if (current_ch == '*' && peek_char() == '/') {
                        current_ch = next_char();  // consume the '*'
                        current_ch = next_char(); // consume the /
                        break; // get out of while loop
                    }
                } while ((current_ch != Source::END_OF_FILE));
            }
            else if(peek_char() == '/'){
                while(current_ch != '\n') current_ch = next_char();
                current_ch = next_char(); // consume \n
            }
        }
        // Not a comment.
        else if(isspace(current_ch)) current_ch = next_char();  // consume whitespace character
    }
}

}}} // namespace wci::frontend::Java
