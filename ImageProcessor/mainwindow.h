#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_actionMaximize_triggered();

    void on_actionClose_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_splitter_2_splitterMoved(int pos, int index);

private:
    Ui::MainWindow *ui;
    QPoint m_dragPosition;
};

#endif // MAINWINDOW_H
