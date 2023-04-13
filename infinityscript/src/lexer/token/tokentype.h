#pragma once

namespace infinity {

	enum TokenType {
		Unknown,
		Number,
		Identifier,
		Equals,
		OpenParen, CloseParam,
		BinaryOp,
		Let
	};

}