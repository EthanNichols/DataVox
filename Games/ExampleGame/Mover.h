#pragma once

#include <cereal/archives/json.hpp>

#include <Imgui/imgui.h>
#include <Imgui/imgui_stdlib.h>

namespace Component
{
	struct Mover
	{
	public:
		bool m_moving;

	public:
		template<class Archive>
		inline void serialize(Archive& archive)
		{
			archive(
				cereal::make_nvp("move", m_moving)
			);
		}
	};
}
