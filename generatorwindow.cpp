#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GeneratorWindow)
{
    random = QRandomGenerator::global();

    ui->setupUi(this);
    ui->genButton->setEnabled(true);
    ui->taskView->setSelectionMode(QAbstractItemView::NoSelection);
}
GeneratorWindow::~GeneratorWindow()
{
    delete ui;
}

void GeneratorWindow::on_genButton_clicked()
{
    if (ui->tasksList->currentItem()->text() == "Функция Эйлера") {
        DialogEulerFunction *window = new DialogEulerFunction(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogEulerFunctionMeta(int)),
                this, SLOT(slotDialogEulerFunctionMeta(int)));
        connect(window, SIGNAL(dialogEulerFunction(int, int, int, EulerFunctionOptions)),
                this, SLOT(slotDialogEulerFunction(int, int, int, EulerFunctionOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Функция Мёбиуса") {
        DialogMebiusFunction *window = new DialogMebiusFunction(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogMebiusFunctionMeta(int)),
                this, SLOT(slotDialogMebiusFunctionMeta(int)));
        connect(window, SIGNAL(dialogMebiusFunction(int, int, int, MebiusFunctionOptions)),
                this, SLOT(slotDialogMebiusFunction(int, int, int, MebiusFunctionOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Символ Лежандра") {
        DialogSymbolLegandre *window = new DialogSymbolLegandre(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSymbolLegandreMeta(int)),
                this, SLOT(slotDialogSymbolLegandreMeta(int)));
        connect(window, SIGNAL(dialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)),
                this, SLOT(slotDialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Символ Якоби") {
        DialogSymbolJacobi *window = new DialogSymbolJacobi(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSymbolJacobiMeta(int)),
                this, SLOT(slotDialogSymbolJacobiMeta(int)));
        connect(window, SIGNAL(dialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions)),
                this, SLOT(slotDialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Группа Перестановок") {
        DialogTranspositionGroup *window = new DialogTranspositionGroup(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogTranspositionGroupMeta(int)),
                this, SLOT(slotDialogTranspositionGroupMeta(int)));
        connect(window, SIGNAL(dialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode)),
                this, SLOT(slotDialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode)));
    }
}
void GeneratorWindow::on_actionTXT_triggered()
{
    QString path = QFileDialog::getSaveFileName(0, QObject::tr("Сохранить задания"),
        "/home/kataich75/qtprojects/TasksGenerator/tests/test_" +
        QDateTime::currentDateTime().toString("dd.MM.yyyy") + "_" +
        QDateTime::currentDateTime().toString("hh:mm:ss") + ".txt", QObject::tr("TXT files (*.txt)"));
    file.setFileName(path);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        int taskIndex = 0;
        for (int i = 0; i < generatedTasks.size(); i += 2) {
            int task = generatedTasks[i].toInt();
            int countOfTasks = generatedTasks[i + 1].toInt();
            out << task << " " << countOfTasks << "\n";
            switch (task) {
            case TaskEulerFunction:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            case TaskMebiusFunction:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            case TaskSymbolLegandre:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1]
                        << " " << generatedData[taskIndex + 2] << "\n";
                    taskIndex += 3;
                } break;
            case TaskSymbolJacobi:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1]
                        << " " << generatedData[taskIndex + 2] << "\n";
                    taskIndex += 3;
                } break;
            case TaskTranspositionGroup:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            }
        }
    }
    file.close();
}
void GeneratorWindow::on_actionQuit_triggered()
{
    qApp->exit();
}

void GeneratorWindow::slotDialogEulerFunctionMeta(int countOfTasks)
{
    generatedTasks.push_back(QString::number(TaskEulerFunction));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        QListWidgetItem *item = new QListWidgetItem("Вычислите функцию Эйлера:", ui->taskView);
        ui->taskView->addItem(item);
    }
}
void GeneratorWindow::slotDialogEulerFunction(int countOfTasks, int minNum, int maxNum, EulerFunctionOptions option)
{
    EulerFunction task;
    QListWidgetItem *item;
    switch (option) {
    case EulerFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
            ui->taskView->addItem(item);
            generatedData.push_back(QString::number(task.getTask()));
            generatedData.push_back(QString::number(task.solve()));
        } break;
    case EulerFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) {
                item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case EulerFunctionOptions::PrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1 && data[0].second > 1) {
                item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second != 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second == 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    }
}

void GeneratorWindow::slotDialogMebiusFunctionMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskMebiusFunction));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        QListWidgetItem *item = new QListWidgetItem("Вычислите функцию Мёбиуса:", ui->taskView);
        ui->taskView->addItem(item);
    }
}
void GeneratorWindow::slotDialogMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option){
    MebiusFunction task;
    QListWidgetItem *item;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
            ui->taskView->addItem(item);
            generatedData.push_back(QString::number(task.getTask()));
            generatedData.push_back(QString::number(task.solve()));
        } break;
    case MebiusFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!isPrime(task.getTask())) {
                item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case MebiusFunctionOptions::EvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 != 0) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case MebiusFunctionOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 == 0) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case MebiusFunctionOptions::PrimeDegree:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            bool accessFlag = false;
            for (int i = 0; i < data.size(); i++) {
                if (data[i].second > 1) {
                    accessFlag = true;
                    break;
                }
            }
            if (accessFlag) {
                item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    }
}

void GeneratorWindow::slotDialogSymbolLegandreMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskSymbolLegandre));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        QListWidgetItem *item = new QListWidgetItem("Вычислите символ Лежандра:", ui->taskView);
        ui->taskView->addItem(item);
    }
}
void GeneratorWindow::slotDialogSymbolLegandre(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolLegandreOptions option){
    SymbolLegandre task;
    QListWidgetItem *item;
    switch (option) {
    case SymbolLegandreOptions::Default:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolLegandreOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first)) {
                item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolLegandreOptions::aEqual_1:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolLegandreOptions::aEqual2:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolLegandreOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0) {
                item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    }
}

void GeneratorWindow::slotDialogSymbolJacobiMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskSymbolJacobi));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        QListWidgetItem *item = new QListWidgetItem("Вычислите символ Якоби:", ui->taskView);
        ui->taskView->addItem(item);
    }
}
void GeneratorWindow::slotDialogSymbolJacobi(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolJacobiOptions option){
    SymbolJacobi task;
    QListWidgetItem *item;
    switch (option) {
    case SymbolJacobiOptions::Default:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                item = new QListWidgetItem("J(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolJacobiOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().second % 2 != 0) {
                item = new QListWidgetItem("J(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolJacobiOptions::aEqual_1:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                item = new QListWidgetItem("J(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolJacobiOptions::aEqual2:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                item = new QListWidgetItem("J(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    case SymbolJacobiOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0 && task.getTask().second % 2 != 0) {
                item = new QListWidgetItem("J(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(QString::number(task.getTask().first));
                generatedData.push_back(QString::number(task.getTask().second));
                generatedData.push_back(QString::number(task.solve()));
            }
        } break;
    }
}

void GeneratorWindow::slotDialogTranspositionGroupMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskTranspositionGroup));
    generatedTasks.push_back(QString::number(countOfTasks));
}
void GeneratorWindow::slotDialogTranspositionGroup(int countOfTasks, int minN, int maxN, TranspositionGroupOptions option, ViewMode mode){
    TranspositionGroup task, task2, result;
    QListWidgetItem *item;
    switch (option) {
    case TranspositionGroupOptions::Write:
        if (countOfTasks > 0) {
            item = new QListWidgetItem("Запишите перестановку в каноническом/циклическом виде:", ui->taskView);
            ui->taskView->addItem(item);
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Standart) + " = " + task.writeToMode(ViewMode::Cycle), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } else {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Cycle) + " = " + task.writeToMode(ViewMode::Standart), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } ui->taskView->addItem(item); i++;
        } break;
    case TranspositionGroupOptions::Multiply:
        if (countOfTasks > 0) {
            item = new QListWidgetItem("Найдите произведение заданных перестановок:", ui->taskView);
            ui->taskView->addItem(item);
        }
        for (int i = 0; i < countOfTasks;) {
            int n = static_cast<int>(random->bounded(minN, maxN));
            task.setTask(n, mode);
            task2.setTask(n, mode);
            result = task * task2;
            if (task.getViewMode() == ViewMode::Standart) {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Standart) + " * " +
                     task2.writeToMode(ViewMode::Standart) + " = " + result.writeToMode(ViewMode::Standart), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Standart) + " " + task2.writeToMode(ViewMode::Standart));
                generatedData.push_back(result.writeToMode(ViewMode::Standart));
            } else {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Cycle) + " * " +
                     task2.writeToMode(ViewMode::Cycle) + " = " + result.writeToMode(ViewMode::Cycle), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Cycle) + " " + task2.writeToMode(ViewMode::Cycle));
                generatedData.push_back(result.writeToMode(ViewMode::Cycle));
            } ui->taskView->addItem(item); i++;
        } break;
    case TranspositionGroupOptions::Inverse:
        if (countOfTasks > 0) {
            item = new QListWidgetItem("Найдите обратную к данной перестановке:", ui->taskView);
            ui->taskView->addItem(item);
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (task.getViewMode() == ViewMode::Standart) {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": ~" + task.writeToMode(ViewMode::Standart) + " = " +
                     result.writeToMode(ViewMode::Standart), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
                generatedData.push_back(result.writeToMode(ViewMode::Standart));
            } else {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": ~" + task.writeToMode(ViewMode::Cycle) + " = " +
                     result.writeToMode(ViewMode::Cycle), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
                generatedData.push_back(result.writeToMode(ViewMode::Cycle));
            } ui->taskView->addItem(item); i++;
        } break;
    case TranspositionGroupOptions::Cycle:
        if (countOfTasks > 0) {
            item = new QListWidgetItem("Вычислите цикловой тип подстановки:", ui->taskView);
            ui->taskView->addItem(item);
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Standart) + " = " +
                     task.cycleType(), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Cycle) + " = " +
                     task.cycleType(), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } ui->taskView->addItem(item); i++; generatedData.push_back(task.cycleType());
        } break;
    case TranspositionGroupOptions::Count:
        if (countOfTasks > 0) {
            item = new QListWidgetItem("Вычислите количество беспорядков подстановки:", ui->taskView);
            ui->taskView->addItem(item);
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Standart) + " = " +
                     QString::number(task.getHaos()), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Cycle) + " = " +
                     QString::number(task.getHaos()), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } ui->taskView->addItem(item); i++; generatedData.push_back(QString::number(task.getHaos()));
        } break;
    case TranspositionGroupOptions::Mod2:
        if (countOfTasks > 0) {
            item = new QListWidgetItem("Определите честность подстановки:", ui->taskView);
            ui->taskView->addItem(item);
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Standart) + " - " +
                     task.getEven(), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Cycle) + " - " +
                     task.getEven(), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } ui->taskView->addItem(item); i++; generatedData.push_back(task.getEven());
        } break;
    case TranspositionGroupOptions::Order:
        if (countOfTasks > 0) {
            item = new QListWidgetItem("Определите порядок подстановки:", ui->taskView);
            ui->taskView->addItem(item);
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Standart) + " = " +
                     QString::number(task.getOrder()), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                item = new QListWidgetItem("S/" + QString::number(task.getTask()) + ": " + task.writeToMode(ViewMode::Cycle) + " = " +
                     QString::number(task.getOrder()), ui->taskView);
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } ui->taskView->addItem(item); i++; generatedData.push_back(QString::number(task.getOrder()));
        } break;
    }
}
