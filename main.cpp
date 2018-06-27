/**
 * @brief Benchmarking Cognitive Abilities of the Brain with Computer Games
 *
 * @file main.cpp
 * @author  Norbert Bátfai <nbatfai@gmail.com>
 * @version 6.0.1
 *
 * @section LICENSE
 *
 * Copyright (C) 2017, 2018 Norbert Bátfai, nbatfai@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 */

#include <QApplication>
#include <QTextStream>
#include <QtWidgets>
#include "BrainBWin.h"

int main ( int argc, char **argv )
{
        QApplication app ( argc, argv );

        QTextStream qout ( stdout );
        qout.setCodec ( "UTF-8" );

        qout << "\n" << BrainBWin::appName << QString::fromUtf8 ( " Copyright (C) 2017, 2018 Norbert Bátfai" ) << endl;

        qout << "This program is free software: you can redistribute it and/or modify it under" << endl;
        qout << "the terms of the GNU General Public License as published by the Free Software" << endl;
        qout << "Foundation, either version 3 of the License, or (at your option) any later" << endl;
        qout << "version.\n" << endl;

        qout << "This program is distributed in the hope that it will be useful, but WITHOUT" << endl;
        qout << "ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS" << endl;
        qout << "FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n" << endl;

        qout << QString::fromUtf8 ( "Ez a program szabad szoftver; terjeszthető illetve módosítható a Free Software" ) << endl;
        qout << QString::fromUtf8 ( "Foundation által kiadott GNU General Public License dokumentumában leírtak;" ) << endl;
        qout << QString::fromUtf8 ( "akár a licenc 3-as, akár (tetszőleges) későbbi változata szerint.\n" ) << endl;

        qout << QString::fromUtf8 ( "Ez a program abban a reményben kerül közreadásra, hogy hasznos lesz, de minden" ) << endl;
        qout << QString::fromUtf8 ( "egyéb GARANCIA NÉLKÜL, az ELADHATÓSÁGRA vagy VALAMELY CÉLRA VALÓ" ) << endl;
        qout << QString::fromUtf8 ( "ALKALMAZHATÓSÁGRA való származtatott garanciát is beleértve. További" ) << endl;
        qout << QString::fromUtf8 ( "részleteket a GNU General Public License tartalmaz.\n" ) << endl;

        qout << "http://gnu.hu/gplv3.html" << endl;

        QRect rect = QApplication::desktop()->availableGeometry();
        BrainBWin brainBWin ( rect.width(), rect.height() );
        brainBWin.setWindowState ( brainBWin.windowState() ^ Qt::WindowFullScreen );
        brainBWin.show();
        return app.exec();
}
