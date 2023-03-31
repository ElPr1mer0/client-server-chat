#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    client = new CLIENT(ui->textBrowser, ui->lineEdit);
}

MainWindow::~MainWindow(){
    delete ui;
    delete client;
}


void MainWindow::on_pushButton_clicked(){
    QString ip = ui->ld_ip->text();
    client->connectToHost(ip, 2282);
}


void MainWindow::on_pushButton_2_clicked(){
    client->SendToServer(ui->lineEdit->text());
}


void MainWindow::on_lineEdit_returnPressed(){
    client->SendToServer(ui->lineEdit->text());
}

