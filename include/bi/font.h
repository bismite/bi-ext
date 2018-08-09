#ifndef __BI_EXT_FONT_H__
#define __BI_EXT_FONT_H__

#include <stdint.h>

struct _BiNode;
typedef struct _BiNode BiNode;

struct _BiTextureImage;
typedef struct _BiTextureImage BiTextureImage;

struct _BiGlyphLayout;
typedef struct _BiGlyphLayout BiGlyphLayout;

struct _BiFontAtlas;
typedef struct _BiFontAtlas BiFontAtlas;

typedef struct {
  uint32_t version;
  uint16_t font_size;
  int16_t height;
  int16_t ascent;
  int16_t descent;
  uint32_t glyph_count;
} BiFontHeader;

struct _BiGlyphLayout {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    int16_t base_x;
    int16_t base_y;
    int16_t advance_x;
    int16_t advance_y;
    uint32_t utf8;
    uint32_t reserved;
};

struct _BiFontAtlas {
  BiGlyphLayout table[0xFFFF];
  BiTextureImage* texture_image;
  int font_size;
  int base_line;
  uint8_t color[4];
};

extern void bi_load_font_layout(const char *filename, BiFontAtlas* font);

extern void bi_update_label(BiNode* node, const char* text, const BiFontAtlas* font );

#endif
