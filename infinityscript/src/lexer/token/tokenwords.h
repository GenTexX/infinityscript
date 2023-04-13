#pragma once

#define INFINITY_OPEN_PAREN				'('
#define INFINITY_CLOSE_PAREN				')'
#define INFINITY_EQUALS					'='
#define INFINITY_PLUS					'+'
#define INFINITY_MINUS					'-'
#define INFINITY_MULTIPLY				'*'
#define INFINITY_DIVIDE					'/'

#define INFINITY_LET					"let"

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

	bool isSkippable(const char val) {
		return (val == ' ' || val == '\n' || val == '\t');
	}

}
