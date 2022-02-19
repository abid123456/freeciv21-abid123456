/***********************************************************************
Copyright (c) 1996-2020 Freeciv21 and Freeciv contributors. This file is
 /\/\             part of Freeciv21. Freeciv21 is free software: you can
   \_\  _..._    redistribute it and/or modify it under the terms of the
   (" )(_..._)      GNU General Public License  as published by the Free
    ^^  // \\      Software Foundation, either version 3 of the License,
                  or (at your option) any later version. You should have
received a copy of the GNU General Public License along with Freeciv21.
                              If not, see https://www.gnu.org/licenses/.
***********************************************************************/
#pragma once

#include "support.h" // bool type

#include "gui_proto_constructor.h"

class QColor;
class QFont;
class QPixmap; // opaque type, real type is gui-dep
class QString;

// Drawing functions
GUI_FUNC_PROTO(void, canvas_put_sprite_fogged, QPixmap *pcanvas,
               int canvas_x, int canvas_y, const QPixmap *psprite, bool fog,
               int fog_x, int fog_y)
GUI_FUNC_PROTO(void, canvas_put_sprite_citymode, QPixmap *pcanvas,
               int canvas_x, int canvas_y, const QPixmap *psprite, bool fog,
               int fog_x, int fog_y)
void canvas_put_unit_fogged(QPixmap *pcanvas, int canvas_x, int canvas_y,
                            const QPixmap *psprite, bool fog, int fog_x,
                            int fog_y);
// Text drawing functions
enum client_font {
  FONT_CITY_NAME,
  FONT_CITY_PROD,
  FONT_REQTREE_TEXT,
  FONT_COUNT
};

QFont get_font(enum client_font font);
