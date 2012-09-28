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

#include "previewwidget.h"

PreviewWidget::PreviewWidget()
    : painter( Cairo::RefPtr<Cairo::Context>() )
{
}

PreviewWidget::~PreviewWidget()
{
}

bool PreviewWidget::on_draw( const Cairo::RefPtr<Cairo::Context>& context )
{
    painter.set_context( context );

    int w = get_width( );
    int h = get_height( );

    context->save();

    if( h < w )
    {
        context->translate( (w-h)/2, 0 );
        context->scale( h, h );
    }
    else
    {
        context->translate( 0, (h-w)/2 );
        context->scale( w, w );
    }

    context->scale( 0.5, -0.5 );
    context->translate( 1, -1 );

    painter.draw( );

    context->restore();

    return true;
}
