#ifndef PARSER_HPP
#define PARSER_HPP

#include "painter.h"

#include <string>

class Parser
{
public:
    Parser();

    void parse( const std::string &line, Painter &painter );

    void parse_file( const std::string &filename, Painter &painter );

private:
};

#endif // PARSER_HPP
