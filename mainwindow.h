#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>

#include <QTimer>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow()=default;
private:
    std::map<int,QString> keyPadMap;
    std::string equation;
    QWidget *centralwidget;
    QPushButton* pushButtons[35];
    QTextEdit* pastCalculationTextEdit;
    QLineEdit* currentCalculationLineEdit;


};

#endif
