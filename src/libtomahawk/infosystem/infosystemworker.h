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

#ifndef TOMAHAWK_INFOSYSTEMWORKER_H
#define TOMAHAWK_INFOSYSTEMWORKER_H

#include "infosystem/infosystem.h"

#include <QNetworkAccessManager>
#include <QtCore/QObject>
#include <QtCore/QtDebug>
#include <QtCore/QMap>
#include <QtCore/QWeakPointer>
#include <QtCore/QSet>
#include <QtCore/QLinkedList>
#include <QtCore/QVariant>
#include <QtCore/QTimer>

#include "dllmacro.h"

namespace Tomahawk {

namespace InfoSystem {

   
class DLLEXPORT InfoSystemWorker : public QObject
{
    Q_OBJECT

public:
    InfoSystemWorker();
    ~InfoSystemWorker();

    void registerInfoTypes( const InfoPluginPtr &plugin, const QSet< InfoType > &getTypes, const QSet< InfoType > &pushTypes );
    QNetworkAccessManager* nam() const;
    
signals:
    void info( Tomahawk::InfoSystem::InfoRequestData requestData, QVariant output );
    void finished( QString target );
    
    void namChanged( QNetworkAccessManager* );

public slots:
    void init( QWeakPointer< Tomahawk::InfoSystem::InfoSystemCache > cache );
    void getInfo( Tomahawk::InfoSystem::InfoRequestData requestData, uint timeoutMillis );
    void pushInfo( QString caller, Tomahawk::InfoSystem::InfoType type, QVariant input );

    void infoSlot( uint requestId, Tomahawk::InfoSystem::InfoRequestData requestData, QVariant output );
    
    void newNam();

private slots:
    void checkTimeoutsTimerFired();
    
private:

    void checkFinished( const QString &target );
    
    QHash< QString, QHash< InfoType, int > > m_dataTracker;
    QMultiMap< qint64, uint > m_timeRequestMapper;
    QHash< uint, bool > m_requestSatisfiedMap;
    QHash< uint, InfoRequestData* > m_savedRequestMap;
    
    QLinkedList< InfoPluginPtr > determineOrderedMatches( const InfoType type ) const;
    
    // For now, statically instantiate plugins; this is just somewhere to keep them
    QLinkedList< InfoPluginPtr > m_plugins;

    QMap< InfoType, QLinkedList< InfoPluginPtr > > m_infoGetMap;
    QMap< InfoType, QLinkedList< InfoPluginPtr > > m_infoPushMap;

    QWeakPointer< QNetworkAccessManager> m_nam;

    uint m_nextRequest;

    QTimer m_checkTimeoutsTimer;
};

}

}

#endif // TOMAHAWK_INFOSYSTEMWORKER_H
