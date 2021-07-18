#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , textFromFile(" ")
    , running(false)
    , currentSentence(NULL)
    , temp(" ")
    , number(0)
    , mistakesCount(0)

{
    ui->setupUi(this);
    timer = new QElapsedTimer();
    timerUpdate = new QTimer(this);
    connect(timerUpdate,SIGNAL(timeout()),this,SLOT(updateTime()));
    timerUpdate->setInterval(100);
    getTextFromFile();

    currentSentence = textFromFile.split('.');
    getSettingsFromFile();
}

MainWindow::~MainWindow()
{
    delete ui;

    timerUpdate->stop();
    delete timerUpdate;
    timerUpdate = nullptr;

    delete timer;
    timer = nullptr;

    writeSettings();
}

void MainWindow::getTextFromFile()
{
    QString filename=":/new/prefix1/typingFile.txt";
    QFile file(filename);
    if(!file.exists())
    {
        qDebug() << "file doesnt exist "<<file.fileName();
    }
    else
    {
        qDebug() << file.fileName()<<" exist...";
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            textFromFile += in.readLine();
        }
        qDebug() << textFromFile;
    }
}
void MainWindow::displaySentencesOneByOne()
{
    ui->label_text->setText(currentSentence[number]);
    temp = currentSentence.at(number);
    //because file can contain first element empty
    if(temp.at(0)==' ')
    {
        temp.remove(0,1);
    }
    number++;
}

void MainWindow::getSettingsFromFile()
{
    QSettings settings("Typer", "MyApp");
    settings.beginGroup("MainWindow");
    QRect myRect = settings.value("position").toRect();
    setGeometry(myRect);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("Typer", "MyApp");
    settings.beginGroup("MainWindow");
    settings.setValue("position",this->geometry());
    settings.endGroup();
}

bool MainWindow::checkStrings(QString arg)
{
    for(int i = 0;i < arg.size();i++)
    {
        if(arg[i]!=temp[i])
            return false;
    }
    return true;
}
void MainWindow::on_pushButton_STOP_clicked()
{
    timerUpdate->stop();
    running = false;
    ui->lineEdit_typing->setEnabled(false);
    //currentSentence = textFromFile.split('.');
    number = 0;
    ui->lineEdit_typing->setText(" ");
}
void MainWindow::on_pushButton_START_clicked()
{
    number = 0;
    displaySentencesOneByOne();
    mistakesCount = 0;
    timer->start();
    timerUpdate->start();
    running = true;
    ui->lineEdit_typing->setEnabled(true);
}

void MainWindow::updateTime()
{
    QString formattedTime = QTime::fromMSecsSinceStartOfDay(timer->elapsed()).toString("hh:mm:ss");
    ui->label_time->setText(formattedTime);


}
void MainWindow::on_lineEdit_typing_textChanged(const QString &arg1)
{
    //check for mistakes
    if(checkStrings(arg1))
    {
        ui->label_text->setStyleSheet("QLabel { color: black }");
    }
    //check if running
    if(running){

        //check for 0 elements to not to check minus(0-1) index
        if(arg1.length()==0)
        {
            return;
        }
        //check
        if(arg1.back()!=temp[arg1.length()-1])
        {
            if(temp[arg1.length()-1]!='\x0')//end of temp
            {
                mistakesCount+=1;
                ui->label_text->setStyleSheet("QLabel { color: red }");

            }
            ui->label_mistakes->setText("Mistakes:" + QString::number(mistakesCount));
        }

        //end of each sentence
        if(arg1.length()==temp.length() && arg1==temp)
        {
            displaySentencesOneByOne();
            ui->lineEdit_typing->setText("");//settings empty
        }
        //if next str is empty than there is no more to do so end everything
        if(temp=="")
        {

            number = 0;
            timerUpdate->stop();
            running = false;
            ui->lineEdit_typing->setEnabled(false);
        }

    }
}


//void MainWindow::keyPressEvent(QKeyEvent *ev)
//{

//    switch(ev->key())
//    {
//    case Qt::Key_Backspace:
//        qDebug()<<BackspacePressed;
//        BackspacePressed = true;
//        break;

//    }

//    //    if(ev->key()==Qt::Key_Backspace)
//    //    {


//    //    }
//}

//void MainWindow::keyReleaseEvent(QKeyEvent *ev)
//{
//    switch(ev->key())
//    {
//    case Qt::Key_Backspace:
//        qDebug()<<BackspacePressed;
//        BackspacePressed = false;
//        break;

//    }
//}
