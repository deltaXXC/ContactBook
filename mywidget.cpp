#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QMessageBox>

QString mFileName1 = "D:/homework/ContactBook/myfile.csv";
QString mFileName2 = "D:/homework/ContactBook/myfile.text";
void Write(QString Filename, QString str)
{
    QFile mFile(Filename);


    if (!mFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out << str;
    mFile.flush();
    mFile.close();
}

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    QStringList ColTotle;

    ui->tableWidget->setColumnCount(4);

    ColTotle<<QStringLiteral("學號")<<QStringLiteral("班級")<<QStringLiteral("姓名")<<QStringLiteral("電話");
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;

    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit_1->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);
}


void MyWidget::on_pushButton_2_clicked()
{
    QString saveFile = "";

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            saveFile += ui->tableWidget->item(i, j)->text() + ",";
        }

        saveFile += "\n";
    }

    Write(mFileName1, saveFile);
    Write(mFileName2, saveFile);

}


void MyWidget::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "選擇 CSV 檔案",
        "",
        "CSV Files (*.csv)"
        );


    if (filename.isEmpty())
        return;


    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "錯誤", "無法開啟檔案");
        return;
    }

    QTextStream in(&file);


    ui->tableWidget->setRowCount(0);


    while (!in.atEnd())
    {
        QString line = in.readLine();


        if (line.trimmed().isEmpty())
            continue;


        QStringList data = line.split(",");


        if (!data.isEmpty() && data.last().isEmpty())
            data.removeLast();


        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);


        for (int col = 0;
             col < data.size() &&
             col < ui->tableWidget->columnCount();
             col++)
        {
            ui->tableWidget->setItem(
                row,
                col,
                new QTableWidgetItem(data[col])
                );
        }
    }


    file.close();


    QMessageBox::information(
        this,
        "匯入成功",
        "CSV 資料已成功匯入"
        );
}

