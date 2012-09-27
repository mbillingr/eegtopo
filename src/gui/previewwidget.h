#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <gtkmm/drawingarea.h>

class PreviewWidget : public Gtk::DrawingArea
{
public:
    PreviewWidget( );
    virtual ~PreviewWidget( );

protected:
    virtual bool on_draw( const Cairo::RefPtr<Cairo::Context>& context );
};

#endif // PREVIEWWIDGET_H
