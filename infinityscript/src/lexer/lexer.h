#pragma once
#include <queue>
#include <string>
#include <format>
#include <logger\logger.h>

#include "token\token.h"
#include "lexer\lexerError.h"

namespace infinity {

	class Lexer {
	public:
		Lexer(const char* filename);
		~Lexer();

		const char* getSrc() { return m_src; }


		Token* analyse();

	private:
		char* m_src;
		char* m_filename;

		std::queue<LexerError> m_error_queue;
		bool checkErrors();

	};

}