#pragma once

#define INFINITY_OPEN_PAREN				'('
#define INFINITY_CLOSE_PAREN			')'
#define INFINITY_OPEN_CURLY				'{'
#define INFINITY_CLOSE_CURLY			'}'
#define INFINITY_OPEN_BRACKET			'['
#define INFINITY_CLOSE_BRACKET			']'
#define INFINITY_EQUALS					'='
#define INFINITY_PLUS					'+'
#define INFINITY_MINUS					'-'
#define INFINITY_MULTIPLY				'*'
#define INFINITY_DIVIDE					'/'
#define INFINITY_COLON					':'
#define INFINITY_SEMICOLON				';'
#define INFINITY_LESSTHAN				'<'
#define INFINITY_GREATERTHAN			'>'
#define INFINITY_QUESTIONMARK			'?'
#define INFINITY_AMPERSAND				'&'
#define INFINITY_CALLSIGN				'!'
#define INFINITY_PERCENTSIGN			'%'
#define INFINITY_COMMA					','
#define INFINITY_UNDERSCORE				'_'
#define INFINITY_QUOTATIONMARK			'\"'
#define INFINITY_SINGLE_QUOTATIONMARK	'\''
#define	INFINITY_DOT					'.'
#define	INFINITY_HASH					'#'
#define	INFINITY_TILDE					'~'
#define	INFINITY_NEWLINE				'\n'

#define INFINITY_LET					"let"
#define INFINITY_FOR					"for"
#define INFINITY_WHILE					"while"
#define INFINITY_DO						"do"
#define INFINITY_BREAK					"break"
#define INFINITY_CONTINUE				"continue"
#define INFINITY_IF						"if"
#define INFINITY_VOID					"void"
#define INFINITY_NULL					"null"
#define INFINITY_CLASS					"class"
#define INFINITY_STRUCT					"struct"
#define INFINITY_SUPER					"super"
#define INFINITY_THIS					"this"
#define INFINITY_PUBLIC					"public"
#define INFINITY_PROTECTED				"protected"
#define INFINITY_PRIVATE				"private"
#define INFINITY_INT					"int"
#define INFINITY_FLOAT					"float"
#define INFINITY_STRING					"string"
#define INFINITY_CHAR					"char"
#define INFINITY_BOOL					"bool"
#define INFINITY_TABLE					"table"
#define INFINITY_SWITCH					"switch"
#define INFINITY_CASE					"case"
#define INFINITY_TRUE					"true"
#define INFINITY_FALSE					"false"


namespace infinity {
	bool isAlphabetical(const char val) {
		return ((val >= 'a' && val <= 'z') || (val >= 'A' && val <= 'Z'));
	}

	bool isAlphabetical(const char* val) {
		bool result = true;
		int i = 0;
		while (val[i] != 0) {
			if (isAlphabetical(val[i])) continue;
			else {
				result = false;
				break;
			}
		}
		return result;
	}

	bool isNumerical(const char val) {
		return (val >= '0' && val <= '9');
	}

	bool isNumerical(const char* val) {
		bool result = true;
		int i = 0;
		while (val[i] != 0) {
			if (isNumerical(val[i])) continue;
			else {
				result = false;
				break;
			}
		}
		return result;
	}

	bool getChar(const char val) {
		if (val == '\\') {
			return true;
		} else {
			return false;
		}
	}

	bool isSkippable(const char val) {
		return (val == ' ' || val == '\n' || val == '\t');
	}

}
