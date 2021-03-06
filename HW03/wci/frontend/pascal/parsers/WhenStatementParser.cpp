
#include <string>
#include <set>
#include "WhenStatementParser.h"
#include "StatementParser.h"
#include "ExpressionParser.h"
#include "../PascalParserTD.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../Token.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;
// COPIED DIRECTLY FROM WHILESTATEMENTPARSER
bool WhenStatementParser::INITIALIZED = false;

set<PascalTokenType> WhenStatementParser::DO_SET;

void WhenStatementParser::initialize()
{
    if (INITIALIZED) return;

    DO_SET = StatementParser::STMT_START_SET;
    DO_SET.insert(PascalTokenType::DO);

    set<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
        DO_SET.insert(*it);
    }

    INITIALIZED = true;
}

WhenStatementParser::WhenStatementParser(PascalParserTD *parent)
    : StatementParser(parent)
{
    initialize();
}

ICodeNode *WhenStatementParser::parse_statement(Token *token) throw (string)
{
    // TODO: I made current token call randomly because for some reasons errors were happening that
    // resulted in stack dumps
    token = next_token(token);  // consume the WHEN
    vector<ICodeNode*> children;
    // no op node for testing
    ICodeNode* root_node = ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN);
    ICodeNode* tmp_node;
    // text for each statemtn on the side of the =>
    string statement_text;
    ExpressionParser e(this);
    StatementParser s(this);

    // look through and prase all branches until otherwise keyword encountered
    while(token->get_text() != "OTHERWISE"){
        children.clear();
        tmp_node = parse_branch(token);
        children = tmp_node->get_children();

        if(children.size() == 2)
        {
            root_node->add_child(children[0]);
            root_node->add_child(children[1]);
        }
        else
        {
            token = synchronize(DO_SET);
            error_handler.flag(token, MISSING_SEMICOLON, this);
            return nullptr;
        }
        token = current_token();

    }

    // form otherwise statement node
    token = next_token(token); // consume otherwise
    if(token->get_text() != "=>") ;// throw error here

    token = next_token(token); // consume =>

    // create compound node, store otherwise statement, then add to root node
    tmp_node = ICodeFactory::create_icode_node((ICodeNodeType) NT_COMPOUND);
    tmp_node->add_child(s.parse_statement(token));
    
    token = current_token();
    token = next_token(token);

    root_node->add_child(tmp_node);
    return root_node;
}

ICodeNode *WhenStatementParser::parse_branch(Token *token) throw (string)
{

    // delcare needed nodes
    ICodeNode* root_node = ICodeFactory::create_icode_node((ICodeNodeType) NT_COMPOUND);
    ICodeNode* test_node = ICodeFactory::create_icode_node((ICodeNodeType) NT_TEST);
    ICodeNode* statement_node = ICodeFactory::create_icode_node((ICodeNodeType) NT_COMPOUND);

    token = current_token();

    ExpressionParser e(this);
    StatementParser s(this);

    // parsed expression to test_node
    test_node->add_child(e.parse_statement(token));

    token = current_token();
    if(token->get_text() != "=>")
    {
        error_handler.flag(token, MISSING_LAMBDA, this); // temporary error should make specific for missing lambda
        return root_node;
    }
    else token = next_token(token);

    // add test node to root
    root_node->add_child(test_node);

    // add parsed statement to statement node
    statement_node->add_child(s.parse_statement(token));
    root_node->add_child(statement_node);

    token = current_token();
    token = next_token(token); // skip semicolon



    return root_node;
}

}}}}