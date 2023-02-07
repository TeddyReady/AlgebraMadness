#include "dialoglatexprinter.h"
#include "ui_dialoglatexprinter.h"

DialogLatexPrinter::DialogLatexPrinter(QString data, QString answers, QWidget *parent) :
    data(data), answers(answers), QDialog(parent),
    ui(new Ui::DialogLatexPrinter)
{
    ui->setupUi(this);
    ui->lineTitle->setText("Контрольная Работа №1");
    ui->lineTheme->setText("Функция Эйлера");
    ui->lineDate->setDate(QDate::currentDate());
    engine = new TeXEngine(ui->pdfView);
    engineAnswers = new TeXEngine(ui->pdfViewAnswers);
    setMinimumWidth(1200);
    setMinimumHeight(800);

    this->answers.replace(QRegularExpression("(@.+?#)"), "~");

    /*answers = "$$\\begin{array}{|r|l|}"
              "\\hline"
              "Вопрос & Ответ \\\\"
              "Вопрос & Ответ \\\\"
              "Вопрос & Ответ \\\\"
              "84 & десятичное \\\\"
              "\\hline"
              "\\end{array}$$";*/
}

DialogLatexPrinter::~DialogLatexPrinter()
{
    delete engine;
    delete engineAnswers;
    delete ui;
}

void DialogLatexPrinter::printData()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохраните файл с заданиями", QDir::homePath(), "LaTeX & PDF файлы (*.tex, *.pdf)");
    //Pdf TASKS File
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));
    printer.setOutputFileName("auto_" + fileName + ".pdf");
    ui->pdfView->print(&printer);

    //TeX TASKS File
    if (!ui->btnLaTeX->isChecked()) return;
    QString title = "", theme = "", date = "";
    if (ui->btnDate->isChecked())
        date = ui->lineDate->text();
    if (ui->btnTheme->isChecked())
        theme = ui->lineTheme->text();
    if (ui->btnTitle->isChecked())
        title = ui->lineTitle->text();

    QFile texFile(fileName + ".tex");
    if (!texFile.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Ошибка открытия файла", "Выбранный файл не может быть открыт или не существует.");
        return;
    } else {
        QString texString = "\\documentclass[12pt, a4paper]{article}\n"
                            "\\usepackage{graphicx}\n"
                            "\\usepackage[T2A]{fontenc}\n"
                            "\\usepackage[russian]{babel}\n"
                            "\\usepackage{amsmath}\n"
                            "\\usepackage{amsfonts}\n"
                            "\\title{" + title + "}\n"
                            "\\author{" + theme + "}\n"
                            "\\date{" + date + "}\n"
                            "\\begin{document}\n"
                            "\\maketitle\n"
                            "\\large{\n"
                            + data.replace("\\\\", "\\\\\n").replace("  ", "$").replace("?", "?$") +
                            "}\n"
                            "\\end{document}\n";
        QTextStream out(&texFile);
        out << texString;
        texFile.close();
    } close();
}

void DialogLatexPrinter::on_btnShow_clicked()
{
    QString tmp = data, answers = this->answers;
    if (ui->btnDate->isChecked()) {
        tmp = "\\large{\\text{" + ui->lineDate->text() + "}}\\\\" + tmp;
        answers = "\\large{\\text{" + ui->lineDate->text() + "}}\\\\" + answers;
    }
    if (ui->btnTheme->isChecked()) {
        tmp = "\\LARGE{\\textit{" + ui->lineTheme->text() + "}}\\\\" + tmp;
        answers = "\\LARGE{\\textit{" + ui->lineTheme->text() + "}}\\\\" + answers;
    }
    if (ui->btnTitle->isChecked()) {
        tmp = "\\huge{\\textbf{" + ui->lineTitle->text() + "}}\\\\" + tmp;
        answers = "\\huge{\\textbf{" + ui->lineTitle->text() + "}}\\\\" + answers;
    }
    engine->TeX2SVG(tmp, true);
    engineAnswers->TeX2SVG("\\Huge{\\textbf{Ответы}}\\\\" + answers, true);
}


void DialogLatexPrinter::on_buttonBox_accepted()
{
    on_btnShow_clicked();
    printData();
}


void DialogLatexPrinter::on_buttonBox_rejected()
{
    close();
}
