#include <lexer/lexer.h>
#include <iostream>
#include <parser/parser.h>
#include <sstream>
using namespace lisp::parser;
using namespace lisp::lexer;
using namespace lisp::ast;
Parser::Parser() {}
Parser::Parser(const std::shared_ptr<Lexer> &lexer) {
    // 调用两次 第一次定义 tokencur  第二次调用获取toen next
    next_token();
    next_token();
}
Parser::~Parser() {}
std::unordered_map<Token::Type, int> Parser::precedences = {{Token::TOKEN_PLUS, SUM},
                                                            {Token::TOKEN_MINUS, SUM},
                                                            {Token::TOKEN_ASTERISK, PROODCT},
                                                            {Token::TOKEN_SLASH, PROODCT}};
std::unordered_map<Token::Type, Parser::prefix_parse_fn> Parser::prefix_parse_fns = {
    {Token::TOKEN_INTEGER, &Parser::parse_integer},
{Token::TOKEN_LPAREN, &Parser::parse_group},
};
std::unordered_map<Token::Type, Parser::infix_parse_fn> Parser::infix_parse_fns = {
        {Token::TOKEN_PLUS, &Parser::parse_infix},
        {Token::TOKEN_MINUS, &Parser::parse_infix},
        {Token::TOKEN_ASTERISK, &Parser::parse_infix},
        {Token::TOKEN_SLASH, &Parser::parse_infix}
};
void Parser::next_token() {
    cur = next;
    // 调用词法分析的nextToken 生成下一个token
    next = lexer->next_token();
}
std::shared_ptr<Expression> Parser::parse_integer() {
    //声明expression e
std::shared_ptr<Integer> e(new Integer());
    e->token=cur;
    //string转换为int
    e->val=std::atol(cur.get_literal().c_str());
   return e;
}
std::shared_ptr<Expression> Parser::parse_group() {
    //声明expression e
    next_token();
    //这里为什么
    auto e=parse_expression(LOWEST);
    if (!expect_next_token(Token::TOKEN_RPAREN)) {
        return nullptr;
    }
    return e;
}
std::shared_ptr<Expression> Parser::parse_expression(int pracedence) {
 auto prefix_fn=prefix_parse_fns.find(cur.get_type());
    if(prefix_fn==prefix_parse_fns.end()) {
        std::cout<<"当前token未定义";
    }
    //找到了这个符号的表达式生成函数 调用返回表达式
    std::shared_ptr<Expression> e(this->*prefix_fn->second());
    //下一个符号的优先级比他小 就说明这是个中缀表达式
    while(!next_token_is(Token::TOKEN_SEMICLON)&&(pracedence<get_next_token_precedence())) {
          auto infix_fn=infix_parse_fns.find(next.get_type());
              if(infix_fn==infix_parse_fns.end()) {
               return e;
              }else {
                  next_token();
                  e=this->*infix_fn->second();
              }
    }
    return e;
}
bool Parser::cur_token_is(Token::Type type) { return cur.get_type() == type; }
bool Parser::next_token_is(Token::Type type) { return next.get_type() == type; }
void Parser::next_error(Token::Type type) {
    std::ostringstream oss;
    oss << "expected next token to be" << type << ", but got" << next.get_type() << "instead";
    errors.push_back(oss.str());
}
bool Parser::expect_next_token(Token::Type type) {
    if (next_token_is(type)) {
        next_token();
        return true;
    }
    // 抛出错误
    next_error(type);
    return false;
}
int Parser::get_cur_token_precedence() {
    auto it = precedences.find(cur.get_type());
    if (it != precedences.end()) {
        return it->second;
    }
    return LOWEST;
};
int Parser::get_next_token_precedence() {
    auto it = precedences.find(next.get_type());
    if (it != precedences.end()) {
        return it->second;
    }
    return LOWEST;
}
void Parser::no_prefix_parse_fn_error(Token::Type type) {
std::ostringstream oss;
    oss<<"no_prefix_parse_fn_error"<<type;
    errors.push_back(oss.str());
}
//中缀表达式
std::shared_ptr<Expression> Parser::parse_infix(const std::shared_ptr<Expression> &left) {
 std::shared_ptr<Infix> e(new Infix());
    e->token=cur;
    e->operation=cur.get_literal();
    e->left=left;
     int precedence=get_cur_token_precedence();
    e->right=parse_expression(precedence);
return e;
}

