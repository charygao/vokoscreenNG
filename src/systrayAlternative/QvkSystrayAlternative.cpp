#include "QvkSystrayAlternative.h"
#include "global.h"

#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDialog>

QvkSystrayAlternative::QvkSystrayAlternative( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow, QvkSpezialSlider *gui_sliderShowInSystrayAlternative )
{
/*
    // Wenn mit Folder aufgerufen dann ohne Fensterleisteneintrag
    QDialog *dialog = new QDialog( this );
    dialog->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog );
//    dialog->setAttribute( Qt::WA_TranslucentBackground, true );
    dialog->setMinimumSize( 100, 100 );
    dialog->setMaximumSize( 100, 100 );
    dialog->resize( 100, 100 );
    dialog->show();
*/


    ui = ui_mainwindow;

    connect( gui_sliderShowInSystrayAlternative, SIGNAL( valueChanged( int ) ), this, SLOT( slot_resizeWindow( int ) ) );
    size = QSize( gui_sliderShowInSystrayAlternative->value(), gui_sliderShowInSystrayAlternative->value() );

    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setScaledContents( true );
    resize( size );
    QPixmap pixmap( ":/pictures/systray/systray.png" );
    pixmap = pixmap.scaled( size , Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    setPixmap( pixmap );

    QAction *titleAction = new QAction( this );
    titleAction->setIcon( QIcon( ":pictures/systray/systray.png" ) );
    titleAction->setText( global::name );
    titleAction->setEnabled( false );

    startAction = new QAction( this );
    startAction->setIcon( QIcon::fromTheme( "media-playback-start", style()->standardIcon( QStyle::SP_MediaPlay ) ) );
    startAction->setText( tr( "Start" ) );
    startAction->setData( "Start" );

    stopAction = new QAction( this );
    stopAction->setIcon( QIcon::fromTheme( "media-playback-stop" , style()->standardIcon( QStyle::SP_MediaStop ) ) );
    stopAction->setText( tr( "Stop" ) );
    stopAction->setData( "Stop" );
    stopAction->setEnabled( false );

    pauseAction = new QAction( this );
    pauseAction->setIcon( QIcon::fromTheme( "media-playback-pause", style()->standardIcon( QStyle::SP_MediaPause ) ) );
    pauseAction->setText( tr( "Pause" ) );
    pauseAction->setData( "Pause" );
    pauseAction->setEnabled( false );

    continueAction = new QAction( this );
    continueAction->setIcon( QIcon::fromTheme( "media-playback-start", style()->standardIcon( QStyle::SP_MediaPlay ) ) );
    continueAction->setText( tr( "Continue" ) );
    continueAction->setData( "Continue" );
    continueAction->setEnabled( false );

    exitAction = new QAction( this );
    exitAction->setIcon( QIcon::fromTheme( "application-exit", style()->standardIcon( QStyle::SP_DialogCloseButton ) ) );
    exitAction->setText( tr( "Exit" ) );
    exitAction->setData( "Exit" );

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setRecordIcon( bool ) ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), startAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setSystrayIcon( bool ) ) );

    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), continueAction, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setPauseIcon( bool ) ) );

    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setRecordIcon( bool ) ) );

    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ),  startAction, SLOT( setDisabled( bool ) ) );

    connect( startAction,    SIGNAL( triggered( bool ) ), ui->pushButtonStart,    SLOT( click() ) );
    connect( stopAction,     SIGNAL( triggered( bool ) ), ui->pushButtonStop,     SLOT( click() ) );
    connect( pauseAction,    SIGNAL( triggered( bool ) ), ui->pushButtonPause,    SLOT( click() ) );
    connect( continueAction, SIGNAL( triggered( bool ) ), ui->pushButtonContinue, SLOT( click() ) );
    connect( exitAction,     SIGNAL( triggered( bool ) ), this,                   SLOT( close() ) );
    connect( exitAction,     SIGNAL( triggered( bool ) ), mainWindow,             SLOT( close() ) );

    menu = new QMenu();
    menu->addAction( titleAction );
    menu->addSeparator();
    menu->addAction( startAction );
    menu->addAction( stopAction );
    menu->addAction( pauseAction );
    menu->addAction( continueAction );
    menu->addSeparator();
    menu->addAction( exitAction );
}


QvkSystrayAlternative::~QvkSystrayAlternative()
{
}


void QvkSystrayAlternative::slot_resizeWindow( int value )
{
    size = QSize( value, value );
    resize( value, value );

    if ( ui->pushButtonStart->isEnabled() == true )
    {
        slot_setSystrayIcon( true );
    }

    if ( ui->pushButtonStop->isEnabled() == true )
    {
        slot_setRecordIcon( true );
    }

    if ( ui->pushButtonContinue->isEnabled() == true )
    {
        slot_setPauseIcon( true );
    }
}


void QvkSystrayAlternative::mouseMoveEvent( QMouseEvent *event )
{
    move( event->globalPos() - point );
}


void QvkSystrayAlternative::mousePressEvent( QMouseEvent *event )
{
    point = event->pos();
}


void QvkSystrayAlternative::contextMenuEvent( QContextMenuEvent *event )
{
   menu->exec( event->globalPos() );
}


void QvkSystrayAlternative::slot_setRecordIcon( bool )
{
    if ( global::VK_showOnlyFallbackIcons == true )
    {
        QPixmap pixmap( ":/pictures/systray/record.png" );
        pixmap = pixmap.scaled( size , Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        setPixmap( pixmap );
    }
    else
    {
        QIcon icon = QIcon::fromTheme( "media-record", QIcon( ":/pictures/systray/record.png" ) );
        QPixmap pixmap( icon.pixmap( size ) );
        setPixmap( pixmap );
    }
}


void QvkSystrayAlternative::slot_setSystrayIcon( bool )
{
    QPixmap pixmap( ":/pictures/systray/systray.png"  );
    pixmap = pixmap.scaled( size , Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    setPixmap( pixmap );
}


void QvkSystrayAlternative::slot_setPauseIcon( bool )
{
    QIcon icon = QIcon::fromTheme( "media-playback-pause", style()->standardIcon( QStyle::SP_MediaPause ) );
    QPixmap pixmap( icon.pixmap( size ) );
    setPixmap( pixmap );
}
