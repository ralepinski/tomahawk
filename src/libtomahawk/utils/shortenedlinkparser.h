/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2010-2011, Leo Franchi <lfranchi@kde.org>
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

#ifndef SNORTENED_LINK_PARSER_H
#define SNORTENED_LINK_PARSER_H

#include "dllmacro.h"
#include "typedefs.h"

#include <QObject>
#include <QSet>
#include <QtCore/QStringList>

class QNetworkReply;
namespace Tomahawk
{

    /**
     * Small class to parse whitelisted shortened links into the redirected urls
     *
     * Connect to urls() to get the result
     *
     */
    class DLLEXPORT ShortenedLinkParser : public QObject
    {
        Q_OBJECT
    public:
        explicit ShortenedLinkParser( const QStringList& urls, QObject* parent = 0 );
        virtual ~ShortenedLinkParser();

    signals:
        void urls( const QStringList& urls );

    private:
        void lengthenUrl( const QString& url );
        void checkFinished();

        QStringList m_links;
        QSet< QNetworkReply* > m_queries;
        public slots:
         void lookupFinished();
    };

}

#endif
