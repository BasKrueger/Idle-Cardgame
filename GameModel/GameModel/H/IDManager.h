#pragma once
class IDManager
{
public:
	static int NewID();

private:
	static int counter;
};

