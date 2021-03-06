/**
 * <h1>JavaStringToken</h1>
 *
 * <p> Java string tokens.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include "JavaStringToken.h"
#include "../JavaError.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java;

JavaStringToken::JavaStringToken(Source *source) throw (string)
    : JavaToken(source)
{
    extract();
}

void JavaStringToken::extract() throw (string)
{
    string value_str = "";
    char current_ch = next_char();  // consume initial quote
    text += "\"";

    // Get string characters.
    do
    {
        // Replace any whitespace character with a blank.
        if (isspace(current_ch)) current_ch = ' ';

        if(current_ch == '\\'){
            text += '\\';
            current_ch = next_char();
            text += current_ch;
            switch(current_ch){
                case 'n':   value_str += "\n";
                            break;
                case 't':   value_str += "\t";
                            break;
                default:   value_str += current_ch;

            }
            current_ch = next_char();
        }

        if ((current_ch != '\"') && (current_ch != EOF) && (current_ch != '\\'))
        {
            text += current_ch;
            value_str  += current_ch;
            current_ch = next_char();  // consume character
        }


    } while ((current_ch != '\"') && (current_ch != Source::END_OF_FILE));

    if (current_ch == '\"')
    {
        text += "\"";
        next_char();  // consume final quote
        type = (TokenType) PT_STRING;
        value = new DataValue(value_str);
    }
    else
    {
        type = (TokenType) PT_ERROR;
        value = new DataValue((int) UNEXPECTED_EOF);
    }
}

}}}}  // namespace wci::frontend::Java::tokens
