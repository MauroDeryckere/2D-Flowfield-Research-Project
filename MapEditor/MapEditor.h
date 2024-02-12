#ifndef MAPEDITOR 
#define MAPEDITOR

#include "../Engine/GameEngine.h";

#include <iostream>
#include <fstream>
#include <filesystem>
#include <optional>

class MapEditor final
{
public:
	MapEditor();
	~MapEditor() = default;

	MapEditor(const MapEditor& other) = delete;
	MapEditor(MapEditor&& other) noexcept = delete;
	MapEditor& operator=(const MapEditor& other) = delete;
	MapEditor& operator=(MapEditor&& other) noexcept = delete;

	void Draw() const;

	static bool LoadMap(const std::filesystem::path& filePath);

private:

};

#endif
