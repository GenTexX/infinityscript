#include "infscrpch.h"
#include "token.h"
#include "tokenwords.h"
#include "lexer\lexer.h"

#include <string>
#include <format>

#define MAX_LEXER_ERROR_BEFORE_BREAK 31
#define POST_TOKENIZE_LOG_TEMPLATE_SUCCESS "Tokenized \033[1;34m{}\033[0m characters into \033[1;35m{}\033[0m tokens in \033[1;33m{}\033[0m microseconds with \033[1;32m{}\033[0m error(s)"
#define POST_TOKENIZE_LOG_TEMPLATE_ERROR "Tokenized \033[1;34m{}\033[0m characters into \033[1;35m{}\033[0m tokens in \033[1;33m{}\033[0m microseconds with \033[1;31m{}\033[0m error(s)"

namespace infinity {


	struct TokenInfo {

		//Sourcecode
		const char* src;
		size_t src_len;
		
		//Tokens and Errors
		Token* tokens;
		LexerError* errors;

		//Index
		size_t tokens_idx;
		size_t src_idx;
		size_t errors_idx;

		//Position
		size_t line;
		size_t line_position;


		bool is_at_end() { return src_idx <= src_len; }

		void set_token(TokenType type) {
			tokens[tokens_idx].setValue(nullptr);
			tokens[tokens_idx].setType(type);
			tokens[tokens_idx].setLine(line);
			tokens[tokens_idx++].setLinePosition(line_position);
		}

		void set_token(TokenType type, const char* value) {
			tokens[tokens_idx].setValue(value);
			tokens[tokens_idx].setType(type);
			tokens[tokens_idx].setLine(line);
			tokens[tokens_idx++].setLinePosition(line_position);
		}

		inline bool match(const char val) {
			return peek() == val;
		}

		inline bool peek_match(const char val) {
			return peek(src_idx + 1) == val;
		}

		inline char peek() {
			return src_idx <= src_len ? src[src_idx] : 0;
		}

		inline char peek(size_t pos) {
			return pos <= src_len ? src[pos] : 0;
		}

		inline bool advance() {
			if (match('\n')) {
				line++;
				line_position = 0;
			} else {
				line_position++;
			}
			src_idx++;
			return src_idx <= src_len;
		}

		inline bool advance(size_t by) {
			while (by > 0) {
				if (!advance()) break;
				by--;
			}
			return src_idx <= src_len;
		}

	};

	static void set_char_literal(TokenInfo& info) {
		info.advance();		//skip the '

		char tmp[3];
		size_t tmp_idx = 0;
		if (getChar(info.peek())) {	//if it starts with "\" it is escaped
			tmp[tmp_idx++] = info.peek();
			info.advance();
		}

		tmp[tmp_idx++] = info.peek();

		//check if char literal is ended with a second '
		if (info.peek_match(INFINITY_SINGLE_QUOTATIONMARK)) {
			info.advance();		//skip the second '
		} else {
			//TODO: ERROR
		}

		tmp[tmp_idx] = 0; //string end

		info.set_token(CharacterLiteral, tmp);
	}


	static void set_single_character(TokenType type, TokenInfo& info) {
		char tmp[2];
		tmp[0] = info.peek();
		tmp[1] = 0;
		info.set_token(type, tmp);
	}

	static void set_double_character(TokenType type, TokenInfo& info) {
		char tmp[3];
		tmp[0] = info.peek();
		info.advance();
		tmp[1] = info.peek();
		tmp[2] = 0;
		info.set_token(type, tmp);
	}

	static void set_string_literal(TokenInfo& info) {
		info.advance();

		size_t begin_idx = info.src_idx;

		while (!info.match(0) && !info.match('\n') && !info.match(INFINITY_QUOTATIONMARK)) {
			info.advance();
		}

		size_t string_len = (info.src_idx - begin_idx);
		char* string = new char[string_len + 1];

		while (info.src_idx > begin_idx) {
			size_t string_idx = string_len - (info.src_idx - begin_idx);
			string[string_idx] = info.peek(begin_idx++);
		}
		string[string_len] = 0;

		info.set_token(StringLiteral, string);
		delete[] string;
	}

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
		if (val == nullptr) m_val = nullptr;
		else {
			m_val = new char[strlen(val) + 1];
			strcpy(m_val, val);
		}
	}
	void Token::setType(const TokenType& type) {
		m_type = type;
	}

	void Token::setLine(const size_t& line) { m_line = line; }

	void Token::setLinePosition(const size_t& line) { m_line = line; }

	std::pair<LexerError*, Token*> Token::tokenize(const char* src, bool skipNewLine) {
		auto begin = std::chrono::high_resolution_clock::now();

		TokenInfo info{};
		info.src = src;
		info.src_len = strlen(src);
		info.src_idx = 0;
		info.tokens = new Token[info.src_len];
		info.errors = new LexerError[MAX_LEXER_ERROR_BEFORE_BREAK + 1];
		info.tokens_idx = 0;
		info.errors_idx = 0;

		info.line = 1;
		info.line_position = 1;

		while (info.is_at_end()) {
			if (info.match(INFINITY_OPEN_PAREN)) {
				info.set_token(OpenParen);
			} else if (info.match(INFINITY_CLOSE_PAREN)) {
				info.set_token(CloseParen);
			} else if (info.match(INFINITY_OPEN_CURLY)) {
				info.set_token(OpenCurly);
			} else if (info.match(INFINITY_CLOSE_CURLY)) {
				info.set_token(CloseCurly);
			} else if (info.match(INFINITY_OPEN_BRACKET)) {
				info.set_token(OpenBracket);
			} else if (info.match(INFINITY_CLOSE_BRACKET)) {
				info.set_token(CloseBracket);
			} else if (info.match(INFINITY_PLUS) && !info.peek_match(INFINITY_PLUS) || info.match(INFINITY_MINUS) && !info.peek_match(INFINITY_MINUS) || info.match(INFINITY_MULTIPLY) || (info.match(INFINITY_DIVIDE) && !info.peek_match(INFINITY_DIVIDE))) {
				set_single_character(BinaryOp, info);
			} else if (info.match(INFINITY_EQUALS) && !info.peek_match(INFINITY_EQUALS)) {
				info.set_token(Equals);
			} else if (info.match(INFINITY_COLON)) {
				info.set_token(Colon);
			} else if (info.match(INFINITY_SEMICOLON)) {
				info.set_token(Semicolon);
			} else if (info.match(0)) {
				info.set_token(End);
			} else if (info.match(INFINITY_NEWLINE)) {
				if(!skipNewLine)
					info.set_token(NewLine);
			} else if (info.match(INFINITY_AMPERSAND)) {
				info.set_token(Ampersand);
			} else if (info.match(INFINITY_QUESTIONMARK)) {
				info.set_token(QuestionMark);
			} else if (info.match(INFINITY_CALLSIGN) && !info.peek_match(INFINITY_EQUALS)) {
				info.set_token(CallSign);
			} else if (info.match(INFINITY_PERCENTSIGN)) {
				info.set_token(PercentSign);
			} else if (info.match(INFINITY_COMMA)) {
				info.set_token(Comma);
			} else if (info.match(INFINITY_DOT)) {
				info.set_token(Dot);
			} else if (info.match(INFINITY_HASH)) {
				info.set_token(Hash);
			} else if (info.match(INFINITY_TILDE)) {
				info.set_token(Tilde);
			} else if (info.match(INFINITY_SINGLE_QUOTATIONMARK)) {
				set_char_literal(info);
			} else if ((info.match(INFINITY_LESSTHAN) && !info.peek_match(INFINITY_EQUALS)) || (info.match(INFINITY_GREATERTHAN) && !info.peek_match(INFINITY_EQUALS))) {
				set_single_character(CompareOp, info);
			} else {

				//MULTI CHARACTER 
				if ((info.match(INFINITY_EQUALS) && info.peek_match(INFINITY_EQUALS)) || 
					(info.match(INFINITY_LESSTHAN) && info.peek_match(INFINITY_EQUALS)) || 
					(info.match(INFINITY_GREATERTHAN) && info.peek_match(INFINITY_EQUALS))) {
					set_double_character(CompareOp, info);
				} else if (info.match(INFINITY_QUOTATIONMARK)) {
					set_string_literal(info);
				} else if (info.match(INFINITY_DIVIDE) && info.peek_match(INFINITY_DIVIDE)) {
					size_t begin_idx = info.src_idx;
					while (!info.peek_match(0) && !info.peek_match('\n')) info.advance();
				} else if (isAlphabetical(info.peek()) || info.match('_')) {
					size_t begin_idx = info.src_idx;
					while (isAlphabetical(info.peek(info.src_idx + 1)) || isNumerical(info.peek(info.src_idx + 1)) || info.peek_match('_')) {
						info.advance();
					}

					size_t wordlen = ((info.src_idx+1) - begin_idx);
					char* tmp = new char[wordlen + 1];
					while (info.src_idx > begin_idx) {
						size_t tmp_idx = wordlen - (info.src_idx + 1 - begin_idx);
						tmp[tmp_idx] = src[begin_idx++];
					}
					tmp[wordlen] = 0;

					//KEYWORDS
					if (!strcmp(tmp, INFINITY_LET)) {
						info.set_token(Let);
					} else if (!strcmp(tmp, INFINITY_FOR)) {
						info.set_token(For);
					} else if (!strcmp(tmp, INFINITY_WHILE)) {
						info.set_token(While);
					} else if (!strcmp(tmp, INFINITY_DO)) {
						info.set_token(Do);
					} else if (!strcmp(tmp, INFINITY_BREAK)) {
						info.set_token(Break);
					} else if (!strcmp(tmp, INFINITY_CONTINUE)) {
						info.set_token(Continue);
					} else if (!strcmp(tmp, INFINITY_IF)) {
						info.set_token(If);
					} else if (!strcmp(tmp, INFINITY_VOID)) {
						info.set_token(Void);
					} else if (!strcmp(tmp, INFINITY_NULL)) {
						info.set_token(Null);
					} else if (!strcmp(tmp, INFINITY_CLASS)) {
						info.set_token(Class);
					} else if (!strcmp(tmp, INFINITY_STRUCT)) {
						info.set_token(Struct);
					} else if (!strcmp(tmp, INFINITY_PUBLIC)) {
						info.set_token(Public);
					} else if (!strcmp(tmp, INFINITY_PROTECTED)) {
						info.set_token(Protected);
					} else if (!strcmp(tmp, INFINITY_PRIVATE)) {
						info.set_token(Private);
					} else if (!strcmp(tmp, INFINITY_INT)) {
						info.set_token(Int);
					} else if (!strcmp(tmp, INFINITY_FLOAT)) {
						info.set_token(Float);
					} else if (!strcmp(tmp, INFINITY_STRING)) {
						info.set_token(String);
					} else if (!strcmp(tmp, INFINITY_CHAR)) {
						info.set_token(Char);
					} else if (!strcmp(tmp, INFINITY_BOOL)) {
						info.set_token(Bool);
					} else if (!strcmp(tmp, INFINITY_TABLE)) {
						info.set_token(Table);
					} else if (!strcmp(tmp, INFINITY_SWITCH)) {
						info.set_token(Switch);
					} else if (!strcmp(tmp, INFINITY_CASE)) {
						info.set_token(Case);
					} else if (!strcmp(tmp, INFINITY_SUPER)) {
						info.set_token(Super);
					} else if (!strcmp(tmp, INFINITY_THIS)) {
						info.set_token(This);
					} else {
						//IDENTIFIER
						info.set_token(Identifier, tmp);
					}

					delete[] tmp;
				} else if (isNumerical(info.peek())) {
					size_t begin_idx = info.src_idx;
					while (isNumerical(info.peek(info.src_idx + 1))) {
						info.advance();
					}

					if (info.peek_match('.')) {
						info.advance();
						while (isNumerical(info.peek(info.src_idx + 1))) 
							info.advance();
					}

					size_t wordlen = (info.src_idx - begin_idx);
					char* tmp = new char[wordlen + 1];
					while (info.src_idx > begin_idx) {
						size_t tmp_idx = wordlen - (info.src_idx - begin_idx);
						tmp[tmp_idx] = src[begin_idx++];
					}
					tmp[wordlen] = 0;

					//number
					info.set_token(NumberLiteral, tmp);

					delete[] tmp;
				} else if (isSkippable(info.peek())) {
					while (isSkippable(info.peek(info.src_idx + 1))) info.advance();
				} else {
					info.errors[info.errors_idx].m_critical = true;
					std::string tmp = std::format("Unknown Token {} at {}:{}", info.peek(), info.line, info.line_position - 1);
					info.errors[info.errors_idx++].m_msg = tmp;
				}
			}
			info.advance();
		}
		
		auto end = std::chrono::high_resolution_clock::now();

		if (info.errors_idx == 0) {
			INFINITY_CORE_INFO(POST_TOKENIZE_LOG_TEMPLATE_SUCCESS, info.src_len, info.tokens_idx, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), 0);
		} else {
			INFINITY_CORE_WARN(POST_TOKENIZE_LOG_TEMPLATE_ERROR, info.src_len, info.tokens_idx, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), info.errors_idx);
		}


		return std::pair<LexerError*, Token*>(info.errors, info.tokens);
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