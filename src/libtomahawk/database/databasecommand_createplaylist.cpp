#include "databasecommand_createplaylist.h"

#include <QSqlQuery>

#include "network/servent.h"

using namespace Tomahawk;


DatabaseCommand_CreatePlaylist::DatabaseCommand_CreatePlaylist( QObject* parent )
    : DatabaseCommandLoggable( parent )
    , m_report( true )
{
    qDebug() << Q_FUNC_INFO << "def";
}


DatabaseCommand_CreatePlaylist::DatabaseCommand_CreatePlaylist( const source_ptr& author,
                                                                const playlist_ptr& playlist )
    : DatabaseCommandLoggable( author )
    , m_playlist( playlist )
    , m_report( false ) //this ctor used when creating locally, reporting done elsewhere
{
    qDebug() << Q_FUNC_INFO;
}


void
DatabaseCommand_CreatePlaylist::exec( DatabaseImpl* lib )
{
    qDebug() << Q_FUNC_INFO;
    Q_ASSERT( !m_playlist.isNull() );
    Q_ASSERT( !source().isNull() );

    TomahawkSqlQuery cre = lib->newquery();
    cre.prepare( "INSERT INTO playlist( guid, source, shared, title, info, creator, lastmodified) "
                 "VALUES( :guid, :source, :shared, :title, :info, :creator, :lastmodified )" );
    cre.bindValue( ":guid", m_playlist->guid() );
    cre.bindValue( ":source", source()->isLocal() ? QVariant(QVariant::Int) : source()->id() );
    cre.bindValue( ":shared", m_playlist->shared() );
    cre.bindValue( ":title", m_playlist->title() );
    cre.bindValue( ":info", m_playlist->info() );
    cre.bindValue( ":creator", m_playlist->creator() );
    cre.bindValue( ":lastmodified", m_playlist->lastmodified() );

    qDebug() << "CREATE PLAYLIST:" << cre.boundValues();

    cre.exec();
}


void
DatabaseCommand_CreatePlaylist::postCommitHook()
{
    qDebug() << Q_FUNC_INFO;
    if( m_report == false )
        return;

    qDebug() << Q_FUNC_INFO << "..reporting..";
    m_playlist->reportCreated( m_playlist );

    if( source()->isLocal() )
        Servent::instance()->triggerDBSync();
}