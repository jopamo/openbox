// gradient.c for the Openbox window manager

#include <glib.h>
#include <string.h>

#include "color.h"
#include "gradient.h"
#include "render.h"

static void highlight(RrSurface *s, RrPixel32 *x, RrPixel32 *y, gboolean raised);
static void gradient_parentrelative(RrAppearance *a, gint w, gint h);
static void gradient_solid(RrAppearance *l, gint w, gint h);
static void gradient_splitvertical(RrAppearance *a, gint w, gint h);
static void gradient_vertical(RrSurface *sf, gint w, gint h);
static void gradient_horizontal(RrSurface *sf, gint w, gint h);
static void gradient_mirrorhorizontal(RrSurface *sf, gint w, gint h);
static void gradient_diagonal(RrSurface *sf, gint w, gint h);
static void gradient_crossdiagonal(RrSurface *sf, gint w, gint h);
static void gradient_pyramid(RrSurface *sf, gint inw, gint inh);

void RrRender(RrAppearance *a, gint w, gint h) {
  RrPixel32 *data = a->surface.pixel_data;
  RrPixel32 current;
  guint r, g, b;
  register gint off, x;

  switch (a->surface.grad) {
    case RR_SURFACE_PARENTREL:
      gradient_parentrelative(a, w, h);
      break;
    case RR_SURFACE_SOLID:
      gradient_solid(a, w, h);
      break;
    case RR_SURFACE_SPLIT_VERTICAL:
      gradient_splitvertical(a, w, h);
      break;
    case RR_SURFACE_VERTICAL:
      gradient_vertical(&a->surface, w, h);
      break;
    case RR_SURFACE_HORIZONTAL:
      gradient_horizontal(&a->surface, w, h);
      break;
    case RR_SURFACE_MIRROR_HORIZONTAL:
      gradient_mirrorhorizontal(&a->surface, w, h);
      break;
    case RR_SURFACE_DIAGONAL:
      gradient_diagonal(&a->surface, w, h);
      break;
    case RR_SURFACE_CROSS_DIAGONAL:
      gradient_crossdiagonal(&a->surface, w, h);
      break;
    case RR_SURFACE_PYRAMID:
      gradient_pyramid(&a->surface, w, h);
      break;
    default:
      g_assert_not_reached();
      return;
  }

  if (a->surface.interlaced) {
    gint i;
    RrPixel32 *p;

    r = a->surface.interlace_color->r;
    g = a->surface.interlace_color->g;
    b = a->surface.interlace_color->b;
    current = (r << RrDefaultRedOffset) + (g << RrDefaultGreenOffset) + (b << RrDefaultBlueOffset);
    p = data;
    for (i = 0; i < h; i += 2, p += w)
      for (x = 0; x < w; ++x, ++p) *p = current;
  }

  if (a->surface.relief == RR_RELIEF_FLAT && a->surface.border) {
    r = a->surface.border_color->r;
    g = a->surface.border_color->g;
    b = a->surface.border_color->b;
    current = (r << RrDefaultRedOffset) + (g << RrDefaultGreenOffset) + (b << RrDefaultBlueOffset);
    for (off = 0, x = 0; x < w; ++x, off++) {
      *(data + off) = current;
      *(data + off + ((h - 1) * w)) = current;
    }
    for (off = 0, x = 0; x < h; ++x, off++) {
      *(data + (off * w)) = current;
      *(data + (off * w) + w - 1) = current;
    }
  }

  if (a->surface.relief != RR_RELIEF_FLAT) {
    if (a->surface.bevel == RR_BEVEL_1) {
      for (off = 1, x = 1; x < w - 1; ++x, off++)
        highlight(&a->surface, data + off, data + off + (h - 1) * w, a->surface.relief == RR_RELIEF_RAISED);
      for (off = 0, x = 0; x < h; ++x, off++)
        highlight(&a->surface, data + off * w, data + off * w + w - 1, a->surface.relief == RR_RELIEF_RAISED);
    }

    if (a->surface.bevel == RR_BEVEL_2) {
      for (off = 2, x = 2; x < w - 2; ++x, off++)
        highlight(&a->surface, data + off + w, data + off + (h - 2) * w, a->surface.relief == RR_RELIEF_RAISED);
      for (off = 1, x = 1; x < h - 1; ++x, off++)
        highlight(&a->surface, data + off * w + 1, data + off * w + w - 2, a->surface.relief == RR_RELIEF_RAISED);
    }
  }
}

static void highlight(RrSurface *s, RrPixel32 *x, RrPixel32 *y, gboolean raised) {
  register gint r, g, b;
  RrPixel32 *up, *down;

  up = raised ? x : y;
  down = raised ? y : x;

  r = (*up >> RrDefaultRedOffset) & 0xFF;
  r += (r * s->bevel_light_adjust) >> 8;
  g = (*up >> RrDefaultGreenOffset) & 0xFF;
  g += (g * s->bevel_light_adjust) >> 8;
  b = (*up >> RrDefaultBlueOffset) & 0xFF;
  b += (b * s->bevel_light_adjust) >> 8;
  r = MIN(r, 0xFF);
  g = MIN(g, 0xFF);
  b = MIN(b, 0xFF);
  *up = (r << RrDefaultRedOffset) + (g << RrDefaultGreenOffset) + (b << RrDefaultBlueOffset);

  r = (*down >> RrDefaultRedOffset) & 0xFF;
  r -= (r * s->bevel_dark_adjust) >> 8;
  g = (*down >> RrDefaultGreenOffset) & 0xFF;
  g -= (g * s->bevel_dark_adjust) >> 8;
  b = (*down >> RrDefaultBlueOffset) & 0xFF;
  b -= (b * s->bevel_dark_adjust) >> 8;
  *down = (r << RrDefaultRedOffset) + (g << RrDefaultGreenOffset) + (b << RrDefaultBlueOffset);
}

static void create_bevel_colors(RrAppearance *l) {
  register gint r, g, b;

  r = l->surface.primary->r;
  r += (r * l->surface.bevel_light_adjust) >> 8;
  g = l->surface.primary->g;
  g += (g * l->surface.bevel_light_adjust) >> 8;
  b = l->surface.primary->b;
  b += (b * l->surface.bevel_light_adjust) >> 8;
  r = MIN(r, 0xFF);
  g = MIN(g, 0xFF);
  b = MIN(b, 0xFF);
  g_assert(!l->surface.bevel_light);
  l->surface.bevel_light = RrColorNew(l->inst, r, g, b);

  r = l->surface.primary->r;
  r -= (r * l->surface.bevel_dark_adjust) >> 8;
  g = l->surface.primary->g;
  g -= (g * l->surface.bevel_dark_adjust) >> 8;
  b = l->surface.primary->b;
  b -= (b * l->surface.bevel_dark_adjust) >> 8;
  g_assert(!l->surface.bevel_dark);
  l->surface.bevel_dark = RrColorNew(l->inst, r, g, b);
}

static inline void repeat_pixel(RrPixel32 *start, gint w) {
  register gint x;
  RrPixel32 *dest = start + 1;

  if (w < 8) {
    for (x = w - 1; x > 0; --x) *(dest++) = *start;
  } else {
    gchar *cdest;
    gint lenbytes = 4 * sizeof(RrPixel32);

    for (x = 3; x > 0; --x) *(dest++) = *start;

    cdest = (gchar *)dest;
    for (x = (w - 4) * sizeof(RrPixel32); x > 0;) {
      memcpy(cdest, start, lenbytes);
      x -= lenbytes;
      cdest += lenbytes;
      lenbytes <<= 1;
      if (lenbytes > x) lenbytes = x;
    }
  }
}

static void gradient_parentrelative(RrAppearance *a, gint w, gint h) {
  RrPixel32 *source, *dest;
  gint sw, sh, partial_w, partial_h;
  register gint i;

  g_assert(a->surface.parent);
  g_assert(a->surface.parent->w);

  sw = a->surface.parent->w;
  sh = a->surface.parent->h;

  if (a->surface.relief != RR_RELIEF_FLAT &&
      (a->surface.parent->surface.relief != RR_RELIEF_FLAT || a->surface.parent->surface.border) &&
      !a->surface.parentx && !a->surface.parenty && sw == w && sh == h) {
    RrSurface old = a->surface;
    a->surface = a->surface.parent->surface;

    a->surface.relief = RR_RELIEF_FLAT;
    a->surface.border = FALSE;

    a->surface.pixel_data = old.pixel_data;

    RrRender(a, w, h);
    a->surface = old;
  } else {
    source = (a->surface.parent->surface.pixel_data + a->surface.parentx + sw * a->surface.parenty);
    dest = a->surface.pixel_data;

    partial_w = (a->surface.parentx + w > sw) ? sw - a->surface.parentx : w;
    partial_h = (a->surface.parenty + h > sh) ? sh - a->surface.parenty : h;

    for (i = 0; i < partial_h; i++, source += sw, dest += w) {
      memcpy(dest, source, partial_w * sizeof(RrPixel32));
    }
  }
}

static void gradient_solid(RrAppearance *l, gint w, gint h) {
  register gint i;
  RrPixel32 pix;
  RrPixel32 *data = l->surface.pixel_data;
  RrSurface *sp = &l->surface;
  gint left = 0, top = 0, right = w - 1, bottom = h - 1;

  pix = (sp->primary->r << RrDefaultRedOffset) + (sp->primary->g << RrDefaultGreenOffset) +
        (sp->primary->b << RrDefaultBlueOffset);

  for (i = 0; i < w * h; i++) *data++ = pix;

  if (sp->interlaced) return;

  XFillRectangle(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->primary), 0, 0, w, h);

  switch (sp->relief) {
    case RR_RELIEF_RAISED:
      if (!sp->bevel_dark) create_bevel_colors(l);

      switch (sp->bevel) {
        case RR_BEVEL_1:
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), left, bottom, right, bottom);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), right, bottom, right, top);

          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), left, top, right, top);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), left, bottom, left, top);
          break;
        case RR_BEVEL_2:
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), left + 2, bottom - 1, right - 2,
                    bottom - 1);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), right - 1, bottom - 1, right - 1,
                    top + 1);

          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), left + 2, top + 1, right - 2, top + 1);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), left + 1, bottom - 1, left + 1, top + 1);
          break;
        default:
          g_assert_not_reached();
      }
      break;
    case RR_RELIEF_SUNKEN:
      if (!sp->bevel_dark) create_bevel_colors(l);

      switch (sp->bevel) {
        case RR_BEVEL_1:
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), left, bottom, right, bottom);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), right, bottom, right, top);

          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), left, top, right, top);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), left, bottom, left, top);
          break;
        case RR_BEVEL_2:
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), left + 2, bottom - 1, right - 2,
                    bottom - 1);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_light), right - 1, bottom - 1, right - 1,
                    top + 1);

          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), left + 2, top + 1, right - 2, top + 1);
          XDrawLine(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->bevel_dark), left + 1, bottom - 1, left + 1, top + 1);
          break;
        default:
          g_assert_not_reached();
      }
      break;
    case RR_RELIEF_FLAT:
      if (sp->border) {
        XDrawRectangle(RrDisplay(l->inst), l->pixmap, RrColorGC(sp->border_color), left, top, right, bottom);
      }
      break;
    default:
      g_assert_not_reached();
  }
}

#define VARS(x)                                          \
  gint len##x;                                           \
  guint color##x[3];                                     \
  gint cdelta##x[3], error##x[3] = {0, 0, 0}, inc##x[3]; \
  gboolean bigslope##x[3] /* color slope > 1 */

#define SETUP(x, from, to, w)                \
  len##x = w;                                \
  color##x[0] = from->r;                     \
  color##x[1] = from->g;                     \
  color##x[2] = from->b;                     \
  cdelta##x[0] = to->r - from->r;            \
  cdelta##x[1] = to->g - from->g;            \
  cdelta##x[2] = to->b - from->b;            \
  for (int i = 0; i < 3; ++i) {              \
    inc##x[i] = (cdelta##x[i] < 0) ? -1 : 1; \
    cdelta##x[i] = abs(cdelta##x[i]);        \
    bigslope##x[i] = cdelta##x[i] > w;       \
  }

#define COLOR_RR(x, c) \
  c->r = color##x[0];  \
  c->g = color##x[1];  \
  c->b = color##x[2]

#define COLOR(x) \
  ((color##x[0] << RrDefaultRedOffset) + (color##x[1] << RrDefaultGreenOffset) + (color##x[2] << RrDefaultBlueOffset))

#define INCREMENT(x, i) (inc##x[i])

#define NEXT(x)                                   \
  {                                               \
    for (int i = 0; i < 3; ++i) {                 \
      if (cdelta##x[i] == 0) continue;            \
      if (bigslope##x[i]) {                       \
        while (1) {                               \
          color##x[i] += INCREMENT(x, i);         \
          error##x[i] += len##x;                  \
          if (error##x[i] << 1 >= cdelta##x[i]) { \
            error##x[i] -= cdelta##x[i];          \
            break;                                \
          }                                       \
        }                                         \
      } else {                                    \
        error##x[i] += cdelta##x[i];              \
        if (error##x[i] << 1 >= len##x) {         \
          color##x[i] += INCREMENT(x, i);         \
          error##x[i] -= len##x;                  \
        }                                         \
      }                                           \
    }                                             \
  }

static void gradient_splitvertical(RrAppearance *a, gint w, gint h) {
  RrSurface *sf = &a->surface;
  RrPixel32 *data;
  gint y1sz, y2sz, y3sz;

  VARS(y1);
  VARS(y2);
  VARS(y3);

  if (h <= 5) {
    y1sz = MAX(h / 2, 0);
    y2sz = (h < 3) ? 0 : (h & 1);
    y3sz = MAX(h / 2, 1);
  } else {
    y1sz = h / 2 - (1 - (h & 1));
    y2sz = 1;
    y3sz = h / 2;
  }

  SETUP(y1, sf->split_primary, sf->primary, y1sz);
  if (y2sz) {
    SETUP(y2, sf->primary, sf->secondary, y2sz + 2);
    NEXT(y2);
  }
  SETUP(y3, sf->secondary, sf->split_secondary, y3sz);

  data = sf->pixel_data;

  for (int y1 = y1sz - 1; y1 > 0; --y1) {
    *data = COLOR(y1);
    data += w;
    NEXT(y1);
  }
  *data = COLOR(y1);
  data += w;

  if (y2sz) {
    for (int y2 = y2sz - 1; y2 > 0; --y2) {
      *data = COLOR(y2);
      data += w;
      NEXT(y2);
    }
    *data = COLOR(y2);
    data += w;
  }

  for (int y3 = y3sz - 1; y3 > 0; --y3) {
    *data = COLOR(y3);
    data += w;
    NEXT(y3);
  }

  *data = COLOR(y3);

  data = sf->pixel_data;
  for (int y1 = h; y1 > 0; --y1) {
    repeat_pixel(data, w);
    data += w;
  }
}

static void gradient_horizontal(RrSurface *sf, gint w, gint h) {
  RrPixel32 *data = sf->pixel_data, *datav;
  gchar *datac;
  gint cpbytes;

  VARS(x);
  SETUP(x, sf->primary, sf->secondary, w);

  datav = data;
  for (int x = w - 1; x > 0; --x) {
    *datav = COLOR(x);
    ++datav;
    NEXT(x);
  }
  *datav = COLOR(x);
  ++datav;

  datac = (gchar *)datav;
  cpbytes = 1 * w * sizeof(RrPixel32);
  for (int y = (h - 1) * w * sizeof(RrPixel32); y > 0;) {
    memcpy(datac, data, cpbytes);
    y -= cpbytes;
    datac += cpbytes;
    cpbytes <<= 1;
    if (cpbytes > y) cpbytes = y;
  }
}

static void gradient_mirrorhorizontal(RrSurface *sf, gint w, gint h) {
  RrPixel32 *data = sf->pixel_data, *datav;
  gchar *datac;
  gint half1 = (w + 1) / 2, half2 = w / 2, cpbytes;

  VARS(x);

  SETUP(x, sf->primary, sf->secondary, half1);
  datav = data;
  for (int x = half1 - 1; x > 0; --x) {
    *datav = COLOR(x);
    ++datav;
    NEXT(x);
  }
  *datav = COLOR(x);
  ++datav;

  if (half2 > 0) {
    SETUP(x, sf->secondary, sf->primary, half2);
    for (int x = half2 - 1; x > 0; --x) {
      *datav = COLOR(x);
      ++datav;
      NEXT(x);
    }
    *datav = COLOR(x);
    ++datav;
  }

  datac = (gchar *)datav;
  cpbytes = 1 * w * sizeof(RrPixel32);
  for (int y = (h - 1) * w * sizeof(RrPixel32); y > 0;) {
    memcpy(datac, data, cpbytes);
    y -= cpbytes;
    datac += cpbytes;
    cpbytes <<= 1;
    if (cpbytes > y) cpbytes = y;
  }
}

static void gradient_vertical(RrSurface *sf, gint w, gint h) {
  RrPixel32 *data;
  VARS(y);
  SETUP(y, sf->primary, sf->secondary, h);

  data = sf->pixel_data;

  for (int y = h - 1; y > 0; --y) {
    *data = COLOR(y);
    data += w;
    NEXT(y);
  }
  *data = COLOR(y);

  data = sf->pixel_data;
  for (int y = h; y > 0; --y) {
    repeat_pixel(data, w);
    data += w;
  }
}

static void gradient_diagonal(RrSurface *sf, gint w, gint h) {
  RrPixel32 *data = sf->pixel_data;
  RrColor left, right;
  RrColor extracorner;

  VARS(lefty);
  VARS(righty);
  VARS(x);

  extracorner.r = (sf->primary->r + sf->secondary->r) / 2;
  extracorner.g = (sf->primary->g + sf->secondary->g) / 2;
  extracorner.b = (sf->primary->b + sf->secondary->b) / 2;

  SETUP(lefty, sf->primary, (&extracorner), h);
  SETUP(righty, (&extracorner), sf->secondary, h);

  for (int y = h - 1; y > 0; --y) {
    COLOR_RR(lefty, (&left));
    COLOR_RR(righty, (&right));

    SETUP(x, (&left), (&right), w);

    for (int x = w - 1; x > 0; --x) {
      *(data++) = COLOR(x);
      NEXT(x);
    }
    *(data++) = COLOR(x);
    NEXT(lefty);
    NEXT(righty);
  }
  COLOR_RR(lefty, (&left));
  COLOR_RR(righty, (&right));

  SETUP(x, (&left), (&right), w);
  for (int x = w - 1; x > 0; --x) {
    *(data++) = COLOR(x);
    NEXT(x);
  }
  *data = COLOR(x);
}

static void gradient_crossdiagonal(RrSurface *sf, gint w, gint h) {
  RrPixel32 *data = sf->pixel_data;
  RrColor left, right;
  RrColor extracorner;

  VARS(lefty);
  VARS(righty);
  VARS(x);

  extracorner.r = (sf->primary->r + sf->secondary->r) / 2;
  extracorner.g = (sf->primary->g + sf->secondary->g) / 2;
  extracorner.b = (sf->primary->b + sf->secondary->b) / 2;

  SETUP(lefty, (&extracorner), sf->secondary, h);
  SETUP(righty, sf->primary, (&extracorner), h);

  for (int y = h - 1; y > 0; --y) {
    COLOR_RR(lefty, (&left));
    COLOR_RR(righty, (&right));

    SETUP(x, (&left), (&right), w);

    for (int x = w - 1; x > 0; --x) {
      *(data++) = COLOR(x);
      NEXT(x);
    }
    *(data++) = COLOR(x);
    NEXT(lefty);
    NEXT(righty);
  }
  COLOR_RR(lefty, (&left));
  COLOR_RR(righty, (&right));

  SETUP(x, (&left), (&right), w);
  for (int x = w - 1; x > 0; --x) {
    *(data++) = COLOR(x);
    NEXT(x);
  }
  *data = COLOR(x);
}

static void gradient_pyramid(RrSurface *sf, gint w, gint h) {
  RrPixel32 *ldata, *rdata;
  RrPixel32 *cp;
  RrColor left, right;
  RrColor extracorner;
  register gint x, y, halfw, halfh, midx, midy;

  VARS(lefty);
  VARS(righty);
  VARS(x);

  extracorner.r = (sf->primary->r + sf->secondary->r) / 2;
  extracorner.g = (sf->primary->g + sf->secondary->g) / 2;
  extracorner.b = (sf->primary->b + sf->secondary->b) / 2;

  halfw = w >> 1;
  halfh = h >> 1;
  midx = w - halfw - halfw;
  midy = h - halfh - halfh;

  SETUP(lefty, sf->primary, (&extracorner), halfh + midy);
  SETUP(righty, (&extracorner), sf->secondary, halfh + midy);

  ldata = sf->pixel_data;
  rdata = ldata + w - 1;
  for (y = halfh + midy; y > 0; --y) {
    RrPixel32 c;

    COLOR_RR(lefty, (&left));
    COLOR_RR(righty, (&right));

    SETUP(x, (&left), (&right), halfw + midx);

    for (x = halfw + midx - 1; x > 0; --x) {
      c = COLOR(x);
      *(ldata++) = *(rdata--) = c;

      NEXT(x);
    }
    c = COLOR(x);
    *ldata = *rdata = c;
    ldata += halfw + 1;
    rdata += halfw - 1 + midx + w;

    NEXT(lefty);
    NEXT(righty);
  }

  ldata = sf->pixel_data + (halfh - 1) * w;
  cp = ldata + (midy + 1) * w;
  for (y = halfh; y > 0; --y) {
    memcpy(cp, ldata, w * sizeof(RrPixel32));
    ldata -= w;
    cp += w;
  }
}
