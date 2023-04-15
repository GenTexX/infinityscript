#pragma once
#include <string>
#include <format>

#define CRITICAL_INFINITY_LEXER_ERROR_FORMAT "critical lexer-error in {}: {}"
#define INFINITY_LEXER_ERROR_FORMAT "lexer-error in {}: {}"

namespace infinity {

	struct LexerError {

		LexerError() :m_msg("") {}
		LexerError(const char* msg, const char* filename, bool crit) :m_msg(nullptr), m_filename(filename), m_critical(crit) {}

		std::string m_msg;
		const char* m_filename;
		bool m_critical;

		const std::string toString() {
			return m_critical
				?
				std::format(CRITICAL_INFINITY_LEXER_ERROR_FORMAT, m_filename, m_msg)
				:
				std::format(INFINITY_LEXER_ERROR_FORMAT, m_filename, m_msg);
		}

	};

}