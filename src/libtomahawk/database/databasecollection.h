/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2010-2011, Christian Muehlhaeuser <muesli@tomahawk-player.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Tomahawk is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATABASECOLLECTION_H
#define DATABASECOLLECTION_H

#include <QDir>

#include "collection.h"
#include "source.h"
#include "typedefs.h"

#include "dllmacro.h"

class DLLEXPORT DatabaseCollection : public Tomahawk::Collection
{
Q_OBJECT

public:
    explicit DatabaseCollection( const Tomahawk::source_ptr& source, QObject* parent = 0 );
    ~DatabaseCollection()
    {
        qDebug() << Q_FUNC_INFO;
    }

    virtual void loadTracks();
    virtual void loadPlaylists();
    virtual void loadAutoPlaylists();
    virtual void loadStations();

    virtual QList< Tomahawk::playlist_ptr > playlists();
    virtual QList< Tomahawk::query_ptr > tracks();
    virtual QList< Tomahawk::dynplaylist_ptr > autoPlaylists();
    virtual QList< Tomahawk::dynplaylist_ptr > stations();

public slots:
    virtual void addTracks( const QList<QVariant>& newitems );
    virtual void removeTracks( const QDir& dir );

private slots:
    void stationCreated( const Tomahawk::source_ptr& source, const QVariantList& data );
    void autoPlaylistCreated( const Tomahawk::source_ptr& source, const QVariantList& data );
};

#endif // DATABASECOLLECTION_H
