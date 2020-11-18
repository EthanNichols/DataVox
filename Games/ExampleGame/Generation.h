#pragma once

#include "cereal/archives/json.hpp"

struct Generation
{
public:

	int renderDistance;

	int maxHeight;
	int chunkSize = 16;

	uint32_t seed;
	bool randomizeSeed = false;
	float perlinStepSize;

public:

	template<class Archive>
	inline void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("renderDistance", renderDistance),
			cereal::make_nvp("maxHeight", maxHeight),
			cereal::make_nvp("chunkSize", chunkSize),
			cereal::make_nvp("seed", seed),
			cereal::make_nvp("randomizeSeed", randomizeSeed),
			cereal::make_nvp("perlinStepSize", perlinStepSize)
		);
	}
};