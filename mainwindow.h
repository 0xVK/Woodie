#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QList>
#include <algorithm>
#include <QSound>
#include <QFile>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool same_word (QString word1, QString word2); // перевірка чи це те саме слово
    bool same_word_with_mistake (QString word1, QString word2); // перевірка чи це те саме слово з помилкою
    void parsing (QList<QString> initial_list, QList<QString> &parsed_list); // розбиття текста на слова і знаки
    int  check (QList<QString> main_text,QList<QString> user_text); // знахлдження найбільшого входження
    void flagging (QList<QString> main_text,QList<QString> user_text);
    void rendering (QList<QString> main_text,QList<QString> user_text); // формування та виділення
    void olive_highlighting (QString &word); // зайве слово
    void teal_highlighting (QString &word); // пропущене слово
    void purple_highlighting (QString &word); // неправильне слово
    void show_text(QString text);
    void text_clear (QString main_text, QString &str);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void show_help_window ();
    void show_about_window ();
    void show_text_info_window ();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
