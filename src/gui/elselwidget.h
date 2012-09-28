#ifndef ELSELWIDGET_H
#define ELSELWIDGET_H

#include "../electrodes.h"
#include "../painter.h"

#include <gtkmm/fixed.h>
#include <gtkmm/checkbutton.h>
#include <vector>
#include <map>

class ElselWidget : public Gtk::Fixed
{
public:
    ElselWidget();
    virtual ~ElselWidget();

    void init_widgets( );

    virtual void on_size_allocate( Gtk::Allocation& allocation );

    virtual bool on_draw( const Cairo::RefPtr<Cairo::Context>& context );

    virtual Gtk::SizeRequestMode get_request_mode_vfunc() const;
    virtual void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
    virtual void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
    virtual void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const;
    virtual void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const;

private:
    typedef Gtk::CheckButton ButtonWidget;
    typedef std::map<std::string, ButtonWidget*> map_label_button;
    typedef std::map<ButtonWidget*, std::string> map_button_label;

    map_label_button buttons;
    map_button_label labels;

    Electrodes el;
    Painter painter;
    double scale;
};

#endif // ELSELWIDGET_H
