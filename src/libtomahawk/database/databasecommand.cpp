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

#include "databasecommand.h"

#include "databasecommand_addfiles.h"
#include "databasecommand_createplaylist.h"
#include "databasecommand_deletefiles.h"
#include "databasecommand_deleteplaylist.h"
#include "databasecommand_logplayback.h"
#include "databasecommand_renameplaylist.h"
#include "databasecommand_setplaylistrevision.h"
#include "databasecommand_createdynamicplaylist.h"
#include "databasecommand_deletedynamicplaylist.h"
#include "databasecommand_setdynamicplaylistrevision.h"
#include "databasecommand_socialaction.h"

#include "utils/logger.h"


DatabaseCommand::DatabaseCommand( QObject* parent )
    : QObject( parent )
    , m_state( PENDING )
{
    //qDebug() << Q_FUNC_INFO;
}


DatabaseCommand::DatabaseCommand( const source_ptr& src, QObject* parent )
    : QObject( parent )
    , m_state( PENDING )
    , m_source( src )
{
    //qDebug() << Q_FUNC_INFO;
}


DatabaseCommand::~DatabaseCommand()
{
//    qDebug() << Q_FUNC_INFO;
}


void
DatabaseCommand::_exec( DatabaseImpl* lib )
{
    //qDebug() << "RUNNING" << thread();
    m_state = RUNNING;
    emit running();
    exec( lib );
    m_state = FINISHED;
    //qDebug() << "FINISHED" << thread();
}


DatabaseCommand*
DatabaseCommand::factory( const QVariant& op, const source_ptr& source )
{
    const QString name = op.toMap().value( "command" ).toString();

    if( name == "addfiles" )
    {
        DatabaseCommand_AddFiles * cmd = new DatabaseCommand_AddFiles;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "deletefiles" )
    {
        DatabaseCommand_DeleteFiles * cmd = new DatabaseCommand_DeleteFiles;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "createplaylist" )
    {
        DatabaseCommand_CreatePlaylist * cmd = new DatabaseCommand_CreatePlaylist;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "deleteplaylist" )
    {
        DatabaseCommand_DeletePlaylist * cmd = new DatabaseCommand_DeletePlaylist;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "logplayback" )
    {
        DatabaseCommand_LogPlayback * cmd = new DatabaseCommand_LogPlayback;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "renameplaylist" )
    {
        DatabaseCommand_RenamePlaylist * cmd = new DatabaseCommand_RenamePlaylist;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "setplaylistrevision" )
    {
        DatabaseCommand_SetPlaylistRevision * cmd = new DatabaseCommand_SetPlaylistRevision;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    } else if( name == "createdynamicplaylist" )
    {
        DatabaseCommand_CreateDynamicPlaylist * cmd = new DatabaseCommand_CreateDynamicPlaylist;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "deletedynamicplaylist" )
    {
        DatabaseCommand_DeleteDynamicPlaylist * cmd = new DatabaseCommand_DeleteDynamicPlaylist;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "setdynamicplaylistrevision" )
    {
        qDebug() << "SETDYN CONTENT:" << op;
        DatabaseCommand_SetDynamicPlaylistRevision * cmd = new DatabaseCommand_SetDynamicPlaylistRevision;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }
    else if( name == "socialaction" )
    {
        DatabaseCommand_SocialAction * cmd = new DatabaseCommand_SocialAction;
        cmd->setSource( source );
        QJson::QObjectHelper::qvariant2qobject( op.toMap(), cmd );
        return cmd;
    }

    qDebug() << "ERROR in" << Q_FUNC_INFO << name;
//    Q_ASSERT( false );
    return NULL;
}
