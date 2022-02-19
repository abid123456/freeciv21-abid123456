/**  / \                                                        ***********
    / _ \
   | / \ |                  Copyright (c) 1996-2020 Freeciv21 and
   ||   || _______    Freeciv contributors. This file is part of Freeciv21.
   ||   || |\     \    Freeciv21 is free software: you can redistribute it
   ||   || ||\     \               and/or modify it under the terms of the
   ||   || || \    |                       GNU  General Public License  as
   ||   || ||  \__/              published by the Free Software Foundation,
   ||   || ||   ||                         either version 3 of the License,
    \\_/ \_/ \_//                    or (at your option) any later version.
   /   _     _   \                  You should have received a copy of the
  /               \        GNU General Public License along with Freeciv21.
  |    O     O    |                               If not,
  |   \  ___  /   |             see https://www.gnu.org/licenses/.
 /     \ \_/ /     \
/  -----  |  --\    \
|     \__/|\__/ \   |                 FOLLOW THE WHITE RABBIT !
\       |_|_|       /
 \_____       _____/
       \     /                                                      *****/
#include "canvas.h"
#include <cmath>
// Qt
#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>

#include "client_main.h"
#include "mapview_common.h"
#include "tilespec.h"
// qt-client
#include "colors.h"
#include "colors_common.h"
#include "fc_client.h"
#include "fonts.h"
#include "qtg_cxxside.h"
#include "sprite.h"

/**
   Copies an area from the source pixmap to the destination pixmap.
 */
void pixmap_copy(QPixmap *dest, const QPixmap *src, int src_x, int src_y,
                 int dest_x, int dest_y, int width, int height)
{
  QRectF source_rect(src_x, src_y, width, height);
  QRectF dest_rect(dest_x, dest_y, width, height);
  QPainter p;

  if (!width || !height) {
    return;
  }

  p.begin(dest);
  p.drawPixmap(dest_rect, *src, source_rect);
  p.end();
}

/**
   Copies an area from the source image to the destination image.
 */
void image_copy(QImage *dest, const QImage *src, int src_x, int src_y,
                int dest_x, int dest_y, int width, int height)
{
  QRectF source_rect(src_x, src_y, width, height);
  QRectF dest_rect(dest_x, dest_y, width, height);
  QPainter p;

  if (!width || !height) {
    return;
  }

  p.begin(dest);
  p.drawImage(dest_rect, *src, source_rect);
  p.end();
}

/**
   Draw a full sprite onto the canvas.  If "fog" is specified draw it with
   fog.
 */
void qtg_canvas_put_sprite_fogged(QPixmap *pcanvas, int canvas_x,
                                  int canvas_y, const QPixmap *psprite,
                                  bool fog, int fog_x, int fog_y)
{
  Q_UNUSED(fog_x)
  Q_UNUSED(fog_y)

  QPixmap temp(psprite->size());
  temp.fill(Qt::transparent);
  QPainter p(&temp);
  p.setCompositionMode(QPainter::CompositionMode_Source);
  p.drawPixmap(0, 0, *psprite);
  p.setCompositionMode(QPainter::CompositionMode_SourceAtop);
  p.fillRect(temp.rect(), QColor(0, 0, 0, 110));
  p.end();

  p.begin(pcanvas);
  p.drawPixmap(canvas_x, canvas_y, temp);
  p.end();
}

/*****************************************************************************
   Draw fog outside city map when city is opened
 */
void qtg_canvas_put_sprite_citymode(QPixmap *pcanvas, int canvas_x,
                                    int canvas_y, const QPixmap *psprite,
                                    bool fog, int fog_x, int fog_y)
{
  Q_UNUSED(fog_x)
  Q_UNUSED(fog_y)
  QPainter p;

  p.begin(pcanvas);
  p.setCompositionMode(QPainter::CompositionMode_Difference);
  p.setOpacity(0.5);
  p.drawPixmap(canvas_x, canvas_y, *psprite);
  p.end();
}

/*****************************************************************************
   Put unit in city area when city dialog is open
 */
void canvas_put_unit_fogged(QPixmap *pcanvas, int canvas_x, int canvas_y,
                            const QPixmap *psprite, bool fog, int fog_x,
                            int fog_y)
{
  Q_UNUSED(fog_y)
  Q_UNUSED(fog_x)
  QPainter p;

  p.begin(pcanvas);
  p.setOpacity(0.7);
  p.drawPixmap(canvas_x, canvas_y, *psprite);
  p.end();
}

/**
   Returns given font
 */
QFont get_font(client_font font)
{
  QFont qf;

  switch (font) {
  case FONT_CITY_NAME:
    qf = fcFont::instance()->getFont(fonts::city_names, 1);
    break;
  case FONT_CITY_PROD:
    qf = fcFont::instance()->getFont(fonts::city_productions, 1);
    break;
  case FONT_REQTREE_TEXT:
    qf = fcFont::instance()->getFont(fonts::reqtree_text);
    break;
  case FONT_COUNT:
    break;
  }

  return qf;
}

/**
   Return rectangle containing pure image (crops transparency)
 */
QRect zealous_crop_rect(QImage &p)
{
  int r, t, b, l;

  l = p.width();
  r = 0;
  t = p.height();
  b = 0;
  for (int y = 0; y < p.height(); ++y) {
    QRgb *row = reinterpret_cast<QRgb *>(p.scanLine(y));
    bool row_filled = false;
    int x;

    for (x = 0; x < p.width(); ++x) {
      if (qAlpha(row[x])) {
        row_filled = true;
        r = qMax(r, x);
        if (l > x) {
          l = x;
          x = r;
        }
      }
    }
    if (row_filled) {
      t = qMin(t, y);
      b = y;
    }
  }
  return QRect(l, t, qMax(0, r - l + 1), qMax(0, b - t + 1));
}
