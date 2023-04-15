#include <infscrpch.h>
#include "lexer.h"
#include <iostream>
#include <fstream>
#include <limits>

#include <logger\logger.h>

#include "token\token.h"

namespace infinity {

	Lexer::Lexer(const char* filename) {
		m_src = nullptr;
		m_filename = strdup(filename);

		std::ifstream filestream;
		filestream.open(filename);

		if (filestream.is_open()) {
			//get filesize
			filestream.ignore(std::numeric_limits<std::streamsize>::max());
			std::streamsize length = filestream.gcount();
			filestream.clear();   //  Since ignore will have set eof.
			filestream.seekg(0, std::ios_base::beg);

			m_src = new char[length + 1];
			m_src[length] = 0;
			filestream.seekg(0, std::ios::beg);
			filestream.read(m_src, length);

			INFINITY_CORE_INFO("successfully read {}", filename);
		} else {
			m_error_queue.push({"could not open file", filename, true});
		}
	}

	Lexer::~Lexer() {
		if (m_src != nullptr) delete[] m_src;
		if (m_filename != nullptr) delete m_filename;
	}

	Token* Lexer::analyse() {
		
		Token* result = nullptr;

		auto errtkn_pair = Token::tokenize(m_src);
		
		size_t idx = 0;
		while (!errtkn_pair.first[idx].m_msg.empty()) {
			errtkn_pair.first[idx].m_filename = m_filename;
			m_error_queue.push(errtkn_pair.first[idx++]);
		}

		if (!checkErrors()) result = errtkn_pair.second;

		return result;
	}

	bool Lexer::checkErrors() {
		bool crit = false;
		while (!m_error_queue.empty()) {
			LexerError err = m_error_queue.front();
			m_error_queue.pop();

			crit |= err.m_critical;
			err.m_critical ? INFINITY_CORE_CRITICAL(err.toString()) : INFINITY_CORE_ERROR(err.toString());
		}

		return crit;
	}

}