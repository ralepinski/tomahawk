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

#include "contextmenu.h"

#include "globalactionmanager.h"
#include "playlistview.h"
#include "viewmanager.h"

#include "utils/logger.h"

using namespace Tomahawk;


ContextMenu::ContextMenu( QWidget* parent )
    : QMenu( parent )
{
    m_sigmap = new QSignalMapper( this );
    connect( m_sigmap, SIGNAL( mapped( int ) ), SLOT( onTriggered( int ) ) );

    m_supportedActions = ActionPlay | ActionQueue | ActionCopyLink;
}


void
ContextMenu::clear()
{
    QMenu::clear();

    m_queries.clear();
    m_albums.clear();
    m_artists.clear();
}


void
ContextMenu::setQueries( const QList<Tomahawk::query_ptr>& queries )
{
    if ( queries.isEmpty() )
        return;

    QMenu::clear();
    m_queries.clear();
    m_queries << queries;

    if ( m_supportedActions & ActionPlay && itemCount() == 1 )
        m_sigmap->setMapping( addAction( tr( "&Play" ) ), ActionPlay );

    if ( m_supportedActions & ActionQueue )
        m_sigmap->setMapping( addAction( tr( "Add to &Queue" ) ), ActionQueue );

    //m_sigmap->setMapping( addAction( tr( "&Add to Playlist" ) ), ActionAddToPlaylist );

    addSeparator();

    if ( m_supportedActions & ActionCopyLink && itemCount() == 1 )
        m_sigmap->setMapping( addAction( tr( "Copy Track &Link" ) ), ActionCopyLink );

    addSeparator();

    if ( m_supportedActions & ActionDelete )
        m_sigmap->setMapping( addAction( queries.count() > 1 ? tr( "&Delete Items" ) : tr( "&Delete Item" ) ), ActionDelete );

    foreach ( QAction* action, actions() )
    {
        connect( action, SIGNAL( triggered() ), m_sigmap, SLOT( map() ) );
    }
}


void
ContextMenu::setQuery( const Tomahawk::query_ptr& query )
{
    QList<query_ptr> queries;
    queries << query;
    setQueries( queries );
}


void
ContextMenu::setAlbums( const QList<Tomahawk::album_ptr>& albums )
{
    if ( albums.isEmpty() )
        return;

    QMenu::clear();
    m_albums.clear();
    m_albums << albums;

    if ( m_supportedActions & ActionPlay && itemCount() == 1 )
        m_sigmap->setMapping( addAction( tr( "&Play" ) ), ActionPlay );

    if ( m_supportedActions & ActionQueue )
        m_sigmap->setMapping( addAction( tr( "Add to &Queue" ) ), ActionQueue );

    //m_sigmap->setMapping( addAction( tr( "&Add to Playlist" ) ), ActionAddToPlaylist );

    addSeparator();

/*    if ( m_supportedActions & ActionCopyLink && itemCount() == 1 )
        m_sigmap->setMapping( addAction( tr( "Copy Album &Link" ) ), ActionCopyLink ); */

    foreach ( QAction* action, actions() )
    {
        connect( action, SIGNAL( triggered() ), m_sigmap, SLOT( map() ) );
    }
}


void
ContextMenu::setAlbum( const Tomahawk::album_ptr& album )
{
    QList<album_ptr> albums;
    albums << album;
    setAlbums( albums );
}


void
ContextMenu::setArtists( const QList<Tomahawk::artist_ptr>& artists )
{
    if ( artists.isEmpty() )
        return;

    QMenu::clear();
    m_artists.clear();
    m_artists << artists;

    if ( m_supportedActions & ActionPlay && itemCount() == 1 )
        m_sigmap->setMapping( addAction( tr( "&Play" ) ), ActionPlay );

    if ( m_supportedActions & ActionQueue )
        m_sigmap->setMapping( addAction( tr( "Add to &Queue" ) ), ActionQueue );

    //m_sigmap->setMapping( addAction( tr( "&Add to Playlist" ) ), ActionAddToPlaylist );

    addSeparator();

/*    if ( m_supportedActions & ActionCopyLink && itemCount() == 1 )
        m_sigmap->setMapping( addAction( tr( "Copy Artist &Link" ) ), ActionCopyLink ); */

    foreach ( QAction* action, actions() )
    {
        connect( action, SIGNAL( triggered() ), m_sigmap, SLOT( map() ) );
    }
}


void
ContextMenu::setArtist( const Tomahawk::artist_ptr& artist )
{
    QList<artist_ptr> artists;
    artists << artist;
    setArtists( artists );
}


void
ContextMenu::onTriggered( int action )
{
    switch ( action )
    {
        case ActionQueue:
            addToQueue();
            break;

        case ActionCopyLink:
            copyLink();
            break;

        default:
            emit triggered( action );
    }
}


void ContextMenu::addToQueue()
{
    foreach ( const query_ptr& query, m_queries )
    {
        ViewManager::instance()->queue()->model()->append( query );
    }
    foreach ( const artist_ptr& artist, m_artists )
    {
        ViewManager::instance()->queue()->model()->append( artist );
    }
    foreach ( const album_ptr& album, m_albums )
    {
        ViewManager::instance()->queue()->model()->append( album );
    }

    ViewManager::instance()->showQueue();
}


void
ContextMenu::copyLink()
{
    if ( m_queries.count() )
    {
        GlobalActionManager::instance()->copyToClipboard( m_queries.first() );
    }
}
