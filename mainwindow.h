#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QProcess>

#define SINGLE_MODEL 1
#define MULTI_MODEL 2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updatePatternListShow(QList<QString>& patternList);
    void runInSingleModel(void);
    void runInMultiModel(void);
    void runOnePattern(QString &pattern);
    void runMultiPattern(QList<QString> &patternList);


public slots:
    void setCurDir(void);
    void addPatternList(void);
    void clearPatternList(void);
    void runPatterns(void);
    void changeModel(void);

    void readAllOutput(void);
private:
    Ui::MainWindow *ui;
    QString m_curDir;
    QString m_command;
    QList<QString> m_patternList;
    int m_model;
    QProcess m_process;
    QString m_output;
};

#endif // MAINWINDOW_H
