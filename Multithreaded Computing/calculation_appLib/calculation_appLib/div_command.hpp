#pragma once
#include "command.hpp"

//TODO: dividing by zero handle.
class div_command final : public command
{
public:
	div_command() = delete;
	div_command(const double lhs, const double rhs) : lhs_{ lhs }, rhs_{ rhs } {}
	
	cmd_result execute() const override {
		return { "div", lhs_ / rhs_ };
	}

private:
	double lhs_;
	double rhs_;
};
