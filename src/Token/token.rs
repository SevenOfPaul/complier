use crate::Token::object::Object;
use crate::Token::token_type::Token_type;
use std::collections::HashMap;
use lazy_static::lazy_static;
#[derive(Clone, Debug)]
pub(crate) struct Token {
    token_type: Token_type,
    lexeme: String,
    literal: Option<Object>,
    line: usize,
}
lazy_static! {
    static ref Keywords: HashMap<String, Token_type> = {HashMap::from([
        (String::from("let"), Token_type::LET),
        (String::from("print"), Token_type::PRINT),
        (String::from("fn"), Token_type::FN),
        (String::from("nil"), Token_type::NIL),
    ])};
}
impl Token {
    pub fn new(
        token_type: Token_type,
        lexeme: String,
        literal: Option<Object>,
        line: usize,
    ) -> Token {
        Self {
            token_type,
            lexeme,
            literal,
            line,
        }
    }
    pub fn to_string(&self) -> String {
        let liter = if let Some(l) = &self.literal {
            &l.to_string()
        } else {
            ""
        };
        self.token_type.to_string() + &self.lexeme + liter + &self.line.to_string()
    }
}
