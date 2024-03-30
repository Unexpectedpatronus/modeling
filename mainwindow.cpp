#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>

#include <QtCharts>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::generateGraph);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::distributionChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::distributionChanged(int index)
{
    if (index == 0) { // Normal Distribution
        // Show mean and stddev input widgets
        ui->meanLabel->show();
        ui->meanLineEdit->show();
        ui->stddevLabel->show();
        ui->stddevLineEdit->show();
    } else if (index == 1) { // Uniform Distribution
        // Hide mean and stddev input widgets
        ui->meanLabel->hide();
        ui->meanLineEdit->hide();
        ui->stddevLabel->hide();
        ui->stddevLineEdit->hide();
    }
}

void MainWindow::generateGraph() {
    double mean = ui->meanLineEdit->text().toDouble();
    double stddev = ui->stddevLineEdit->text().toDouble();
    double a = ui->LeftBoundary->text().toDouble();
    double b = ui->RightBoundary->text().toDouble();

    const int nrolls = 10000;  // number of experiments

    std::default_random_engine generator;
    std::normal_distribution<double> normalDistribution(mean, stddev);
    std::uniform_real_distribution<double> uniformDistribution(a, b);

    // Prepare data series
    QLineSeries *series = new QLineSeries();
    if (ui->comboBox->currentText() == "Normal Distribution") {
        for (int i = a; i < b; ++i) {
            int count = 0;
            for (int j = 0; j < nrolls; ++j) {
                double number = normalDistribution(generator);
                if (number >= i && number < (i + 1))
                    ++count;
            }
            series->append(i, count);
        }
    } else if (ui->comboBox->currentText() == "Uniform Distribution") {
        for (int i = a; i < b; ++i) {
            int count = 0;
            for (int j = 0; j < nrolls; ++j) {
                double number = uniformDistribution(generator);
                if (number >= i && number < (i + 1))
                    ++count;
            }
            series->append(i, count);
        }
    }

    // Create chart and add series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(ui->comboBox->currentText());
    chart->createDefaultAxes();
    chart->axisX()->setTitleText("Распределение случайной величины Х");
    chart->axisY()->setTitleText("f(x)");

    // Create chart view and set the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set graphicsView to display the chart
    ui->graphicsView->setChart(chart);
}
