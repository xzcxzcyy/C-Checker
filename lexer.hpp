#include <fstream>
#include <string>
#include <vector>

class Lexer {
private:
    std::ifstream inputStream;

public:
    Lexer(std::string fileName) {
        inputStream = std::ifstream(fileName);
    }

    ~Lexer() {
        inputStream.close();
    }

};
