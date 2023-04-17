#include <iostream>

#include <lexer\lexer.h>
#include <logger\logger.h>


int main() {

	infinity::Logger::init();
	infinity::Lexer lexer("assets/test_basic_3.is");

	auto tokens = lexer.analyse();

	size_t idx = 0;
	//while (tokens != nullptr && tokens[idx].getType() != infinity::End) {
	//	std::cout << tokens[idx++].toString();
	//}

	delete[] tokens;

	return 0;
}
