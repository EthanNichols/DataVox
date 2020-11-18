#pragma once

#include "Mover.h"
#include "Generation.h"

namespace Widgets
{
	void MoverWidget(Component::Mover& mover)
	{

	}

	void GenerationWidget(Generation& generationData)
	{
		ImGui::InputInt("Render Distance", &generationData.renderDistance);
		ImGui::InputInt("Chunk Size", &generationData.chunkSize);
		ImGui::InputInt("Max Height", &generationData.maxHeight);

		int unsignedValue = generationData.seed;

		ImGui::InputInt("Seed", &unsignedValue);
		ImGui::Checkbox("Randomize Seed", &generationData.randomizeSeed);
		ImGui::InputFloat("Perling Step Size", &generationData.perlinStepSize);

		generationData.seed = glm::abs(unsignedValue);
	}
}