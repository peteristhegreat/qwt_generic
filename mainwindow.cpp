#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qwt_legend_label.h>

// https://github.com/VladimirSap/Qwt_Tutorial

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addPlot();

    addPlotGrid();

    addCurve();

    enableMagnifier();

    enableMovingOnPlot();

    enablePicker();

    showCurve(curve, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPlot()
{

    ui->plot->setTitle( "Qwt demonstration" );
    ui->plot->setCanvasBackground( Qt::white );


    ui->plot->setAxisTitle(QwtPlot::yLeft, "Y");
    ui->plot->setAxisTitle(QwtPlot::xBottom, "X");

    QwtLegend * legend = new QwtLegend();
    legend->setDefaultItemMode(QwtLegendData::Checkable);
    ui->plot->insertLegend( legend );

    QObject::connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
             this, SLOT( legendChecked( const QVariant &, bool ) ) );
}

//void MainWindow::legendChecked( const QVariant &, bool )
//{
//    qDebug() << Q_FUNC_INFO;
//    QwtPlotItem* plotItem = ui->plot->infoToItem( itemInfo );
////    if ( plotItem->yAxis() == QwtPlot::yRight )
////        ui->plot->legend()->updateLegend( itemInfo, data );
//    plotItem->setVisible(!plotItem->isVisible());
////    else
////        mLegendLeft->updateLegend( itemInfo, data );
//}

void MainWindow::legendChecked( const QVariant &itemInfo, bool on )
{
    QwtPlotItem *plotItem = ui->plot->infoToItem( itemInfo );
    if ( plotItem )
        showCurve( plotItem, on );
}

void MainWindow::showCurve( QwtPlotItem *item, bool on )
{
    item->setVisible( on );

    QwtLegend *lgd = qobject_cast<QwtLegend *>( ui->plot->legend() );

    QList<QWidget *> legendWidgets =
        lgd->legendWidgets( ui->plot->itemToInfo( item ) );

    if ( legendWidgets.size() == 1 )
    {
        QwtLegendLabel *legendLabel =
            qobject_cast<QwtLegendLabel *>( legendWidgets[0] );

        if ( legendLabel )
            legendLabel->setChecked( on );
    }

    ui->plot->replot();
}

void MainWindow::addPlotGrid()
{

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QPen( Qt::gray, 2 ));
    grid->attach( ui->plot );
}

void MainWindow::addCurve()
{
    //#include <qwt_plot_curve.h>
    curve = new QwtPlotCurve();
    curve->setTitle( "Demo Curve" );
    curve->setPen( Qt::blue, 6 );



    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
    curve->setSymbol( symbol );




    for (int i = 0; i < 5; i++) {
        points << QPointF( 1.0 * i, 1.0 * i);
    }

    curve->setSamples( points );

    curve->attach( ui->plot );
}

void MainWindow::enableMagnifier()
{

    QwtPlotMagnifier *magnifier =
            new QwtPlotMagnifier(ui->plot->canvas());

    magnifier->setMouseButton(Qt::MidButton);
}

void MainWindow::enableMovingOnPlot()
{

    QwtPlotPanner *d_panner = new QwtPlotPanner( ui->plot->canvas() );

    d_panner->setMouseButton( Qt::RightButton );
}

void MainWindow::enablePicker()
{


    QwtPlotPicker *d_picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
    QwtPlotPicker::NoRubberBand,
    QwtPicker::AlwaysOff,
    ui->plot->canvas() );


    d_picker->setRubberBandPen( QColor( Qt::red ) );


    d_picker->setTrackerPen( QColor( Qt::black ) );

    d_picker->setStateMachine( new QwtPickerDragPointMachine() );

    connect( d_picker, SIGNAL( appended( const QPoint & ) ),
            SLOT( click_on_canvas( const QPoint & ) ) );
}

void MainWindow::click_on_canvas( const QPoint &pos )
{

    double x = ui->plot->invTransform(QwtPlot::xBottom, pos.x());
    double y = ui->plot->invTransform(QwtPlot::yLeft, pos.y());

    statusBar()->showMessage("x= " + QString::number(x) +
                             "; y = " + QString::number(y));

    double dist;
    int index = curve->closestPoint(pos, &dist);
    if(index >= 0)
    {
        qDebug() << curve->sample(index) << dist;
    }
}


void MainWindow::on_moveByXButton_clicked()
{
    double x = 0.0;


    QString spinBoxText = ui->changeXSpinBox->text().replace(
                QLocale().decimalPoint(), QLatin1Char('.'));

    double xChangeValue = spinBoxText.toDouble();

    for (int i = 0; i <points.size(); i++) {
        x = points[i].x();
        points[i].setX(x + xChangeValue);
    }

    curve->setSamples(points);
    ui->plot->replot();
}
