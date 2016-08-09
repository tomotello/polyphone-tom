/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2014-2015 Davy Triponney                                **
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

#include "sfarkfilemanager.h"

#include <QFile>
#include <QDataStream>

int SfArkFileManager::_maxFileHandler = 0;

SfArkFileManager::SfArkFileManager()
{}

SfArkFileManager::~SfArkFileManager()
{
    clearFiles();
}

// Return file handler if success, otherwise -1
int SfArkFileManager::openReadOnly(const char * name)
{
    int handler = _maxFileHandler;

    if (_mapName.contains(name))
        handler = _mapName.value(name);
    else
    {
        QFile * file = new QFile(name);
        if (file->open(QIODevice::ReadOnly))
        {
            _mapName[name] = _maxFileHandler;
            _mapFile[_maxFileHandler] = file;
            _maxFileHandler++;
        }
        else
        {
            delete file;
            return -1;
        }
    }

    if (_mapFile.contains(handler))
        _mapDataStream[handler] = new QDataStream(_mapFile[handler]);
    else
        _mapDataStream[handler] = new QDataStream(_mapByteArray[handler], QIODevice::ReadOnly);

    return handler;
}

// Return file handler
int SfArkFileManager::create(const char *name)
{
    int handler = _maxFileHandler;
    if (_mapName.contains(name))
        handler = _mapName.value(name);
    else
    {
        _mapName[name] = _maxFileHandler;
        _mapByteArray[_maxFileHandler] = new QByteArray();
        _maxFileHandler++;
    }

    _mapDataStream[handler] = new QDataStream(_mapByteArray[handler], QIODevice::ReadWrite);

    return handler;
}

// Return true if success, otherwise false
void SfArkFileManager::close(int fileHandler)
{
    // Suppression du DataStream
    if (_mapDataStream.contains(fileHandler))
         delete _mapDataStream.take(fileHandler);

    // Fermeture si fichier ouvert, pas de suppression des byteArrays
    if (_mapFile.contains(fileHandler))
    {
        QFile * file = _mapFile.take(fileHandler);
        file->close();
        delete file;

        QString key = _mapName.key(fileHandler, "");
        if (!key.isEmpty())
            _mapName.remove(key);
    }
}

// Return true if success, otherwise false
int SfArkFileManager::setPos(int fileHandler, long offset)
{
    if (_mapDataStream.contains(fileHandler))
    {
        QDataStream * stream = _mapDataStream.value(fileHandler);
        QIODevice * device = stream->device();
        return device->seek(offset + device->pos());
    }
    return false;
}

// Return the number of bytes read, -1 otherwise
int SfArkFileManager::read(int fileHandler, char * ptr, unsigned int count)
{
    if (_mapDataStream.contains(fileHandler))
    {
        QDataStream * stream = _mapDataStream.value(fileHandler);
        return stream->readRawData(ptr, count);
    }
    return -1;
}

// Return the number of bytes written, -1 otherwise
int SfArkFileManager::write(int fileHandler, const char *ptr, unsigned int count)
{
    if (_mapDataStream.contains(fileHandler))
    {
        QDataStream * stream = _mapDataStream.value(fileHandler);
        return stream->writeRawData(ptr, count);
    }
    return -1;
}

char * SfArkFileManager::retrieveData(const char *name, int &size)
{
    char * ret = NULL;
    if (_mapName.contains(name))
    {
        int handle = _mapName.value(name);
        if (_mapByteArray.contains(handle))
        {
            QByteArray * data = _mapByteArray.value(handle);
            ret = data->data();
            size = data->size();
        }
    }
    return ret;
}

void SfArkFileManager::clearFiles()
{
    _maxFileHandler = 0;

    _mapName.clear();

    QList<int> keys = _mapDataStream.keys();
    foreach (int key, keys)
        delete _mapDataStream.take(key);

    keys = _mapByteArray.keys();
    foreach (int key, keys)
        delete _mapByteArray.take(key);

    keys = _mapFile.keys();
    foreach (int key, keys)
    {
        QFile * file = _mapFile.take(key);
        file->close();
        delete file;
    }
}
