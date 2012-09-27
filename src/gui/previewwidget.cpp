#include "previewwidget.h"

#include "../painter.h"

PreviewWidget::PreviewWidget()
{
}

PreviewWidget::~PreviewWidget()
{
}

bool PreviewWidget::on_draw( const Cairo::RefPtr<Cairo::Context>& context )
{

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

    Painter painter( context );
    painter.draw_head( );
    painter.draw_grid( 2 );

    context->restore();

    return true;
}
