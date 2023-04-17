#pragma once

#include <lexer\token\token.h>

namespace infinity {

	enum ExpressionType {
		Binary, Unary, Grouping, Literal
	};

	class Expression {
	public:
		Expression() = default;
		virtual ~Expression() = default;

		virtual ExpressionType getExpressionType() const = 0;

	protected:

	private:

	};

	class BinaryExpression : public Expression {
	public:
		BinaryExpression(const Expression& left, const Token& op, const Expression& right) : m_left(&left), m_op(&op), m_right(&right) {}
		virtual ~BinaryExpression() = default;

		ExpressionType getExpressionType() const override { return infinity::ExpressionType::Binary; }

	private:
		const Expression const* m_left;
		const Token const* m_op;
		const Expression const* m_right;
	};

	class UnaryExpression : public Expression {
	public:
		UnaryExpression(const Token& op, const Expression& expr) : m_op(&op), m_expr(&expr) {}
		virtual ~UnaryExpression() = default;

		ExpressionType getExpressionType() const override { return infinity::ExpressionType::Unary; }

	private:
		const Token const* m_op;
		const Expression const* m_expr;
	};

	class GroupingExpression : public Expression {
	public:
		GroupingExpression(const Expression& expr) : m_expr(&expr) {}
		virtual ~GroupingExpression() = default;

		ExpressionType getExpressionType() const override { return infinity::ExpressionType::Grouping; }

	private:
		const Expression const* m_expr;
	};

	class LiteralExpression : public Expression {
	public:
		LiteralExpression(const Token& lit) : m_literal(&lit) {}
		virtual ~LiteralExpression() = default;

		ExpressionType getExpressionType() const override { return infinity::ExpressionType::Literal; }

	private:
		const Token const* m_literal;
	};

}