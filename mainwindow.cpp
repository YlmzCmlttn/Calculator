#include "mainwindow.h"
#include <iostream>
#include "ShuntingYard.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{ 
    
    this->resize(10, 10);
    centralwidget = new QWidget(this);
    this->setCentralWidget(centralwidget);
    QVBoxLayout* centralWidget_VerticalLayout = new QVBoxLayout(centralwidget);
    QGroupBox* calculatorGroupBox = new QGroupBox("Calculator",centralwidget);
    centralWidget_VerticalLayout->addWidget(calculatorGroupBox);

    QGridLayout* Calculator_GB_GL = new QGridLayout(calculatorGroupBox);

    keyPadMap={
        {0 ,"Rad"},{1 ,"Deg"},{2 ,"x!" },{3 ,"("},{4 ,")"},{5 ,"%"},{6 ,"<"},
        {7 ,"Inv"},{8 ,"sin"},{9 ,"ln" },{10,"7"},{11,"8"},{12,"9"},{13,"/"},
        {14,"π"  },{15,"cos"},{16,"log"},{17,"4"},{18,"5"},{19,"6"},{20,"*"},
        {21,"e"  },{22,"tan"},{23,"√"  },{24,"1"},{25,"2"},{26,"3"},{27,"-"},
        {28,"ANS"},{29,"EXP"},{30,"^"  },{31,"0"},{32,"."},{33,"="},{34,"+"},
        
    };

    pastCalculationTextEdit = new QTextEdit(calculatorGroupBox);
    Calculator_GB_GL->addWidget(pastCalculationTextEdit,0,0,1,7);
    currentCalculationLineEdit = new QLineEdit(calculatorGroupBox);
    Calculator_GB_GL->addWidget(currentCalculationLineEdit,1,0,1,7);
    currentCalculationLineEdit->setMinimumHeight(45);
    currentCalculationLineEdit->setFont(QFont("Courier New", 20));
    currentCalculationLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    pastCalculationTextEdit->setAlignment(Qt::AlignRight);
    
    for (int i = 0; i < (35); i++)
    {
        pushButtons[i] = new QPushButton(keyPadMap[i],calculatorGroupBox);
        Calculator_GB_GL->addWidget(pushButtons[i],(i/7)+2,i%7,1,1);
        pushButtons[i]->setMinimumHeight(40);
        pushButtons[i]->setMinimumWidth(90);
    }
    connect(pushButtons[6],&QPushButton::clicked,[=](){
        currentCalculationLineEdit->setText(QString::fromStdString(currentCalculationLineEdit->text().toStdString().substr(0,currentCalculationLineEdit->text().toStdString().size()-1)));
    });
    connect(pushButtons[33],&QPushButton::clicked,[=](){
        std::string eqn = currentCalculationLineEdit->text().toStdString();
        int e_index =0;
        while(true){
            e_index = eqn.find("e+",e_index);
            if(e_index == std::string::npos)break;

            eqn.replace(e_index,2,"*10^");
        }
        e_index =0;
        while(true){
            e_index = eqn.find("e-",e_index);
            if(e_index == std::string::npos)break;

            eqn.replace(e_index,2,"*10^-");
        }
        ReversePolishNotation::RPN rpn = ReversePolishNotation::reversePolishNotation(eqn);
        std::shared_ptr<ShuntingYard::Node> tree = ShuntingYard::parse(rpn);
        //std::cout << "= " << ShuntingYard::calculate(tree) << std::endl;

        
        double ret = ShuntingYard::calculate(tree);
        
        pastCalculationTextEdit->append(currentCalculationLineEdit->text()+"="+QString::number(ret));
        currentCalculationLineEdit->setText(QString::number(ret));

    });
    connect(pushButtons[2],&QPushButton::clicked,[=](){
        currentCalculationLineEdit->insert("!");
    });
    connect(pushButtons[14],&QPushButton::clicked,[=](){
        currentCalculationLineEdit->insert("π");
    });
    connect(pushButtons[21],&QPushButton::clicked,[=](){
        currentCalculationLineEdit->insert("exp(1)");
    });
    
    for (int i=1;i<4;i++){
        for(int j=0;j<3;j++){
            connect(pushButtons[(31-(i*7)+j)],&QPushButton::clicked,[=](){
                currentCalculationLineEdit->insert(QString::number(((i-1)*3+1)+j));
            });
        }
    }
    for(int i=3;i<6;i++){
        connect(pushButtons[i],&QPushButton::clicked,[=](){
            currentCalculationLineEdit->insert(keyPadMap[i]);
        });
    }
    for (int i=30;i<33;i++){
        connect(pushButtons[i],&QPushButton::clicked,[=](){
            currentCalculationLineEdit->insert(keyPadMap[i]);
        });
    }
    for(int i=13;i<35;i=i+7){
        connect(pushButtons[i],&QPushButton::clicked,[=](){
            currentCalculationLineEdit->insert(keyPadMap[i]);
        });
    }
    for(int i=8;i<23;i=i+7){
        connect(pushButtons[i],&QPushButton::clicked,[=](){
            currentCalculationLineEdit->insert(keyPadMap[i]);
            currentCalculationLineEdit->insert("(");
        });
        connect(pushButtons[i+1],&QPushButton::clicked,[=](){
            currentCalculationLineEdit->insert(keyPadMap[i+1]);
            currentCalculationLineEdit->insert("(");
        });
    }








    




    

    
}
