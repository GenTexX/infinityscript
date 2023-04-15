#pragma once

namespace infinity {

	enum TokenType {
		Unknown, 
		NumberLiteral, StringLiteral, CharacterLiteral,
		Identifier,
		Equals,
		OpenParen, CloseParen,
		OpenCurly, CloseCurly,
		OpenBracket, CloseBracket,
		QuestionMark, Ampersand,
		CallSign, PercentSign,
		BinaryOp, CompareOp,
		Colon, Semicolon, Dot, Comma, Hash, Tilde,
		

		//KEYWORDS
		Let, 
		For, While, Do, Break, Continue,
		If, 
		Void, Null,
		Class, Struct, Super, This, Public, Protected, Private,
		Int, Float, String, Char, Bool, Table,
		Switch, Case,
		True, False,

		NewLine, End
	};

}