#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_model(SINGLE_MODEL),
    m_command("lua tbd.lua ice pattern script"),
    m_curDir("E:")
{
    ui->setupUi(this);

    m_patternList.clear();
    ui->lineEditCommand->setText("lua tbd.lua ice pattern script");
    ui->lineEditCurDir->setText(m_curDir);

    connect(ui->buttonSetCurDir,SIGNAL(clicked()),this,SLOT(setCurDir()));
    connect(ui->buttonAddPattern,SIGNAL(clicked()),this,SLOT(addPatternList()));
    connect(ui->buttonClearPattern,SIGNAL(clicked()),this,SLOT(clearPatternList()));
    connect(ui->buttonRun,SIGNAL(clicked()),this,SLOT(runPatterns()));
    connect(ui->buttonModel,SIGNAL(clicked()),this,SLOT(changeModel()));

    connect(&m_process,SIGNAL(readyRead()),this,SLOT(readAllOutput()));
}

MainWindow::~MainWindow()
{
    delete ui;

    m_patternList.clear();

}

void MainWindow::setCurDir(void)
{
    m_curDir = QFileDialog::getExistingDirectory(this,
                                                 "Set current directory",
                                                 "C:\\Program Files (x86)\\Lua\\5.1\\examples\\");
    ui->lineEditCurDir->setText(m_curDir);
}

void MainWindow::addPatternList(void)
{
    if (m_curDir.isEmpty())
    {
        qDebug() << "VM Path is empty!";
        return ;
    }

    QList<QString> filesList = QFileDialog::getOpenFileNames(this,
                                                             "Choose patterns",
                                                             m_curDir,
                                                             "Pattern (*.lua)");
    for (int i=0;i<filesList.count();++i)
    {
        m_patternList.append(filesList[i]);
    }

    updatePatternListShow(filesList);
}

void MainWindow::clearPatternList(void)
{
    m_patternList.clear();
    ui->listWidgePatternList->clear();
}

void MainWindow::updatePatternListShow(QList<QString>& patternList)
{
    for (int i=0;i<patternList.count();++i)
    {
        ui->listWidgePatternList->insertItem(i,patternList[i]);
    }
}

void MainWindow::runPatterns(void)
{
    if (m_model == SINGLE_MODEL)
        runInSingleModel();
//        qDebug() << "SINGLE";
    else
        runInMultiModel();
//        qDebug() << "MULTI";
}

void MainWindow::runInSingleModel(void)
{
    if (m_patternList.isEmpty())
        return ;

    runOnePattern(m_patternList[0]);
    m_patternList.removeFirst();

    QListWidgetItem *item = ui->listWidgePatternList->takeItem(0);
    ui->listWidgePatternList->removeItemWidget(item);

}

void MainWindow::runInMultiModel(void)
{
    if (m_patternList.isEmpty())
        return ;

    qDebug() << "runInMultiModel";
    runMultiPattern(m_patternList);
}

void MainWindow::runOnePattern(QString &pattern)
{
    qDebug() << pattern;

    if (m_curDir.isEmpty())
    {
        qDebug() << "VM Path is empty!";
        return;
    }

    m_process.setWorkingDirectory(m_curDir);
    QString program = "lua";
    QStringList arguments;
    arguments << pattern;

    m_process.start(program, arguments);
    m_process.waitForStarted(10000);

    m_process.waitForReadyRead(10000);

}

void MainWindow::runMultiPattern(QList<QString> &patternList)
{
    for (int i=0;i<patternList.count();++i)
    {
        runOnePattern(patternList[i]);
    }
}

void MainWindow::changeModel(void)
{
    qDebug() << "Change model";
    if (ui->buttonModel->isChecked())
        m_model = SINGLE_MODEL;
    else
        m_model = MULTI_MODEL;
}

void MainWindow::readAllOutput(void)
{
    m_output += m_process.readAll();
    ui->textOutput->setText(m_output);
}
