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

#ifndef AUDIOCONTROLS_H
#define AUDIOCONTROLS_H

#include <QWidget>

#include "result.h"
#include "playlistinterface.h"
#include "infosystem/infosystem.h"

class QDropEvent;
class QDragEnterEvent;
class QDragMoveEvent;
namespace Ui
{
    class AudioControls;
}

class AudioControls : public QWidget
{
Q_OBJECT

public:
    AudioControls( QWidget* parent = 0 );
    ~AudioControls();

signals:
    void playPressed();
    void pausePressed();

public slots:
    void onRepeatModeChanged( Tomahawk::PlaylistInterface::RepeatMode mode );
    void onShuffleModeChanged( bool enabled );

protected:
    void changeEvent( QEvent* e );
    void dragEnterEvent ( QDragEnterEvent* );
    void dragMoveEvent ( QDragMoveEvent* );
    void dropEvent ( QDropEvent* );

private slots:
    void onPlaybackStarted( const Tomahawk::result_ptr& result );
    void onPlaybackLoading( const Tomahawk::result_ptr& result );
    void onPlaybackPaused();
    void onPlaybackResumed();
    void onPlaybackStopped();

    void onPlaybackTimer( qint64 msElapsed );
    void onVolumeChanged( int volume );

    void onRepeatClicked();
    void onShuffleClicked();

    void onArtistClicked();
    void onAlbumClicked();
    void onTrackClicked();
    void onLoveButtonClicked( bool );

    void infoSystemInfo( Tomahawk::InfoSystem::InfoRequestData requestData, QVariant output );
    void infoSystemFinished( QString target );

    void droppedTracks( QList<Tomahawk::query_ptr> );

    void socialActionsLoaded();
private:
    Ui::AudioControls *ui;

    QPixmap m_defaultCover;

    Tomahawk::result_ptr m_currentTrack;
    Tomahawk::PlaylistInterface::RepeatMode m_repeatMode;
    bool m_shuffled;
};

#endif // AUDIOCONTROLS_H
