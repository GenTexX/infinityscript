#include "infscrpch.h"
#include "token.h"
#include "tokenwords.h"
#include "lexer\lexer.h"

#include <string>
#include <format>

#define MAX_LEXER_ERROR_BEFORE_BREAK 31
#define POST_TOKENIZE_LOG_TEMPLATE_SUCCESS "Tokenized \033[1;34m{}\033[0m characters into \033[1;35m{}\033[0m tokens in \033[1;32m{}\033[0m microseconds with \033[1;32m{}\033[0m error(s)"
#define POST_TOKENIZE_LOG_TEMPLATE_ERROR "Tokenized \033[1;34m{}\033[0m characters into \033[1;35m{}\033[0m tokens in \033[1;32m{}\033[0m microseconds with \033[1;31m{}\033[0m error(s)"

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
	std::pair<LexerError*, Token*> Token::tokenize(const char* src, bool skipNewLine) {
		auto begin = std::chrono::high_resolution_clock::now();

		size_t srclen = strlen(src);
		size_t idx = 0;
		Token* tokens = new Token[srclen];
		LexerError* errors = new LexerError[MAX_LEXER_ERROR_BEFORE_BREAK + 1];
		size_t tokens_idx = 0;
		size_t errors_idx = 0;

		size_t line_number = 1;
		size_t line_pos = 1;


		while (idx <= srclen) {
			if (src[idx] == INFINITY_OPEN_PAREN) {
				tokens[tokens_idx++].setType(TokenType::OpenParen);
			} else if (src[idx] == INFINITY_CLOSE_PAREN) {
				tokens[tokens_idx++].setType(TokenType::CloseParen);
			} else if (src[idx] == INFINITY_OPEN_CURLY) {
				tokens[tokens_idx++].setType(TokenType::OpenCurly);
			} else if (src[idx] == INFINITY_CLOSE_CURLY) {
				tokens[tokens_idx++].setType(TokenType::CloseCurly);
			} else if (src[idx] == INFINITY_OPEN_BRACKET) {
				tokens[tokens_idx++].setType(TokenType::OpenBracket);
			} else if (src[idx] == INFINITY_CLOSE_BRACKET) {
				tokens[tokens_idx++].setType(TokenType::CloseBracket);
			} else if (src[idx] == INFINITY_PLUS || src[idx] == INFINITY_MINUS || src[idx] == INFINITY_MULTIPLY || (src[idx] == INFINITY_DIVIDE && src[idx+1] != INFINITY_DIVIDE)) {
				char tmp[2];
				tmp[0] = src[idx];
				tmp[1] = 0;
				tokens[tokens_idx].setValue(tmp);
				tokens[tokens_idx++].setType(TokenType::BinaryOp);
			} else if (src[idx] == INFINITY_EQUALS && src[idx + 1] != INFINITY_EQUALS) {
				tokens[tokens_idx++].setType(TokenType::Equals);
			} else if (src[idx] == INFINITY_COLON) {
				tokens[tokens_idx++].setType(TokenType::Colon);
			} else if (src[idx] == INFINITY_SEMICOLON) {
				tokens[tokens_idx++].setType(TokenType::Semicolon);
			} else if (src[idx] == 0) {
				tokens[tokens_idx++].setType(TokenType::End);
			} else if (src[idx] == INFINITY_NEWLINE) {
				if(!skipNewLine)
					tokens[tokens_idx++].setType(TokenType::NewLine);
				line_number++;
				line_pos = 0;
			} else if (src[idx] == INFINITY_AMPERSAND) {
				tokens[tokens_idx++].setType(TokenType::Ampersand);
			} else if (src[idx] == INFINITY_QUESTIONMARK) {
				tokens[tokens_idx++].setType(TokenType::QuestionMark);
			} else if (src[idx] == INFINITY_CALLSIGN && src[idx] != INFINITY_EQUALS) {
				tokens[tokens_idx++].setType(TokenType::CallSign);
			} else if (src[idx] == INFINITY_PERCENTSIGN) {
				tokens[tokens_idx++].setType(TokenType::PercentSign);
			} else if (src[idx] == INFINITY_COMMA) {
				tokens[tokens_idx++].setType(TokenType::Comma);
			} else if (src[idx] == INFINITY_DOT) {
				tokens[tokens_idx++].setType(TokenType::Dot);
			} else if (src[idx] == INFINITY_HASH) {
				tokens[tokens_idx++].setType(TokenType::Hash);
			} else if (src[idx] == INFINITY_TILDE) {
				tokens[tokens_idx++].setType(TokenType::Tilde);
			} else if (src[idx] == INFINITY_SINGLE_QUOTATIONMARK && (src[idx + 2] == INFINITY_SINGLE_QUOTATIONMARK || src[idx + 3] == INFINITY_SINGLE_QUOTATIONMARK)) {
				idx++;
				char tmp[5];
				size_t tmp_idx = 0;
				if (getChar(src[idx])) {	//if char is f.e. \n
					tmp[tmp_idx++] = src[idx];
					idx++;
				}
				tmp[tmp_idx++] = src[idx];
				idx++;
				tmp[tmp_idx] = 0;

				tokens[tokens_idx].setValue(tmp);
				tokens[tokens_idx++].setType(TokenType::CharacterLiteral);
			} else if ((src[idx] == INFINITY_LESSTHAN && src[idx + 1] != INFINITY_EQUALS) || (src[idx] == INFINITY_GREATERTHAN && src[idx + 1] != INFINITY_EQUALS)) {
				char tmp[2];
				tmp[0] = src[idx];
				tmp[1] = 0;
				tokens[tokens_idx].setValue(tmp);
				tokens[tokens_idx++].setType(TokenType::CompareOp);
			} else {

				//MULTI CHARACTER 
				if ((src[idx] == INFINITY_EQUALS && src[idx + 1] == INFINITY_EQUALS) || (src[idx] == INFINITY_LESSTHAN && src[idx + 1] == INFINITY_EQUALS) || (src[idx] == INFINITY_GREATERTHAN && src[idx + 1] == INFINITY_EQUALS)) {
					char tmp[3];
					tmp[0] = src[idx++];
					tmp[1] = src[idx];
					tmp[2] = 0;
					tokens[tokens_idx].setValue(tmp);
					tokens[tokens_idx++].setType(TokenType::CompareOp);
				} else if (src[idx] == INFINITY_QUOTATIONMARK) {
					idx += 1;
					size_t begin_idx = idx;
					while (src[idx] != 0 && src[idx] != INFINITY_QUOTATIONMARK) {
						idx++;
					}

					size_t stringlen = (idx - begin_idx);
					char* string = new char[stringlen + 1];

					while (idx > begin_idx) {
						size_t string_idx = stringlen - (idx - begin_idx);
						string[string_idx] = src[begin_idx++];
					}
					string[stringlen] = 0;

					tokens[tokens_idx].setValue(string);
					tokens[tokens_idx++].setType(TokenType::StringLiteral);
					delete[] string;
				} else if (src[idx] == INFINITY_DIVIDE && src[idx + 1] == INFINITY_DIVIDE) {
					idx += 2;
					size_t begin_idx = idx;
					while (src[idx] != 0 && src[idx] != '\n') {
						idx++;
					}
					idx--;
				} else if (isAlphabetical(src[idx]) || src[idx] == '_') {
					size_t begin_idx = idx;
					while (isAlphabetical(src[idx]) || isNumerical(src[idx]) || src[idx] == '_') {
						idx++;
						line_pos++;
					}
					size_t wordlen = (idx - begin_idx);
					char* tmp = new char[wordlen + 1];
					while (idx > begin_idx) {
						size_t tmp_idx = wordlen - (idx - begin_idx);
						tmp[tmp_idx] = src[begin_idx++];
					}
					tmp[wordlen] = 0;
					idx--;

					//KEYWORDS
					if (!strcmp(tmp, INFINITY_LET)) {
						tokens[tokens_idx++].setType(TokenType::Let);
					} else if (!strcmp(tmp, INFINITY_FOR)) {
						tokens[tokens_idx++].setType(TokenType::For);
					} else if (!strcmp(tmp, INFINITY_WHILE)) {
						tokens[tokens_idx++].setType(TokenType::While);
					} else if (!strcmp(tmp, INFINITY_DO)) {
						tokens[tokens_idx++].setType(TokenType::Do);
					} else if (!strcmp(tmp, INFINITY_BREAK)) {
						tokens[tokens_idx++].setType(TokenType::Break);
					} else if (!strcmp(tmp, INFINITY_CONTINUE)) {
						tokens[tokens_idx++].setType(TokenType::Continue);
					} else if (!strcmp(tmp, INFINITY_IF)) {
						tokens[tokens_idx++].setType(TokenType::If);
					} else if (!strcmp(tmp, INFINITY_VOID)) {
						tokens[tokens_idx++].setType(TokenType::Void);
					} else if (!strcmp(tmp, INFINITY_NULL)) {
						tokens[tokens_idx++].setType(TokenType::Null);
					} else if (!strcmp(tmp, INFINITY_CLASS)) {
						tokens[tokens_idx++].setType(TokenType::Class);
					} else if (!strcmp(tmp, INFINITY_STRUCT)) {
						tokens[tokens_idx++].setType(TokenType::Struct);
					} else if (!strcmp(tmp, INFINITY_PUBLIC)) {
						tokens[tokens_idx++].setType(TokenType::Public);
					} else if (!strcmp(tmp, INFINITY_PROTECTED)) {
						tokens[tokens_idx++].setType(TokenType::Protected);
					} else if (!strcmp(tmp, INFINITY_PRIVATE)) {
						tokens[tokens_idx++].setType(TokenType::Private);
					} else if (!strcmp(tmp, INFINITY_INT)) {
						tokens[tokens_idx++].setType(TokenType::Int);
					} else if (!strcmp(tmp, INFINITY_FLOAT)) {
						tokens[tokens_idx++].setType(TokenType::Float);
					} else if (!strcmp(tmp, INFINITY_STRING)) {
						tokens[tokens_idx++].setType(TokenType::String);
					} else if (!strcmp(tmp, INFINITY_CHAR)) {
						tokens[tokens_idx++].setType(TokenType::Char);
					} else if (!strcmp(tmp, INFINITY_BOOL)) {
						tokens[tokens_idx++].setType(TokenType::Bool);
					} else if (!strcmp(tmp, INFINITY_TABLE)) {
						tokens[tokens_idx++].setType(TokenType::Table);
					} else if (!strcmp(tmp, INFINITY_SWITCH)) {
						tokens[tokens_idx++].setType(TokenType::Switch);
					} else if (!strcmp(tmp, INFINITY_CASE)) {
						tokens[tokens_idx++].setType(TokenType::Case);
					} else if (!strcmp(tmp, INFINITY_TRUE)) {
						tokens[tokens_idx++].setType(TokenType::Case);
					} else if (!strcmp(tmp, INFINITY_FALSE)) {
						tokens[tokens_idx++].setType(TokenType::Case);
					} else if (!strcmp(tmp, INFINITY_SUPER)) {
						tokens[tokens_idx++].setType(TokenType::Super);
					} else if (!strcmp(tmp, INFINITY_THIS)) {
						tokens[tokens_idx++].setType(TokenType::This);
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
						line_pos++;
					}

					if (src[idx] == '.') {
						idx++;
						line_pos++;

						while (isNumerical(src[idx])) {
							idx++;
							line_pos++;
						}

					}

					size_t wordlen = (idx - begin_idx);
					char* tmp = new char[wordlen + 1];
					while (idx > begin_idx) {
						size_t tmp_idx = wordlen - (idx - begin_idx);
						tmp[tmp_idx] = src[begin_idx++];
					}
					tmp[wordlen] = 0;
					idx--;

					//IDENTIFIER
					tokens[tokens_idx].setValue(tmp);
					tokens[tokens_idx++].setType(TokenType::NumberLiteral);


					delete[] tmp;
				} else if (isSkippable(src[idx])) {
					//NOTHING TODO FOR NOW
				} else {
					errors[errors_idx].m_critical = true;
					std::string tmp = std::format("Unknown Token {} at {}:{}", src[idx], line_number, line_pos - 1);
					errors[errors_idx++].m_msg = tmp;

				}
			}

			idx++;
			line_pos++;
		}
		tokens[tokens_idx++].setType(TokenType::OpenParen);

		auto end = std::chrono::high_resolution_clock::now();

		if (errors[0].m_msg.empty()) {
			INFINITY_CORE_INFO(POST_TOKENIZE_LOG_TEMPLATE_SUCCESS, srclen, tokens_idx, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), 0);
		} else {
			INFINITY_CORE_WARN(POST_TOKENIZE_LOG_TEMPLATE_ERROR, srclen, tokens_idx, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), errors_idx);
		}


		return std::pair<LexerError*, Token*>(errors, tokens);
	}
	std::string Token::toString() {

		std::string result = "";

		switch (m_type)
		{
		case infinity::Unknown:
			result += "[Unknown]";
			break;
		case infinity::NumberLiteral:
			result += "[Number(";
			result += m_val;
			result += ")]";
			break;
		case infinity::Identifier:
			result += "[Identifier(";
			result += m_val;
			result += ")]";
			break;
		case infinity::Equals:
			result += "[Equals]";
			break;
		case infinity::OpenParen:
			result += "[OpenParenthesis]";
			break;
		case infinity::CloseParen:
			result += "[CloseParenthesis]";
			break;
		case infinity::BinaryOp:
			result += "[BinaryOperation(";
			result += m_val;
			result += ")]";
			break;
		case infinity::Colon:
			result += "[Colon]";
			break;
		case infinity::Semicolon:
			result += "[Semicolon]";
			break;
		case infinity::Let:
			result += "[Let]";
			break;
		case infinity::NewLine:
			result += "\n";
			break;
		case infinity::OpenCurly:
			result += "[OpenCurlyBracket]";
			break;
		case infinity::CloseCurly:
			result += "[CloseCurlyBracket]";
			break;
		case infinity::OpenBracket:
			result += "[OpenBracket]";
			break;
		case infinity::CloseBracket:
			result += "[CloseBracket]";
			break;
		case infinity::Ampersand:
			result += "[Ampersand]";
			break;
		case infinity::CallSign:
			result += "[Callsign]";
			break;
		case infinity::PercentSign:
			result += "[PercentSign]";
			break;
		case infinity::Comma:
			result += "[Comma]";
			break;
		case infinity::Dot:
			result += "[Dot]";
			break;
		case infinity::QuestionMark:
			result += "[QuestionMark]";
			break;
		case infinity::For:
			result += "[For]";
			break;
		case infinity::While:
			result += "[While]";
			break;
		case infinity::Do:
			result += "[Do]";
			break;
		case infinity::Break:
			result += "[Break]";
			break;
		case infinity::Continue:
			result += "[Continue]";
			break;
		case infinity::If:
			result += "[If]";
			break;
		case infinity::Void:
			result += "[Void]";
			break;
		case infinity::Null:
			result += "[Null]";
			break;
		case infinity::Class:
			result += "[Class]";
			break;
		case infinity::Struct:
			result += "[Struct]";
			break;
		case infinity::Public:
			result += "[Public]";
			break;
		case infinity::Protected:
			result += "[Protected]";
			break;
		case infinity::Private:
			result += "[Private]";
			break;
		case infinity::Int:
			result += "[Int]";
			break;
		case infinity::Float:
			result += "[Float]";
			break;
		case infinity::String:
			result += "[String]";
			break;
		case infinity::Char:
			result += "[Char]";
			break;
		case infinity::Bool:
			result += "[Bool]";
			break;
		case infinity::Table:
			result += "[Table]";
			break;
		case infinity::Switch:
			result += "[Switch]";
			break;
		case infinity::Case:
			result += "[Case]";
			break;
		case infinity::StringLiteral:
			result += "[String(";
			result += m_val;
			result += ")]";
			break;
		case infinity::CharacterLiteral:
			result += "[Char(";
			result += m_val;
			result += ")]";
			break;
		case infinity::CompareOp:
			result += "[CompareOperatpr(";
			result += m_val;
			result += ")]";
			break;
		default:
			result += "------------ERROR------------";
			break;
		}

		return result;
	}
}