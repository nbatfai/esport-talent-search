/**
 * @brief Benchmarking Cognitive Abilities of the Brain with Computer Games
 *
 * @file BrainBWin.cpp
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

#include "BrainBWin.h"

BrainBWin::BrainBWin(int w, int h, QWidget *parent) : QMainWindow(parent)
{
    QString name = "NEMESPOR BrainB Test 3";

    setWindowTitle(name);
    setFixedSize(QSize(w, h));

    dir = name + " - " + QDate::currentDate().toString() + QString::number(QDateTime::currentMSecsSinceEpoch());

    brainBThread = new BrainBThread(w, h);
    brainBThread->start();

    connect(brainBThread, SIGNAL(heroesChanged(QImage, int, int)),
            this, SLOT(updateHeroes(QImage, int, int)));

    connect(brainBThread, SIGNAL(stats(int)),
            this, SLOT(stats(int)));

}

void BrainBWin::stats(const int &t)
{
    save(t);
}

void BrainBWin::updateHeroes(const QImage &image, const int &x, const int &y)
{

    if (start) {

        int dist = (this->mouse_x - x) * (this->mouse_x - x) + (this->mouse_y - y) * (this->mouse_y - y);

        if (dist > 121) {
            ++nofLost;
            nofFound = 0;
            if (nofLost > 12) {

                if (state == found && firstLost) {
                    found2lost.push_back(brainBThread->get_bps());
                }

                firstLost = true;

                state = lost;
                nofLost = 0;
                qDebug() << "LOST";
                double mean = brainBThread->meanLost();
                qDebug() << mean;

                brainBThread->decComp();
            }
        } else {
            ++nofFound;
            nofLost = 0;
            if (nofFound > 12) {

                if (state == lost && firstLost) {
                    lost2found.push_back(brainBThread->get_bps());
                }

                state = found;
                nofFound = 0;
                qDebug() << "FOUND";
                double mean = brainBThread->meanFound();
                qDebug() << mean;

                brainBThread->incComp();
            }

        }

    }
    pixmap = QPixmap::fromImage(image);
    update();
}

void BrainBWin::paintEvent(QPaintEvent *)
{
    if (pixmap.isNull()) {
        return;
    }

    QPainter qpainter(this);

    qpainter.drawPixmap(0, 0, pixmap);
    qpainter.drawText(10, 20, "Press and hold the mouse button on Samu Entropy");

    qpainter.end();
}

void BrainBWin::mouseMoveEvent(QMouseEvent *event)
{

    start = true;
    mouse_x = event->pos().x();
    mouse_y = event->pos().y();

}

void BrainBWin::keyPressEvent(QKeyEvent *event)
{

    if (event->key() == Qt::Key_S) {
        save(42);
    }

}

BrainBWin::~BrainBWin()
{

}
