#include "MapEditor.h"

MapEditor::MapEditor()
{
}

void MapEditor::Draw() const
{
}

bool MapEditor::LoadMap(const std::filesystem::path& filePath) 
{
    //Float3f toRead{}; // | std::ios::binary

    int dataToRead{};

    std::vector<int> data;
    data.reserve(100);

    if (std::ifstream input{ filePath, std::ios::in})
    {
        int dataToRead;
        while (input >> dataToRead) 
        {
            data.push_back(dataToRead);
        }
    }
    else
    {
        OutputDebugString(filePath);
        OutputDebugString(_T("    FALSE  -------------------------\n"));

        return false;
    }

    OutputDebugString(_T("\n\n"));
    for (int i = 0; i < 100; ++i)
    {
        OutputDebugStringW(std::to_wstring(data[i]));
        if ((i+1)%10==0)
        {
            OutputDebugString(_T("\n"));
        }
    }

    OutputDebugString(_T("\n\n"));

	return true;
}

