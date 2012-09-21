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
