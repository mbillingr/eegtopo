/*
 *  This file is part of EEGtopo.
 *
 *  Foobar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  Copyright 2012 Martin Billinger
 *
 */

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "../painter.h"

#include <gtkmm/drawingarea.h>

class PreviewWidget : public Gtk::DrawingArea
{
public:
    PreviewWidget( );
    virtual ~PreviewWidget( );

protected:
    Painter painter;

    virtual bool on_draw( const Cairo::RefPtr<Cairo::Context>& context );
};

#endif // PREVIEWWIDGET_H
