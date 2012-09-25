#include "guimain.h"

#include <QApplication>
#include <QTextEdit>

int gui_main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    QTextEdit textEdit;
    textEdit.show();
    app.exec();
}
