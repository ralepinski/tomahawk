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

#include "musixmatchplugin.h"

#include <QNetworkReply>
#include <QDomDocument>

#include "utils/tomahawkutils.h"
#include "utils/logger.h"

using namespace Tomahawk::InfoSystem;


// for internal neatness

MusixMatchPlugin::MusixMatchPlugin()
    : InfoPlugin()
    , m_apiKey("61be4ea5aea7dd942d52b2f1311dd9fe")
{
    qDebug() << Q_FUNC_INFO;
    m_supportedGetTypes << Tomahawk::InfoSystem::InfoTrackLyrics;
}

MusixMatchPlugin::~MusixMatchPlugin()
{
    qDebug() << Q_FUNC_INFO;
}

void
MusixMatchPlugin::namChangedSlot( QNetworkAccessManager *nam )
{
    qDebug() << Q_FUNC_INFO;
    if( !nam )
        return;

    m_nam = QWeakPointer< QNetworkAccessManager >( nam );
}

void
MusixMatchPlugin::getInfo( uint requestId, Tomahawk::InfoSystem::InfoRequestData requestData )
{
    qDebug() << Q_FUNC_INFO;
    if( !isValidTrackData( requestId, requestData ) || !requestData.input.canConvert< QVariantMap >() || m_nam.isNull() || requestData.type != Tomahawk::InfoSystem::InfoTrackLyrics )
        return;
    QVariantMap hash = requestData.input.value< QVariantMap >();
    QString artist = hash["artistName"].toString();
    QString track = hash["trackName"].toString();
    if( artist.isEmpty() || track.isEmpty() )
    {
        emit info( requestId, requestData, QVariant() );
        return;
    }
    qDebug() << "artist is " << artist << ", track is " << track;
    QString requestString( "http://api.musixmatch.com/ws/1.1/track.search?format=xml&page_size=1&f_has_lyrics=1" );
    QUrl url( requestString );
    url.addQueryItem( "apikey", m_apiKey );
    url.addQueryItem( "q_artist", artist );
    url.addQueryItem( "q_track", track );
    QNetworkReply* reply = m_nam.data()->get( QNetworkRequest( url ) );
    reply->setProperty( "requestId", requestId );
    reply->setProperty( "requestData", QVariant::fromValue< Tomahawk::InfoSystem::InfoRequestData >( requestData ) );

    connect( reply, SIGNAL( finished() ), SLOT( trackSearchSlot() ) );
}

bool
MusixMatchPlugin::isValidTrackData( uint requestId, Tomahawk::InfoSystem::InfoRequestData requestData )
{
    qDebug() << Q_FUNC_INFO;
    if ( requestData.input.isNull() || !requestData.input.isValid() || !requestData.input.canConvert< QVariantMap >() )
    {
        emit info( requestId, requestData, QVariant() );
        qDebug() << "MusixMatchPlugin::isValidTrackData: Data null, invalid, or can't convert";
        return false;
    }
    QVariantMap hash = requestData.input.value< QVariantMap >();
    if ( hash[ "trackName" ].toString().isEmpty() )
    {
        emit info( requestId, requestData, QVariant() );
        qDebug() << "MusixMatchPlugin::isValidTrackData: Track name is empty";
        return false;
    }
    if ( hash[ "artistName" ].toString().isEmpty() )
    {
        emit info( requestId, requestData, QVariant() );
        qDebug() << "MusixMatchPlugin::isValidTrackData: No artist name found";
        return false;
    }
    return true;
}

void
MusixMatchPlugin::trackSearchSlot()
{
    qDebug() << Q_FUNC_INFO;
    QNetworkReply* oldReply = qobject_cast<QNetworkReply*>( sender() );
    if ( !oldReply )
        return; //timeout will handle it

    QDomDocument doc;
    doc.setContent(oldReply->readAll());
    qDebug() << doc.toString();
    QDomNodeList domNodeList = doc.elementsByTagName("track_id");
    if ( domNodeList.isEmpty() )
    {
        emit info( oldReply->property( "requestId" ).toUInt(), oldReply->property( "requestData" ).value< Tomahawk::InfoSystem::InfoRequestData >(), QVariant() );
        return;
    }
    QString track_id = domNodeList.at(0).toElement().text();
    QString requestString( "http://api.musixmatch.com/ws/1.1/track.lyrics.get?track_id=%1&format=xml&apikey=%2" );
    QUrl url( requestString );
    url.addQueryItem( "apikey", m_apiKey );
    url.addQueryItem( "track_id", track_id );
    QNetworkReply* newReply = m_nam.data()->get( QNetworkRequest( url ) );
    newReply->setProperty( "requestId", oldReply->property( "requestId" ) );
    newReply->setProperty( "requestData", oldReply->property( "requestData" ) );
    connect( newReply, SIGNAL( finished() ), SLOT( trackLyricsSlot() ) );
}

void
MusixMatchPlugin::trackLyricsSlot()
{
    qDebug() << Q_FUNC_INFO;
    QNetworkReply* reply = qobject_cast< QNetworkReply* >( sender() );
    if ( !reply )
        return; //timeout will handle it

    QDomDocument doc;
    doc.setContent( reply->readAll() );
    QDomNodeList domNodeList = doc.elementsByTagName( "lyrics_body" );
    if ( domNodeList.isEmpty() )
    {
        emit info( reply->property( "requestId" ).toUInt(), reply->property( "requestData" ).value< Tomahawk::InfoSystem::InfoRequestData >(), QVariant() );
        return;
    }
    QString lyrics = domNodeList.at(0).toElement().text();
    qDebug() << "Emitting lyrics: " << lyrics;
    emit info( reply->property( "requestId" ).toUInt(), reply->property( "requestData" ).value< Tomahawk::InfoSystem::InfoRequestData >(), QVariant( lyrics ) );
}
