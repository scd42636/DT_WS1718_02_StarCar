#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QHBoxLayout>
#include <QThread>
#include <QMainWindow>

class StartWidget : public QWidget
{
    Q_OBJECT

public:

    explicit StartWidget(QWidget *parent = nullptr, QMainWindow *mainwindow = nullptr);

    ~StartWidget();

private:

    // Label
    QLabel          *lblHeadline;

    // ProgressBar
    QProgressBar    *progressBar;

    // Buttons
    QPushButton     *pButtonExit;
    QPushButton     *pButtonStart;
    QPushButton     *pButtonAlert;

    // Vertical-Box
    QVBoxLayout     *vBox1;

    // Horizontal-Box
    QHBoxLayout     *hBox1;

    // MainWindow
    QMainWindow     *MainWindow;

    // Methodes
    void generateStartLayout();

    void setupUIElements();
    void setupConnects();
    void styleWidget();

signals:

public slots:



private slots:
    void fillProgressBar();
    void closeStarCar();
    void showAlert();
};

#endif // STARTWIDGET_H
