#ifndef QVKPLAYER_H
#define QVKPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QKeyEvent>

namespace Ui {
class QvkPlayer;
}

class QvkPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit QvkPlayer(QWidget *parent = 0);
    ~QvkPlayer();
    void setMediaFile( QString string );


public slots:
    void slot_play();
    void slot_mute();
    void slot_durationChanged( qint64 value );
    void slot_sliderMoved( int value );
    void slot_positionChanged( qint64 value );

    void slot_stateChanged( QMediaPlayer::State );
    void slot_mutedChanged( bool muted );


private slots:
    void slot_setNewImage( QImage image );


private:
    Ui::QvkPlayer *ui;
    QMediaPlayer *mediaPlayer;


protected:
    void mouseDoubleClickEvent( QMouseEvent *event );
    void closeEvent(QCloseEvent *event);
    void keyPressEvent( QKeyEvent *event );


};

#endif // QVKPLAYER_H