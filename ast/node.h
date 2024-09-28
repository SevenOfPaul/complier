#pragma once
#include <token/token.h>
#include <unordered_map>
using namespace lisp::token;

namespace lisp {
    namespace ast {
        class Node {
        public:
            enum Type {
                NODE_INEGER = 0,
                NODE_INFIX,
                NODE_EXPRESSION_STATEMENT,
                NODE_PROGRAM,
                NODE_PREFIX,
            };

            Node(){};

            Node(Type type) : type(type) {}

            virtual ~Node() {}

            Type get_type() const { return type; }

            string get_name() const;
            Type type;
            Token token;
            static unordered_map<Type, string> names;
        };


        class Statement : public Node {
        public:
            Statement(){}

            Statement(Type type) : Node(type) {}

            ~Statement() {}
        };

    } // namespace ast
} // namespace lisp
