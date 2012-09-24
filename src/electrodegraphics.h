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

#ifndef ELECTRODEGRAPHICS_HPP
#define ELECTRODEGRAPHICS_HPP

#include "membermap.h"
#include "colormap.h"
#include "electrodes.h"

#include <cairomm/fontface.h>
#include <string>
#include <map>

class ElectrodeGraphics : public MemberMap
{
public:
    ElectrodeGraphics( );
    ElectrodeGraphics(const std::string &label );
    ElectrodeGraphics(const std::string &label , double x, double y);

    void set_defaults( );

private:
};

#endif // ELECTRODEGRAPHICS_HPP
