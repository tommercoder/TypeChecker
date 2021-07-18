#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QSettings>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getTextFromFile();
    void displaySentencesOneByOne();
    void getSettingsFromFile();
    void writeSettings();
    bool checkStrings(QString arg);
    ///void keyPressEvent(QKeyEvent *ev);
    ///void keyReleaseEvent(QKeyEvent *ev);
private slots:
    void on_pushButton_STOP_clicked();

    void on_pushButton_START_clicked();
    void updateTime();
    void on_lineEdit_typing_textChanged(const QString &arg1);




private:
    Ui::MainWindow *ui;
    QElapsedTimer *timer;
    QTimer *timerUpdate;
    QString textFromFile;
    bool running;

    QStringList currentSentence;
    QString temp;
    int number;
    int mistakesCount;

};
#endif // MAINWINDOW_H
