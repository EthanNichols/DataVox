#pragma once

#include <string>


struct EntityName
{
public:

	EntityName()
	{
	}
	EntityName(std::string name) : m_name(name)
	{
	}
	~EntityName()
	{
	}

public:

	const char* c_str()
	{
		return m_name.c_str();
	}

public:

	operator std::string* ()
	{
		return &m_name;
	}

	operator std::string& ()
	{
		return m_name;
	}

private:
	std::string m_name;
};