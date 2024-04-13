#pragma once
#include "Texture.h"

namespace CultyEngine::Graphics
{
	using TextureID = std::size_t;

	class TextureManager
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static TextureManager* Get();

		TextureManager() = default;
		~TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager(const TextureManager&&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&&) = delete;

		void SetRootDirector(const std::filesystem::path& root);

		TextureID LoadTexture(const std::filesystem::path& fileName, bool useRootDir = true);
		const Texture* GetTexture(TextureID id) const;

		void BindVS(TextureID id, uint32_t slot) const;
		void BindPS(TextureID id, uint32_t slot) const;

	private:
		using Inventory = std::unordered_map<TextureID, std::unique_ptr<Texture>>;
		Inventory mInventory;

		std::filesystem::path mRootDirectory;
	};
}