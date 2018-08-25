#include <bi/ext/font.h>
#include <bi/ext/simple_unicode.h>
#include <bi/node.h>
#include <bi/util.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void bi_load_font_layout(const char *filename, BiFontAtlas* font)
{
  FILE *layout_file = fopen( filename, "rb");
  if(layout_file == NULL) {
    fprintf(stderr, "Layout: %s Load Failed.\n", filename );
  }

  BiFontHeader header;
  fread(&header,sizeof(BiFontHeader),1,layout_file);

  font->font_size = header.font_size;
  font->base_line = header.descent-1;

  for(int i=0;i<0xFFFF;i++) {
    font->table[i].utf8 = 0; // XXX: no-font
  }

  for(int i=0; i<header.glyph_count; i++){
    BiGlyphLayout l;
    size_t count = fread(&l,sizeof(BiGlyphLayout),1,layout_file);
    if(count < 1) {
      printf("%d table created.\n",i);
      break;
    }
    uint16_t ucs2 = utf8_to_ucs2( l.utf8 );
    font->table[ucs2] = l;
  }

  fclose(layout_file);
}

void bi_update_label(BiNode* node, const char* text, const BiFontAtlas* font )
{
  // text
  size_t textlen = strlen(text);
  int x = 0;
  int y = 0;
  int i = 0;
  for(int i=0;i<node->children_size;i++) {
    node->children[i]->visible = false;
  }
  while (textlen > 0) {
      uint32_t ucs2 = utf8_getch_as_ucs2(&text, &textlen);

      const BiGlyphLayout *g = &(font->table[ucs2]);

      if(g->utf8==0 || g->w==0 || g->h==0) {
        continue;
      }

      BiNode* n = NULL;
      if( i < node->children_size ){
        n = node->children[i];
      }else{
        n = malloc(sizeof(BiNode));
        bi_node_init(n);
        bi_add_node(node,n);
        n->texture = malloc(sizeof(BiTexture));
      }
      // node
      bi_node_set_position(n, x - g->base_x, y + font->base_line + (g->base_y - g->h) );
      bi_node_set_size(n,g->w,g->h);
      n->visible = true;
      //texture
      n->texture->texture_image = font->texture_image;
      bi_set_texture_boundary(n->texture, g->x,g->y,g->w,g->h);
      // color
      bi_set_color( n->color, font->color[0], font->color[1], font->color[2], font->color[3] );

      x += g->advance_x;
      i++;
  }
  bi_node_set_size(node,x,font->font_size);
  bi_node_set_matrix_include_anchor_translate(node,true);
}
