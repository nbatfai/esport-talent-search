#ifndef BrainBThread_H
#define BrainBThread_H

/**
 * @brief Benchmarking Cognitive Abilities of the Brain with Computer Games
 *
 * @file BrainBThread.h
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

#include <QThread>
#include <QSize>
#include <QImage>
#include <QDebug>
#include <sstream>
#include <QPainter>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Hero;
typedef std::vector<Hero> Heroes;

class Hero
{

public:
    int x;
    int y;
    int color;
    int agility;
    int conds {0};
    std::string name;

    Hero ( int x=0, int  y=0, int color=0, int agility=1,  std::string name ="Samu Entropy" ) :
        x ( x ), y ( y ), color ( color ), agility ( agility ), name ( name )
    {}
    ~Hero() {}

    void move ( int max, int env ) {

        int newx = x+ ( ( ( double ) agility*1.0 ) * ( double ) ( std::rand() / ( RAND_MAX+1.0 ) )-agility/2 ) ;
        if ( newx-env > 0 && newx+env < max ) {
            x = newx;
        }
        int newy = y+ ( ( ( double ) agility*1.0 ) * ( double ) ( std::rand() / ( RAND_MAX+1.0 ) )-agility/2 );
        if ( newy-env > 0 && newy+env < max ) {
            y = newy;
        }

    }

};

class BrainBThread : public QThread
{
    Q_OBJECT

    Heroes heroes;
    int heroRectSize {40};

    cv::Mat prev {3*40, 3*40, CV_8UC3, cv::Scalar ( 0, 0, 255 ) };
    int bps;
    long time {0};
    long endTime {10*60*10};
    int delay {100};

    bool paused {true};
    int nofPaused {0};

    std::vector<int> lostBPS;
    std::vector<int> foundBPS;

    int w;
    int h;
    int dispShift {40};

public:
    BrainBThread ( int w = 256, int h = 256 );
    ~BrainBThread();

    void run();
    void pause();
    void set_paused(bool p);
    int getDelay() const {

        return delay;

    }
    void setDelay ( int delay ) {

        if ( delay > 0 ) {
            delay = delay;
        }

    }

    void devel() {

        for ( Hero & hero : heroes ) {

            hero.move ( w, w/10 );

        }

    }

    int nofHeroes () {

        return  heroes.size();

    }

    std::vector<int> &lostV () {

        return  lostBPS;

    }

    std::vector<int> &foundV () {

        return  foundBPS;

    }

    double meanLost () {

        return  mean ( lostBPS );

    }

    double varLost ( double mean ) {

        return  var ( lostBPS, mean );

    }

    double meanFound () {

        return  mean ( foundBPS );

    }

    double varFound ( double mean ) {

        return  var ( foundBPS, mean );

    }

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

    int get_bps() const {

        return bps;

    }

    bool get_paused() const {

        return paused;

    }
    
    int get_nofPaused() const {

        return nofPaused;

    }

    void decComp() {

        lostBPS.push_back ( bps );

        if ( heroes.size() > 1 ) {
            heroes.pop_back();
        }

        for ( Hero & hero : heroes ) {
            if ( hero.agility >= 5 ) {
                hero.agility -= 2;
            }

        }

    }

    void incComp() {

        foundBPS.push_back ( bps );

        if ( heroes.size() > 300 ) {

            return;

        }

        Hero other ( 200 +200.0*std::rand() / ( RAND_MAX+1.0 )-100,
                     200 +200.0*std::rand() / ( RAND_MAX+1.0 )-100,
                     255.0*std::rand() / ( RAND_MAX+1.0 ), 11, "New Entropy" );

        heroes.push_back ( other );

        for ( Hero & hero : heroes ) {

            ++hero.conds;
            if ( hero.conds == 3 ) {
                hero.conds = 0;
                hero.agility += 2;
            }

        }

    }

    void draw () {

        cv::Mat src ( w+3*heroRectSize, h+3*heroRectSize, CV_8UC3, cv::Scalar ( 0, 0, 255 ) );

        for ( Hero & hero : heroes ) {

            cv::Point x ( hero.x-heroRectSize+dispShift, hero.y-heroRectSize+dispShift );
            cv::Point y ( hero.x+heroRectSize+dispShift, hero.y+heroRectSize+dispShift );
            cv::rectangle ( src, x, y, cv::Scalar ( hero.color, 0, 0 ) );

            cv::putText ( src, hero.name, x, cv::FONT_HERSHEY_SIMPLEX, .35, cv::Scalar ( hero.color, 0, 0 ), 1 );

            cv::Point xc ( hero.x+dispShift, hero.y+dispShift );

            cv::circle ( src, xc, 11, cv::Scalar ( 155, 0, 0 ), CV_FILLED, 8, 0 );

            cv::Mat box = src ( cv::Rect ( x, y ) );
            cv::Mat cbox ( 2*heroRectSize, 2*heroRectSize, CV_8UC3, cv::Scalar ( 0, 120, 120 ) );
            box = cbox*.3 + box*.7;
        }

        cv::Mat comp;

        cv::Point focusx ( heroes[0].x-(3*heroRectSize)/2+dispShift, heroes[0].y-(3*heroRectSize)/2+dispShift );
        cv::Point focusy ( heroes[0].x+(3*heroRectSize)/2+dispShift, heroes[0].y+(3*heroRectSize)/2+dispShift );
        cv::Mat focus = src ( cv::Rect ( focusx, focusy ) );
		
        cv::compare ( prev, focus, comp, cv::CMP_NE );

        cv::Mat aRgb;
        cv::extractChannel ( comp, aRgb, 0 );

        bps = cv::countNonZero ( aRgb ) * 10;

        //qDebug()  << bps << " bits/sec";
	
	prev = focus;

        QImage dest( src.data, src.cols, src.rows, src.step, QImage::Format_RGB888 );
        dest=dest.rgbSwapped();
        dest.bits();

        emit heroesChanged ( dest, heroes[0].x, heroes[0].y );
        
    }

    long getT() const {

        return time;

    }

    void finish () {

        time = endTime;

    }
    
    
signals:

    void heroesChanged ( const QImage &image, const int &x, const int &y );
    void endAndStats ( const int &t );

};

#endif // BrainBThread_H
