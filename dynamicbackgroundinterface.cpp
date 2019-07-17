#include "dynamicbackgroundinterface.h"

DynamicBackgroundInterface::DynamicBackgroundInterface(QWidget *parent) : QObject(parent), widget(parent), show_ani_progress(0)
{
    timer = new QTimer(this);
    timer->setInterval(interval = REFRESH_INTERVAL); // 刷新周期
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    // 更新随机数
    QTime time = QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
}


void DynamicBackgroundInterface::setInterval(int iv)
{
    timer->setInterval(interval = iv);
}

void DynamicBackgroundInterface::showAni()
{
    QTimer* ani = new QTimer(this);
    ani->setInterval(10);
    ani->start();
    show_ani_progress = 1;
    connect(ani, &QTimer::timeout, [=]{
        show_ani_progress++;
        if (show_ani_progress > 100)
        {
            show_ani_progress = 0;
            ani->stop();
            ani->deleteLater();
        }
        redraw();
    });
}

void DynamicBackgroundInterface::accumulate(int x)
{
    accumulation += x;
}

void DynamicBackgroundInterface::timeout()
{
    redraw();
}

QRect DynamicBackgroundInterface::getGeometry()
{
    return QRect(-1, -1, widget->size().width()+1, widget->size().height()+1);
}

int DynamicBackgroundInterface::randRange(int min, int max)
{
    if (min > max)
    {
        min = min ^ max;
        max = min ^ max;
        min = min ^ max;
    }
    int res = qrand() % (max-min+1) + min;

    return res;
}

bool DynamicBackgroundInterface::randBool()
{
    return qrand() % 2;
}

int DynamicBackgroundInterface::intToUnity(int v)
{
    if (v == 0)
        return 0;
    if (v > 0)
        return 1;
    return -1;
}

void DynamicBackgroundInterface::colorToArray(int *arr, QColor c)
{
    arr[R] = c.red();
    arr[G] = c.green();
    arr[B] = c.blue();
    arr[A] = c.alpha();
}

void DynamicBackgroundInterface::redraw()
{
    emit signalRedraw();
}

