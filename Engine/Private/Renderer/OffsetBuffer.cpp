#include "Renderer/OffsetBuffer.h"

#include <unordered_set>

void FOffsetBuffer::AddIndices(const std::vector<unsigned int>& Buffer)
{
	m_Buffer.reserve(Buffer.size() + m_Buffer.size());

	for (unsigned int Index : Buffer)
	{
		m_Buffer.push_back(Index + static_cast<unsigned int>(m_Offset));
	}

	const std::unordered_set<unsigned int> UniqueBuffer(Buffer.begin(), Buffer.end());
	m_Offset += UniqueBuffer.size();
}
