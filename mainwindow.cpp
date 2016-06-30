#include "mainwindow.h"
#include "ui_mainwindow.h"

//#### Блок констант ########################################################################
const float MIN_COINCIDENCE_COEFFICIENT = 49.0; // мінімальний коефіцієнт співпадання
const int MAX_WORDS = 500;
int D[MAX_WORDS][MAX_WORDS] = {0};
int M1[MAX_WORDS] = {0};
int M2[MAX_WORDS] = {0};
int word_state_m[MAX_WORDS] = {0}; // 0 - правильне / 1 - слово, якого немає в введеному масиві
int word_state_u[MAX_WORDS] = {0}; // 0 - зайве слово / 1- слово правильне / 2 -слово з помилкою
#define OUT { if (i == main_text.count() || j == user_text.count()) { Go = false; break;} }
// ###########################################################################################

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent),  ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->help_action,SIGNAL(triggered(bool)),this,SLOT(show_help_window()));
    connect(ui->about_action,SIGNAL(triggered(bool)),this,SLOT(show_about_window()));
    connect(ui->text_info,SIGNAL(triggered(bool)),this,SLOT(show_text_info_window()));
}

bool MainWindow::same_word(QString word1, QString word2)
{
    return (word1 == word2);
}

bool MainWindow::same_word_with_mistake(QString word1, QString word2)
{
    int d[50][50] = {0};

    for (int i = 1; i <= word1.size(); i++)
    {
        for (int j = 1; j <= word2.size(); j++)
        {
            if (word1[i-1] == word2[j-1])
                d[i][j] = d[i-1][j-1]+1;
            else
                d[i][j] = (d[i-1][j] < d[i][j-1]) ? d[i][j-1] : d[i-1][j];
        }
    }

    int tmp = abs(word2.size() - d[word1.size()][word2.size()]);
    return ((float)tmp <= word2.size()/2.0);
}

void MainWindow::parsing(QList<QString> initial_list, QList<QString> &parsed_list)
{
    for (int i = 0; i < initial_list.count(); i++)
    {
        if (initial_list[i].contains(","))
        {
            initial_list[i].replace(",","");
            parsed_list.append(initial_list[i]);
            parsed_list.append(",");
        }

        else if (initial_list[i].contains("."))
        {
            QString tmp_word = initial_list[i];
            int count_dots = 0;
            for (int ind = 0; ind < tmp_word.length(); ind++)
            {
                if (tmp_word[ind] == '.')
                    count_dots++;
            }

            if (count_dots == 1)
            {
                initial_list[i].replace(".","");
                parsed_list.append(initial_list[i]);
                parsed_list.append(".");
            }
            if (count_dots == 3)
            {
                initial_list[i].replace(".","");
                parsed_list.append(initial_list[i]);
                parsed_list.append("...");
            }
        }

        else if (initial_list[i].contains("!"))
        {
            initial_list[i].replace("!","");
            parsed_list.append(initial_list[i]);
            parsed_list.append("!");
        }

        else if (initial_list[i].contains("?"))
        {
            initial_list[i].replace("?","");
            parsed_list.append(initial_list[i]);
            parsed_list.append("?");
        }

        else if (initial_list[i].contains(":"))
        {
            initial_list[i].replace(":","");
            parsed_list.append(initial_list[i]);
            parsed_list.append(":");
        }

        else if (initial_list[i].contains(";"))
        {
            initial_list[i].replace(";","");
            parsed_list.append(initial_list[i]);
            parsed_list.append(";");
        }

        else if (initial_list[i].contains("-"))
        {
            initial_list[i].replace("-","");
            parsed_list.append(initial_list[i]);
            parsed_list.append("-");
        }

        else if (initial_list[i].contains(")"))
        {
            initial_list[i].replace(")","");
            parsed_list.append(initial_list[i]);
            parsed_list.append(")");
        }

        else if (initial_list[i].contains("("))
        {
            initial_list[i].replace("(","");
            parsed_list.append(initial_list[i]);
            parsed_list.append("(");
        }

        else if (initial_list[i].contains("\""))
        {
            initial_list[i].replace("\"","");
            parsed_list.append(initial_list[i]);
            parsed_list.append("\"");
        }

        else if (initial_list[i].contains("»"))
        {
            initial_list[i].replace("»","");
            parsed_list.append(initial_list[i]);
            parsed_list.append("»");
        }
        else if (initial_list[i].contains("«"))
        {
            initial_list[i].replace("«","");
            parsed_list.append(initial_list[i]);
            parsed_list.append("«");
        }

        else
            parsed_list.append(initial_list[i]);

    }

    //for (int i = 0; i < parsed_list.count(); i++)
     //   ui->IncWords->append(parsed_list[i]);
}

int MainWindow::check(QList<QString> main_text, QList<QString> user_text)
{
    for (int i = 1; i <= main_text.count(); i++)
    {
        for (int j = 1; j <= user_text.count(); j++)
        {
            if (same_word_with_mistake(main_text[i-1],user_text[j-1]))
                D[i][j] = D[i-1][j-1]+1;
            else
                D[i][j] = (D[i-1][j] < D[i][j-1]) ? D[i][j-1] : D[i-1][j];
        }
    }

    return D[main_text.count()][user_text.count()];
}

void MainWindow::flagging(QList<QString> main_text, QList<QString> user_text)
{
    for (int i = main_text.count(), j = user_text.count(); !(i == 0&&j == 0);)
    {
          while (D[i][j] == D[i][j-1])
          {
              word_state_u[j]=0;
              j--;
          }

          while ( D[i][j] == D[i-1][j])
          {
              word_state_m[j]=1;
              i--;
          }

          if (same_word(main_text[i-1],user_text[j-1]))
              word_state_u[j] = 1;

          else if (same_word_with_mistake(main_text[i-1],user_text[j-1]))
              word_state_u[j] = 2;

          i--; j--;

    }
}

void MainWindow::rendering(QList<QString> main_text, QList<QString> user_text)
{
    QString out_buffer("");
        QString buf("");
        bool Go = true;
        int i = 0, j = 0;
        // word_state_m ------ main_text ----- i ----- 0(ok) 1(un)
        // word_state_u ------ user_text ----- j ----- 1(ok) 2(err) 3(un)
        while (Go)
        {

            if (word_state_m[i+1] == 0 && word_state_u[j+1] == 1) // все ок
            {
                out_buffer += user_text[j];
                out_buffer += ' ';
                i++;   j++;
                OUT
                continue;
            }

            if (word_state_m[i+1] == 0 && word_state_u[j+1] == 2) // слово з помилкою
            {
                buf = user_text[j]; purple_highlighting(buf);
                out_buffer += buf;
                out_buffer += ' ';
                i++; j++;
                OUT
                continue;
            }
            if (word_state_m[i+1] == 0 && word_state_u[j+1] == 0) // лишнє слово
            {
                buf = user_text[j]; olive_highlighting(buf);
                out_buffer += buf;
                out_buffer += ' ';
                j++;
                OUT
                continue;
            }

            if (word_state_m[i+1] == 1 && word_state_u[j+1] == 0) // пропущене слово
            {
                buf = user_text[j]; teal_highlighting(buf);
                out_buffer += buf;
                out_buffer += ' ';
                buf = main_text[i+1]; teal_highlighting(buf);
                out_buffer += buf;
                out_buffer += ' ';

                i++; j++;
                OUT
                continue;
            }

            if (word_state_m[i+1] == 1 && word_state_u[j+1] == 1)
            {
                buf = user_text[i];
                out_buffer += buf;
                out_buffer += ' ';
                buf = main_text[i+1]; teal_highlighting(buf);
                out_buffer += buf;
                out_buffer += ' ';
                j++; i++;
                OUT
                continue;
            }

            if (word_state_m[i+1] == 1 && word_state_u[j+1] == 2)
            {

                buf = user_text[i]; purple_highlighting(buf);
                out_buffer += buf;
                out_buffer += ' ';
                buf = main_text[i+1]; teal_highlighting(buf);
                out_buffer += buf;
                out_buffer += ' ';
                j++; i++;
                OUT
                continue;
            }

        }

        QString str;
        text_clear (out_buffer, str);
        ui->UserText->append(str);
}

void MainWindow::olive_highlighting(QString &word)
{
    word = "<font color= #808080>" + word + "</font>";
}

void MainWindow::teal_highlighting(QString &word)
{
    word = "<font color = #008080>" + word + "</font>";
}

void MainWindow::purple_highlighting(QString &word)
{
    word = "<font color =#800080>" + word + "</font>";
}

void MainWindow::show_text(QString text)
{
    QWidget *win = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    QTextEdit *edit = new QTextEdit(text);
    layout->addWidget(edit);
    win->setLayout(layout);
    edit->setReadOnly(1);
    win->show();
}

void MainWindow::text_clear(QString main_text, QString &str)
{
    str = main_text;
    for (int i=0; i<str.length(); i++)
        if ((str[i]==' ') and (
            (str[i+1] == '.') or
            (str[i+1] == ',') or
            (str[i+1] == ':') or
            (str[i+1] == '!') or
            (str[i+1] == ';') or
            (str[i+1] == '?') or
            (str[i+1] == '\'')))
//            (str[i+1] == "«") or
//            (str[i+1] == "»") or
//            (str[i+1] == '"')))
        for(int j=i; j<str.length(); j++)
            str[j]=str[j+1];
    int r=0;
    QString strtmp;
    for (int i=str.length()-1; i>0; i--)
    {
        if((str[i]==' ') and (str[i-1]!=' '))
        {
            r=i;
            break;
        }
    }
    for (int i=0; i<r; i++)
    strtmp[i]= str[i];
    str=strtmp;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString user_text = ui->UserText->toPlainText();
    ui->UserText->clear();

    QFile file("data/text.txt");
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this,"Помилка", "Помилка при відкритті файлу:\n" + file.errorString());
        return;
    }

    QByteArray array = file.readAll();
    QString main_text;
    main_text.append(array);

    QList<QString> init_main_text_words = main_text.split(QRegularExpression(" "),QString::SkipEmptyParts);
    QList<QString> init_user_text_words = user_text.split(QRegularExpression(" "),QString::SkipEmptyParts);

    QList<QString> main_text_words, user_text_words;

    parsing(init_main_text_words, main_text_words);
    parsing(init_user_text_words, user_text_words);

    check(main_text_words,user_text_words); // складаємо матрицю D[][]
    flagging(main_text_words,user_text_words); // відмічаємо стани

    rendering (main_text_words,user_text_words); // збираємо на вихід

    ui->pushButton->setEnabled(0);
    ui->UserText->setReadOnly(1);


    show_text(main_text);
}

void MainWindow::show_help_window()
{
    QWidget *win = new QWidget;
    win->resize(500,520);
    QVBoxLayout *layout = new QVBoxLayout;
    win->setFixedSize(win->size());
    QFile file ("data/help.html");

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this,"Помилка", "Помилка при відкритті файлу:\n" + file.errorString());
        return;
    }

    QByteArray array = file.readAll();
    QString b;
    b.append(array);
    QTextEdit *edit = new QTextEdit(b);
    layout->addWidget(edit);
    win->setLayout(layout);
    edit->setReadOnly(1);
    file.close();
    win->show();
}

void MainWindow::show_about_window()
{
    QWidget *win = new QWidget;
    win->resize(230,205);
    QVBoxLayout *layout = new QVBoxLayout;
    win->setFixedSize(win->size());
    QFile file ("data/about.html");

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this,"Помилка", "Помилка при відкритті файлу:\n" + file.errorString());
        return;
    }

    QByteArray array = file.readAll();
    QString b;
    b.append(array);
    QTextEdit *edit = new QTextEdit(b);
    layout->addWidget(edit);
    win->setLayout(layout);
    edit->setReadOnly(1);
    file.close();
    win->show();
}

void MainWindow::show_text_info_window()
{
    QMessageBox::information(this,"Інформація про текст", "Назва: Тут назва\nКількість слів: 100\n"
                                                          "Відведений час: 5хв.\n");
}
