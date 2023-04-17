#pragma once
#include "tokentype.h"
#include "lexer\lexerError.h"

namespace infinity {

	class Token {
	public:
		Token();
		
		void setValue(const char* val);
		const char* getValue() { return m_val; }
		void setType(const TokenType& type);
		TokenType getType() { return m_type; }
		void setLine(const size_t& type);
		size_t getLine() { return m_line; }
		void setLinePosition(const size_t& line_position);
		size_t getLinePosition() { return m_line_position; }

		static std::pair<LexerError*, Token*> tokenize(const char* src, bool skipNewLine = false);

		std::string toString();

	private:
		char*		m_val;
		TokenType	m_type;
		size_t		m_line;
		size_t		m_line_position;

	};

}