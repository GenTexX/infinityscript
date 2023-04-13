#include <infscrpch.h>
#include "lexer.h"

namespace infinity {

	Lexer::Lexer(const char* filename) {
		FILE* f = fopen(filename, "r");

		char buff[255];
		while (true) {

			fgets(buff, 255, (FILE*)f);
			size_t length = strlen(buff);

			printf("%s", buff);

			if (buff[length - 1] != '\n') {
				printf("\n", buff);
				break;
			}
			

		}
		fclose(f);
	}

	Lexer::~Lexer() {
	}

}