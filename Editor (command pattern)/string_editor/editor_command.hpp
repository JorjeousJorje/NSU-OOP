#pragma once

class editor_command
{
public:
	virtual void execute() = 0;
	virtual void undo() = 0;
	
	virtual ~editor_command() = default;
};
