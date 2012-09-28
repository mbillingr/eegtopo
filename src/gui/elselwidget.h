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

    void set_visible( std::string label, bool vis );
    void set_visible(std::vector<std::string> labels );

    virtual void on_size_allocate( Gtk::Allocation& allocation );

    virtual bool on_draw( const Cairo::RefPtr<Cairo::Context>& context );

    virtual Gtk::SizeRequestMode get_request_mode_vfunc() const;
    virtual void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
    virtual void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
    virtual void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const;
    virtual void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const;

    const Painter& get_painter( ) const { return painter; }

private:
    typedef Gtk::CheckButton ButtonWidget;
    typedef std::map<std::string, ButtonWidget*> map_label_button;
    typedef std::map<ButtonWidget*, std::string> map_button_label;

    map_label_button buttons;
    map_button_label labels;

    Painter painter;
    double scale;
};

#endif // ELSELWIDGET_H
