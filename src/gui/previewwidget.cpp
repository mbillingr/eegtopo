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
