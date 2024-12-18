#pragma once

namespace CultyEngine
{
	class GameObjectHandle
	{
	public:
		GameObjectHandle() = default;

        const int GetIndex() const { return mIndex; }

	private:
		friend class GameWorld;

		int mIndex = -1;
		int mGeneration = -1;
	};
}