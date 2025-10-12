/*
File:
    font_manager.cpp
Authors:
    Lucas
Purpose:
    Implementation of the FontManager singleton, responsible for handling fonts
License:
    MIT (see LICENSE file)
*/

#include "font_manager.hpp"

Font::~Font() {
    if (face) FT_Done_Face(face);
}

const Glyph* Font::getGlyph(char c) {
    auto it = glyphs.find(c);
    if (it != glyphs.end()) {
        return &it->second;
    }

    // Load glyph from FreeType
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        Logger::error("Failed to load glyph: " + c);
        return nullptr;
    }

    Glyph glyph;
    FT_GlyphSlot slot = face->glyph;

    glyph.width = slot->bitmap.width;
    glyph.height = slot->bitmap.rows;
    glyph.bearingX = slot->bitmap_left;
    glyph.bearingY = slot->bitmap_top;
    glyph.advance = slot->advance.x >> 6; // 1/64th pixels -> pixels

    // Copy bitmap data
    glyph.bitmap.assign(
        slot->bitmap.buffer,
        slot->bitmap.buffer + (slot->bitmap.width * slot->bitmap.rows)
    );

    // Insert into cache
    auto [inserted, _] = glyphs.emplace(c, std::move(glyph));
    return &inserted->second;
}

FontManager::FontManager() {
    if (FT_Init_FreeType(&library)) {
        Logger::error("Failed to initialize FreeType");
    }
}

FontManager::~FontManager() {
    FT_Done_FreeType(library);
}

Font* FontManager::getFont(const std::string& path, int size) {
    std::string key = path + "#" + std::to_string(size);

    auto it = fonts.find(key);
    if (it != fonts.end()) {
        return it->second.get();
    }

    // Not loaded yet → load
    auto font = std::make_unique<Font>();
    if (FT_New_Face(library, path.c_str(), 0, &font->face)) {
        Logger::error("Failed to load font: " + path);
        return nullptr;
    }

    FT_Set_Pixel_Sizes(font->face, 0, size);
    font->size = size;

    Font* ptr = font.get();
    fonts.emplace(key, std::move(font));
    return ptr;
}

float FontManager::getTextWidth(const std::string& text, const std::string& path, int size) {
    Font* font = getFont(path, size);
    if (!font || !font->face) {
        return 0.0f;
    }

    float totalWidth = 0.0f;

    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char c = text[i];

        // Get glyph so we can add it's width
        const Glyph* glyph = font->getGlyph(c);
        if (!glyph) continue;

        totalWidth += glyph->advance;

        // Kerning
        if (i + 1 < text.size()) {
            FT_Vector kern;
            FT_Get_Kerning(font->face, FT_Get_Char_Index(font->face, c), FT_Get_Char_Index(font->face, text[i + 1]), FT_KERNING_DEFAULT, &kern);
            totalWidth += kern.x >> 6; // convert from 1/64 pixels
        }
    }

    return totalWidth;
}