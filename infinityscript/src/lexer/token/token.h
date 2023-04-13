#pragma once

#include "tokentype.h"

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

		static Token* tokenize(const char* src);

	private:
		char* m_val;
		TokenType m_type;

	};

}