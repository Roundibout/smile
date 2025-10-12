/*
File:
    font_manager.hpp
Authors:
    Lucas
Purpose:
    Definition of the FontManager singleton, responsible for handling fonts
License:
    MIT (see LICENSE file)
*/

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <util/logger.hpp>

struct Glyph {
    std::vector<unsigned char> bitmap;
    int width, height;
    int bearingX, bearingY;
    int advance;
};

struct Font {
    FT_Face face;
    std::unordered_map<char, Glyph> glyphs;
    int size;
    ~Font();
    const Glyph* getGlyph(char c);
};

class FontManager {
private:
    FT_Library library;
    std::unordered_map<std::string, std::unique_ptr<Font>> fonts; // Key by path+size
    // Private constructor to prevent direct instantiation (there should be only one font manager)
    FontManager();
    ~FontManager();
    // Delete copy constructor and assignment operator to prevent copying (same reason)
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
public:
    static FontManager& get() {
        // Initialize font manager only once
        static FontManager instance; 
        return instance;
    }
    Font* getFont(const std::string& path, int size);
    float getTextWidth(const std::string& text, const std::string& path, int size);
};