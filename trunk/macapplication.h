/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2015 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: http://www.polyphone.fr/                             **
**             Date: 01.01.2013                                           **
***************************************************************************/

#ifndef MACAPPLICATION_H
#define MACAPPLICATION_H

#include <QApplication>
#include <QFileOpenEvent>
#include <QStringList>
#include "sf2_types.h"

class MacApplication: public QApplication
{
    Q_OBJECT
public:
    MacApplication(int &argc, char **argv, int flags = ApplicationFlags) :
        QApplication(argc, argv, flags)
    {}

signals:
    void openFile(QString path);

protected:
    bool event(QEvent * event)
    {
        if (event->type() == QEvent::FileOpen)
        {
            QString path = static_cast<QFileOpenEvent*>(event)->file();
            emit(openFile(path));
            return true;
        }
        return QApplication::event(event);
    }
};


#endif // MACAPPLICATION_H
