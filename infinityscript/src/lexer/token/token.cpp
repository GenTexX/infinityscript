#include "infscrpch.h"
#include "token.h"
#include "tokenwords.h"

namespace infinity {

	

	Token::Token() {
		m_val = nullptr;
		m_type = TokenType::Unknown;
	}

	Token::Token(const char* val, const TokenType& type) {
		m_type = type;
		size_t val_len = strlen(val);
		m_val = new char[val_len + 1];
		strcpy(m_val, val);
	}
	Token::~Token() {
		delete[] m_val;
	}
	void Token::setValue(const char* val) {
		m_val = new char[strlen(val) + 1];
		strcpy(m_val, val);

	}
	void Token::setType(const TokenType& type) {
		m_type = type;
	}
	Token* Token::tokenize(const char* src) {

		size_t srclen = strlen(src);
		size_t idx = 0;
		Token* tokens = new Token[srclen];
		size_t tokens_idx = 0;

		while (idx < srclen) {
			if (src[idx] == INFINITY_OPEN_PAREN) {
				tokens[tokens_idx++].setType(TokenType::OpenParen);
			} else if (src[idx] == INFINITY_CLOSE_PAREN) {
				tokens[tokens_idx++].setType(TokenType::CloseParam);
			} else if (src[idx] == INFINITY_PLUS || src[idx] == INFINITY_MINUS || src[idx] == INFINITY_MULTIPLY || src[idx] == INFINITY_DIVIDE) {
				tokens[tokens_idx++].setType(TokenType::BinaryOp);
			} else if (src[idx] == INFINITY_EQUALS) {
				tokens[tokens_idx++].setType(TokenType::Equals);
			} else {
				//MULTI CHARACTER TOKEN
				if (isAlphabetical(src[idx])) {
					size_t begin_idx = idx;
					while (isAlphabetical(src[idx])) {
						idx++;
					}
					size_t wordlen = (idx - begin_idx);
					char* tmp = new char[wordlen + 1];
					while (idx > begin_idx) {
						tmp[wordlen - (idx - begin_idx)] = src[begin_idx++];
					}

					if (!strcmp(tmp, INFINITY_LET)) {
						tokens[tokens_idx++].setType(TokenType::Let);
					} else {
						//IDENTIFIER
						tokens[tokens_idx].setValue(tmp);
						tokens[tokens_idx++].setType(TokenType::Identifier);
					}

					delete[] tmp;
				} else if (isNumerical(src[idx])) {
					size_t begin_idx = idx;
					while (isNumerical(src[idx])) {
						idx++;
					}
					size_t wordlen = (idx - begin_idx);
					char* tmp = new char[wordlen + 1];
					while (idx > begin_idx) {
						tmp[wordlen - (idx - begin_idx)] = src[begin_idx++];
					}

					//IDENTIFIER
					tokens[tokens_idx].setValue(tmp);
					tokens[tokens_idx++].setType(TokenType::Number);
					

					delete[] tmp;
				} else if (isSkippable(src[idx])) {
					//NOTHING TODO FOR NOW
				} else {
					__debugbreak();
				}

			}

			idx++;
		}

		return tokens;
	}
}