#pragma once

#include <string>

#include "cereal/archives/json.hpp"


namespace Component
{
	struct EntityName
	{
	public:

		EntityName()
		{
		}
		EntityName(std::string name) :
			Name(name)
		{
		}
		~EntityName()
		{
		}

	public:

		static void ConstructWidget(Registry& registry, Entity entity)
		{

		}

	public:

		const char* c_str()
		{
			return Name.c_str();
		}

		operator std::string* ()
		{
			return &Name;
		}

		operator std::string& ()
		{
			return Name;
		}

		template<class Archive>
		inline void serialize(Archive& archive)
		{
			archive(
				cereal::make_nvp("Name", Name)
			);
		}

	public:

		std::string Name;
	};
}