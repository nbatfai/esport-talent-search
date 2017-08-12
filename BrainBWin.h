#ifndef BrainBWin_H
#define BrainBWin_H

/**
 * @brief Benchmarking Cognitive Abilities of the Brain with Computer Games
 *
 * @file BrainBWin.h
 * @author  Norbert Bátfai <nbatfai@gmail.com>
 * @version 0.0.1
 *
 * @section LICENSE
 *
 * Copyright (C) 2017 Norbert Bátfai, nbatfai@gmail.com
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

#include <QKeyEvent>
#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QFile>
#include <QString>
#include <QCloseEvent>
#include <QDate>
#include <QDir>
#include <QDateTime>
#include "BrainBThread.h"

enum playerstate {
    lost,
    found
};

class BrainBWin : public QMainWindow
{
    Q_OBJECT

    BrainBThread *brainBThread;
    QPixmap pixmap;
    Heroes *heroes;

    int mouse_x;
    int mouse_y;
    int nofLost {0};
    int nofFound {0};

    bool firstLost {false};
    bool start {false};
    playerstate state = lost;
    std::vector<int> lost2found;
    std::vector<int> found2lost;

    QString dir;

public:
    BrainBWin ( int w = 256, int h = 256, QWidget *parent = 0 );

    void closeEvent ( QCloseEvent *e ) {

        if ( save ( 4242 ) ) {
            e->accept();
        } else {
            e->ignore();
        }


    }

    virtual ~BrainBWin();
    void paintEvent ( QPaintEvent * );
    void keyPressEvent ( QKeyEvent *event );
    void mouseMoveEvent ( QMouseEvent *event );

    double mean ( std::vector<int> vect ) {

        double sum = std::accumulate ( vect.begin (), vect.end (), 0.0 );
        return  sum / vect.size();

    }

    double var ( std::vector<int> vect, double mean ) {

        double accum = 0.0;

        std::for_each ( vect.begin (), vect.end (), [&] ( const double d ) {
            accum += ( d - mean ) * ( d - mean );
        } );

        return sqrt ( accum / ( vect.size()-1 ) );
    }


    bool save ( int t ) {

        bool ret = false;

        if ( !QDir ( dir ).exists() )
            if ( !QDir().mkdir ( dir ) ) {
                return false;
            }

        QString name = dir + "/BrainB-Test3-" + QString::number ( t );
        QFile file ( name + "-screenimage.png" );
        if ( file.open ( QIODevice::WriteOnly ) ) {
            ret = pixmap.save ( &file, "PNG" );
        }

        QFile tfile ( name + "-stats.txt" );
        ret = tfile.open ( QIODevice::WriteOnly | QIODevice::Text );
        if ( ret ) {
            QTextStream textStremam ( &tfile );

            textStremam << "time      : " <<  brainBThread->getT() << "\n";
            textStremam << "bps       : " <<  brainBThread->get_bps() << "\n";
            textStremam << "noc       : " <<  brainBThread->nofHeroes() << "\n";

            textStremam << "lost      : "  << "\n";
            std::vector<int> l = brainBThread->lostV();
            for ( int n : l ) {
                textStremam << n << ' ';
            }
            textStremam <<  "\n";
            int m = mean ( l );
            textStremam << "mean      : " <<  m << "\n";
            textStremam << "var       : " <<  var ( l, m ) << "\n";

            textStremam << "found     : " ;
            std::vector<int> f = brainBThread->foundV();
            for ( int n : f ) {
                textStremam << n << ' ';
            }
            textStremam <<  "\n";
            m = mean ( f );
            textStremam << "mean      : " <<  m << "\n";
            textStremam << "var       : " <<  var ( f, m ) << "\n";

            textStremam << "lost2found: "  ;
            for ( int n : lost2found ) {
                textStremam << n << ' ';
            }
            textStremam <<  "\n";
            m = mean ( lost2found );
            textStremam << "mean      : " <<  m << "\n";
            textStremam << "var       : " <<  var ( lost2found, m ) << "\n";

            textStremam << "found2lost: "  ;
            for ( int n : found2lost ) {
                textStremam << n << ' ';
            }
            textStremam <<  "\n";
            m = mean ( found2lost );
            textStremam << "mean      : " <<  m << "\n";
            textStremam << "var       : " <<  var ( found2lost, m ) << "\n";

            tfile.close();
        }
        return ret;
    }

public slots :

    void updateHeroes ( const QImage &image, const int &x, const int &y );
    void stats ( const int &t );

};

#endif // BrainBWin
