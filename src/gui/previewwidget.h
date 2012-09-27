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
