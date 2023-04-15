#pragma once
#include "tokentype.h"
#include "lexer\lexerError.h"

namespace infinity {

	class Token {
	public:
		Token();
		Token(const char* val, const TokenType& type);
		~Token();
		
		void setValue(const char* val);
		const char* getValue() { return m_val; }
		void setType(const TokenType& type);
		TokenType getType() { return m_type; }

		static std::pair<LexerError*, Token*> tokenize(const char* src, bool skipNewLine = false);

		std::string toString();

	private:
		char* m_val;
		TokenType m_type;

	};

}