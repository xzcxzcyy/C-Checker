#include <string>

class Token {
public:
    enum TokenType { IDENTIFIER, NUMBER, OPEN_PARAMETER, CLOSE_PARAM, CLOSE_PARAM };
    TokenType type;
    std::string name;

    Token(std::string name) {
        this->name = name;
        // TODO: handle TokenType
    }
};
