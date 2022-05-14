#pragma once
#include "command.hpp"

class add_command final : public command
{
public:
	add_command() = delete;
	add_command(const double lhs, const double rhs) : lhs_ {lhs}, rhs_{ rhs } {}
	
	cmd_result execute() const override {
		return { "add", lhs_ + rhs_ };
	}

private:
	double lhs_;
	double rhs_;
};
