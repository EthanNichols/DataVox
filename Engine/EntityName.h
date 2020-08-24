#pragma once

#include <string>

#include "cereal/archives/json.hpp"


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

	operator std::string* ()
	{
		return &m_name;
	}

	operator std::string& ()
	{
		return m_name;
	}

	template<class Archive>
	inline void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("Name", m_name)
		);
	}

public:

	std::string m_name;
};