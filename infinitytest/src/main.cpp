#include <iostream>

#include <lexer\lexer.h>


int main() {

	infinity::Lexer lexer("assets/test_basic_1.is");

	std::cout << lexer.hello() << std::endl;

	return 0;
}
