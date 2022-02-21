/*
 * Runtime dark mode for Rack plugins
 * Copyright (C) 2022 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <nanosvg.h>

static inline
bool invertPaint(NSVGpaint& paint)
{
    if (paint.type == NSVG_PAINT_NONE)
        return true;
    if (paint.type != NSVG_PAINT_COLOR)
        return false;

    // AnimatedCircuits logo
    if (paint.color == 0xff303030)
    {
        paint.color = 0xffefefef;
        return true;
    }

    switch (paint.color)
    {
    // AnimatedCircuits blue tone
    case 0xffa0783c:
        return false;
    // pure black (convert to not quite pure white)
    case 0xff000000:
        paint.color = 0xffd0d0d0;
        return true;
    // all others (direct invert)
    default:
        paint.color = (paint.color & 0xff000000)
                    | (0xff0000 - (paint.color & 0xff0000))
                    | (0xff00 - (paint.color & 0xff00))
                    | (0xff - (paint.color & 0xff));
        return true;
    }
}

static inline
void invertColorsOfSVG(NSVGimage* const handle)
{
    for (NSVGshape* shape = handle->shapes; shape != nullptr; shape = shape->next)
    {
        if (invertPaint(shape->fill))
            invertPaint(shape->stroke);
    }
}
