#pragma once
#include "command.hpp"

class sub_command final : public command
{
public:
	sub_command() = delete;
	sub_command(const double lhs, const double rhs) : lhs_{ lhs }, rhs_{ rhs } {}
	
	cmd_result execute() const override {
		return { "sub", lhs_ - rhs_ };
	}
	
private:
	double lhs_;
	double rhs_;
};
