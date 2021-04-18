#pragma once

class FWindow;

class UserInterface
{
public:
	UserInterface(const FWindow& Window);
	~UserInterface();

public:
	void PreTick();
	void PostTick();
};