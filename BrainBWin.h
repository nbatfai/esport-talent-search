#ifndef BrainBWin_H
#define BrainBWin_H

/**
 * @brief Benchmarking Cognitive Abilities of the Brain with Computer Games
 *
 * @file BrainBWin.h
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
    int yshift {50};
    int nofLost {0};
    int nofFound {0};

    int xs, ys;

    bool firstLost {false};
    bool start {false};
    playerstate state = lost;
    std::vector<int> lost2found;
    std::vector<int> found2lost;

    QString statDir;

public:
    static const QString appName;
    static const QString appVersion;
    BrainBWin ( int w = 256, int h = 256, QWidget *parent = 0 );

    void closeEvent ( QCloseEvent *e ) {

        if ( save ( brainBThread->getT() ) ) {
            brainBThread->finish();
            e->accept();
        } else {
            e->ignore();
        }

    }

    virtual ~BrainBWin();
    void paintEvent ( QPaintEvent * );
    void keyPressEvent ( QKeyEvent *event );
    void mouseMoveEvent ( QMouseEvent *event );
    void mousePressEvent ( QMouseEvent *event );
    void mouseReleaseEvent ( QMouseEvent *event );

    double mean ( std::vector<int> vect ) {

        if ( vect.size() > 0 ) {
            double sum = std::accumulate ( vect.begin (), vect.end (), 0.0 );
            return  sum / vect.size();
        } else {
            return 0.0;
        }
    }

    double var ( std::vector<int> vect, double mean ) {

        if ( vect.size() > 1 ) {

            double accum = 0.0;

            std::for_each ( vect.begin (), vect.end (), [&] ( const double d ) {
                accum += ( d - mean ) * ( d - mean );
            } );

            return sqrt ( accum / ( vect.size()-1 ) );
        } else {
            return 0.0;
        }

    }

    void millis2minsec ( int millis, int &min, int &sec ) {

        sec = ( millis * 100 ) / 1000;
        min = sec / 60;
        sec = sec - min * 60;

    }

    bool save ( int t ) {

        bool ret = false;

        if ( !QDir ( statDir ).exists() )
            if ( !QDir().mkdir ( statDir ) ) {
                return false;
            }

        QString name = statDir + "/Test-" + QString::number ( t );
        QFile file ( name + "-screenimage.png" );
        if ( file.open ( QIODevice::WriteOnly ) ) {
            ret = pixmap.save ( &file, "PNG" );
        }

        QFile tfile ( name + "-stats.txt" );
        ret = tfile.open ( QIODevice::WriteOnly | QIODevice::Text );
        if ( ret ) {
            QTextStream textStremam ( &tfile );

            textStremam << appName + " " + appVersion << "\n";
            textStremam << "time      : " <<  brainBThread->getT() << "\n";
            textStremam << "bps       : " <<  brainBThread->get_bps() << "\n";
            textStremam << "noc       : " <<  brainBThread->nofHeroes() << "\n";
            textStremam << "nop       : " <<  brainBThread->get_nofPaused() << "\n";

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
            int m1 = m = mean ( lost2found );
            textStremam << "mean      : " <<  m << "\n";
            textStremam << "var       : " <<  var ( lost2found, m ) << "\n";

            textStremam << "found2lost: "  ;
            for ( int n : found2lost ) {
                textStremam << n << ' ';
            }
            textStremam <<  "\n";
            int m2 = m = mean ( found2lost );
            textStremam << "mean      : " <<  m << "\n";
            textStremam << "var       : " <<  var ( found2lost, m ) << "\n";

            if ( m1 < m2 ) {
                textStremam << "mean(lost2found) < mean(found2lost)" << "\n";
            }

            int min, sec;
            millis2minsec ( t, min, sec );
            textStremam << "time      : " <<  min  << ":"  << sec << "\n";

            double res = ( ( ( ( double ) m1+ ( double ) m2 ) /2.0 ) /8.0 ) /1024.0;
            textStremam << "U R about " << res << " Kilobytes\n";

            tfile.close();
        }
        return ret;
    }

public slots :

    void updateHeroes ( const QImage &image, const int &x, const int &y );
    //void stats ( const int &t );
    void endAndStats ( const int &t );
};

#endif // BrainBWin
