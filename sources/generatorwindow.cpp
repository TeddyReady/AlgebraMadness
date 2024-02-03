﻿#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent), totalTestTasks(0), lastSizeCount(0), mode(false),
      random(QRandomGenerator::global()), ui(new Ui::GeneratorWindow)
{
    uploadSettings();
    uploadUI();
    getConnection();

    showMaximized();
}

GeneratorWindow::~GeneratorWindow()
{
    delete engine;
    delete ui;
}

void GeneratorWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void GeneratorWindow::uploadUI()
{
    ui->setupUi(this);
    ui->genButton->setEnabled(true);
    ui->genButton->setCursor(Qt::PointingHandCursor);
    ui->testButton->setCursor(Qt::PointingHandCursor);
    ui->tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->addTab(new TuringMachine, tr("Машина Тьюринга"));
    ui->taskView->setCursor(Qt::BlankCursor);
    ui->taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    engine = new TeXEngine(ui->taskView);

    ui->libraryLayout->addWidget(new PDFViewer(RSC::theory::theory));

    /* Left Alignment */
    ui->toolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::checkAnswers), "Показать ответы", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::clearTasks), "Очистить задачи", ui->toolBar));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::printer), "Подготовить печатный вариант...", ui->toolBar));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::compile), "Запустить программу", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::reference_manual), "Справка по программированию", ui->toolBar));

    ui->toolBar->actions().at(Check)->setDisabled(true);
    ui->toolBar->actions().at(Clear)->setDisabled(true);
    ui->toolBar->actions().at(Print)->setDisabled(true);
    ui->toolBar->actions().at(Compile)->setVisible(false);
    ui->toolBar->actions().at(MNRInfo)->setVisible(false);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);

    /* Right Alignment */
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::manual), "Справочник", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::font_size), "Настройки шрифта...", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap(RSC::pics::quit), "Выход", ui->toolBar));

    for (int i = 0; i < ui->tasksList->count(); ++i)
        ui->taskBox->addItem(ui->tasksList->item(i)->text());

    /* Tasks List */
    ui->tasksList->setResizeMode(QListWidget::Adjust);
    ui->tasksList->setIconSize(QSize(20, 20));
    uploadListWidget();

    setCursor(Qt::ArrowCursor);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(App::AppName);
    setWindowIcon(QIcon(RSC::pics::appIcon));
}

void GeneratorWindow::uploadListWidget()
{
    QDomDocument document;
    QFile tasksFile(RSC::xml::tasks);
    if (!tasksFile.open(QIODevice::ReadOnly ))
    {
        qDebug() << "Cannot open xml file:" << tasksFile;
        return;
    }
    document.setContent(&tasksFile);
    tasksFile.close();

    QDomNodeList taskNames = document.elementsByTagName(QString("task"));

    for (int taskCounter = 0; taskCounter < taskNames.count(); ++taskCounter)
    {
        if (taskNames.item(taskCounter).isElement() && taskNames.item(taskCounter).toElement().attribute("type") == "on")
        {
            QDomNodeList subTasks = taskNames.item(taskCounter).toElement().elementsByTagName(QString("subtask"));

            TaskItem *curItem = new TaskItem(taskNames.item(taskCounter).toElement().attribute("name"));
            curItem->setTextAlignment(Qt::AlignLeft);

            if (subTasks.count() != 0)
            {
                curItem->setIcon(QIcon(RSC::pics::listWidget_arrow));

                for (int subCounter = 0; subCounter < subTasks.count(); ++subCounter)
                {
                    if (subTasks.item(taskCounter).isElement() && subTasks.item(taskCounter).toElement().attribute("type") == "on")
                    {

                    }
                }
            }

            ui->tasksList->addItem(curItem);
        }
    }
}

void GeneratorWindow::saveSettings()
{
    QSettings settings(App::OrgName, App::AppName);

    settings.beginGroup("General");
    settings.setValue("windowSize", geometry());
    settings.endGroup();

    settings.beginGroup("Style");
    settings.setValue("taskFontSize", taskFontSize);
    settings.setValue("mathFontSize", mathFontSize);
    settings.endGroup();
}

void GeneratorWindow::uploadSettings()
{
    QSettings settings(App::OrgName, App::AppName);

    settings.beginGroup("General");
    setGeometry(settings.value("windowSize").toRect());
    settings.endGroup();

    settings.beginGroup("Style");
    taskFontSize = settings.value("taskFontSize", "\\normalsize").toString();
    mathFontSize = settings.value("mathFontSize", "\\normalsize").toString();
    settings.endGroup();
}

void GeneratorWindow::getConnection()
{
    connect(ui->toolBar->actions().at(Check), &QAction::triggered, this, &GeneratorWindow::checkAnswers);
    connect(ui->toolBar->actions().at(Clear), &QAction::triggered, this, &GeneratorWindow::clearTasks);
    connect(ui->toolBar->actions().at(Print), &QAction::triggered, this, &GeneratorWindow::printTasks);
    connect(ui->toolBar->actions().at(Manual), &QAction::triggered, this, &GeneratorWindow::openManual);
    connect(ui->toolBar->actions().at(Font), &QAction::triggered, this, &GeneratorWindow::changeFontSize);
    connect(ui->toolBar->actions().at(Compile), &QAction::triggered, dynamic_cast<TuringMachine *>(ui->tabWidget->widget(3)), &TuringMachine::do_compile);
    connect(ui->toolBar->actions().at(MNRInfo), &QAction::triggered, dynamic_cast<TuringMachine *>(ui->tabWidget->widget(3)), &TuringMachine::do_info);
    connect(ui->toolBar->actions().at(Exit), &QAction::triggered, [&](){qApp->closeAllWindows();});

    connect(ui->genButton, &QPushButton::clicked, this, &GeneratorWindow::generateTask);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &GeneratorWindow::switchTab);

    connect(ui->taskBox, &QComboBox::currentTextChanged, this, &GeneratorWindow::switchTask);
    connect(ui->testButton, &QPushButton::clicked, this, &GeneratorWindow::runTest);
}

void GeneratorWindow::isReadyRender()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = lastSizeCount; i < descriptions.size(); ++i)
    {
        taskForTeX.append("{TASK_FONT\\textbf{" + descriptions.at(i) + ":}}\\\\");
        for (int j = 0; j < tasks.at(i).size(); ++j)
        {
            taskForTeX.append("{MATH_FONT" + QString::number(j + 1) + ")~" + tasks.at(i).at(j) + "}\\\\");
        }
    }
    lastSizeCount = descriptions.size();
    QString TeXBuffer = taskForTeX;
    TeXBuffer.replace("TASK_FONT", taskFontSize).replace("MATH_FONT", mathFontSize);
    engine->TeX2SVG(MathJax::startConf + TeXBuffer + MathJax::endConf, true);
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::checkAnswers()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString TeXBuffer = taskForTeX;
    TeXBuffer.replace("TASK_FONT", taskFontSize).replace("MATH_FONT", mathFontSize);
    for (int i = 0; i < answers.size(); i++)
        TeXBuffer.replace(TeXBuffer.indexOf('?'), QString("?").size(), answers[i]);

    engine->TeX2SVG(MathJax::startConf + TeXBuffer + MathJax::endConf, true);
    ui->toolBar->actions().at(Check)->setDisabled(true);
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::clearTasks()
{
    descriptions.clear();
    tasks.clear();
    answers.clear();
    taskForTeX.clear();
    lastSizeCount = 0;
    engine->TeX2SVG(MathJax::startConf + taskFontSize + "{\\textbf{В ожидании генерации задач...}}" + MathJax::endConf, true);
    ui->toolBar->actions().at(Check)->setDisabled(true);
    ui->toolBar->actions().at(Clear)->setDisabled(true);
    ui->toolBar->actions().at(Print)->setDisabled(true);
}

void GeneratorWindow::printTasks()
{
    DialogLatexPrinter *latexDialog = new DialogLatexPrinter(descriptions, tasks, answers, this);
    latexDialog->exec();
}

void GeneratorWindow::openManual()
{
    DialogManual *manDialog = new DialogManual(taskFontSize, mathFontSize, this);
    manDialog->show();
}

void GeneratorWindow::changeFontSize()
{
    DialogFontSize *fontDialog = new DialogFontSize(taskFontSize, mathFontSize, this);

    connect(fontDialog, &DialogFontSize::changeTaskFontSize, [&](QString newFont){
        taskFontSize = newFont;
    });
    connect(fontDialog, &DialogFontSize::changeMathFontSize, [&](QString newFont){
        mathFontSize = newFont;
    });

    fontDialog->exec();
}

void GeneratorWindow::switchTab(int index)
{
    switch (static_cast<TabBarModes>(index)) {
    case TabBarModes::TestMode:
        mode = 1;
        ui->toolBar->actions().at(Check)->setVisible(false);
        ui->toolBar->actions().at(Clear)->setVisible(false);
        ui->toolBar->actions().at(Sep_1)->setVisible(false);
        ui->toolBar->actions().at(Print)->setVisible(false);
        ui->toolBar->actions().at(Sep_2)->setVisible(false);
        ui->toolBar->actions().at(Compile)->setVisible(false);
        ui->toolBar->actions().at(MNRInfo)->setVisible(false);
        break;

    case TabBarModes::Tasks:
        mode = 0;
        ui->toolBar->actions().at(Check)->setVisible(true);
        ui->toolBar->actions().at(Clear)->setVisible(true);
        ui->toolBar->actions().at(Sep_1)->setVisible(true);
        ui->toolBar->actions().at(Print)->setVisible(true);
        ui->toolBar->actions().at(Sep_2)->setVisible(true);
        ui->toolBar->actions().at(Compile)->setVisible(false);
        ui->toolBar->actions().at(MNRInfo)->setVisible(false);
        break;

    case TabBarModes::Library:
        mode = 0;
        ui->toolBar->actions().at(Check)->setVisible(false);
        ui->toolBar->actions().at(Clear)->setVisible(false);
        ui->toolBar->actions().at(Sep_1)->setVisible(false);
        ui->toolBar->actions().at(Print)->setVisible(false);
        ui->toolBar->actions().at(Sep_2)->setVisible(false);
        ui->toolBar->actions().at(Compile)->setVisible(false);
        ui->toolBar->actions().at(MNRInfo)->setVisible(false);
        break;

    case TabBarModes::TuringMachine:
        mode = 0;
        ui->toolBar->actions().at(Check)->setVisible(false);
        ui->toolBar->actions().at(Clear)->setVisible(false);
        ui->toolBar->actions().at(Sep_1)->setVisible(false);
        ui->toolBar->actions().at(Print)->setVisible(false);
        ui->toolBar->actions().at(Sep_2)->setVisible(false);
        ui->toolBar->actions().at(Compile)->setVisible(true);
        ui->toolBar->actions().at(MNRInfo)->setVisible(true);
        break;
    }
}

void GeneratorWindow::runTest()
{
    if (tasksForTest.isEmpty())
        statusBar()->showMessage("Перед запуском теста, необходимо сгенерировать задания!", 1500);
    else {
        DialogTestTimer *window = new DialogTestTimer(this);
        connect(window, &DialogTestTimer::timeRemaining, this, &GeneratorWindow::startTest);
    }
}

void GeneratorWindow::startTest(QTime time)
{
    //Рандомная сортировка
    for (size_t i = 0; i < static_cast<size_t>(tasksForTest.size()); ++i) {
        int j = random->bounded(0, tasksForTest.size());
        auto tmp = tasksForTest[i];
        tasksForTest[i] = tasksForTest[j];
        tasksForTest[j] = tmp;
    }
    TestMode *testWindow = new TestMode(tasksForTest, time, this);
    testWindow->setWindowTitle("Тест");
    testWindow->showMaximized();
    totalTestTasks = 0;
    ui->lcdNumber->display(totalTestTasks);
}

void GeneratorWindow::generateTask()
{
    if (ui->tasksList->currentItem() == nullptr) {
        statusBar()->showMessage("Перед генерацией выберите типаж задания!");
        return;
    }
    runTaskManager(ui->tasksList->currentItem()->text(), true);
}

void GeneratorWindow::switchTask(const QString &task)
{
    runTaskManager(task, false);
}

void GeneratorWindow::runTaskManager(const QString &task, bool closeMode)
{
    if (!ui->mainLayout->isEmpty() && !closeMode)
        delete ui->mainLayout->takeAt(0)->widget();

    DialogBase *window;
    if (task == "Группа Перестановок")
        window = new DialogBase(AllTasks::TranspositionGroup, closeMode, this);
    else if (task == "Функция Эйлера")
        window = new DialogBase(AllTasks::EulerFunction, closeMode, this);
    else if (task == "Функция Мёбиуса")
        window = new DialogBase(AllTasks::MebiusFunction, closeMode, this);
    else if (task == "Символ Лежандра")
        window = new DialogBase(AllTasks::SymbolLegandre, closeMode, this);
    else if (task == "Символ Якоби")
        window = new DialogBase(AllTasks::SymbolJacobi, closeMode, this);
    else if (task == "Алгебраические Структуры")
        window = new DialogBase(AllTasks::AlgebraStructures, closeMode, this);
    else if (task == "Группы и их свойства")
        window = new DialogBase(AllTasks::GroupProperties, closeMode, this);
    else if (task == "Матрицы")
        window = new DialogBase(AllTasks::Matrix, closeMode, this);
    else if (task == "Кольцо Вычетов")
        window = new DialogBase(AllTasks::RingResidue, closeMode, this);
    else if (task == "Кольцо Многочленов")
        window = new DialogBase(AllTasks::RingOfMembers, closeMode, this);
    else if (task == "Комплексные Числа")
        window = new DialogBase(AllTasks::Complex, closeMode, this);
    else if (task == "Булевые Функции")
        window = new DialogBase(AllTasks::BooleanFunction, closeMode, this);
    else if (task == "Таблица Кэлли")
        window = new DialogBase(AllTasks::KeliTable, closeMode, this);
    else return;

    connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
    connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    if (!closeMode)
        ui->mainLayout->addWidget(window);
}

void GeneratorWindow::receivedMetaInfo(int countOfTasks)
{
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        ui->toolBar->actions().at(Check)->setEnabled(true);
        ui->toolBar->actions().at(Clear)->setEnabled(true);
        ui->toolBar->actions().at(Print)->setEnabled(true);
    }
}

void GeneratorWindow::receivedData(std::vector<int> data, AllTasks task, int subTask, int optional)
{
    /* If tasksCount is null */
    if (data[0] == 0) return;

    TaskInterface *interface;
    QVector<QString> tasks;

    switch (task) {
    case AllTasks::EulerFunction:
        interface = new EulerFunctionInterface(data[1], data[2], static_cast<EulerFunctionOptions>(subTask));
        break;
    case AllTasks::MebiusFunction:
        interface = new MebiusFunctionInterface(data[1], data[2], static_cast<MebiusFunctionOptions>(subTask));
        break;
    case AllTasks::SymbolLegandre:
        interface = new SymbolLegandreInterface(std::make_pair(data[1], data[2]), std::make_pair(data[3], data[4]),
                                                static_cast<SymbolLegandreOptions>(subTask));
        break;
    case AllTasks::SymbolJacobi:
        interface = new SymbolJacobiInterface(std::make_pair(data[1], data[2]), std::make_pair(data[3], data[4]),
                                                static_cast<SymbolLegandreOptions>(subTask));
        break;
    case AllTasks::TranspositionGroup:
        interface = new TranspositionGroupInterface(data[1], data[2], static_cast<TranspositionGroupOptions>(subTask), static_cast<ViewMode>(optional));
        break;
    case AllTasks::AlgebraStructures:
        interface = new AlgebraStructuresInterface(static_cast<AlgebraStructuresOptions>(subTask));
        break;
    case AllTasks::GroupProperties:
//        runGroupProperties(data[0], data[1], data[2], static_cast<GroupPropertiesOptions>(subTask));
        break;
    case AllTasks::Matrix:
        switch (static_cast<Set>(optional))
        {
        case Set::R:
            interface = new MatrixInterface<double>(std::make_pair(data[1], data[2]), static_cast<MatrixOptions>(subTask));
            break;
        case Set::Z:
            interface = new MatrixInterface<int>(std::make_pair(data[1], data[2]), static_cast<MatrixOptions>(subTask));
            break;
        case Set::Zn:
            interface = new MatrixInterface<Zn>(std::make_pair(data[1], data[2]), static_cast<MatrixOptions>(subTask));
            break;
        case Set::Zp:
            interface = new MatrixInterface<Zp>(std::make_pair(data[1], data[2]), static_cast<MatrixOptions>(subTask));
            break;
        case Set::C:
            interface = new MatrixInterface<Complex<double>>(std::make_pair(data[1], data[2]), static_cast<MatrixOptions>(subTask));
            break;
        case Set::Z_i:
            interface = new MatrixInterface<Complex<int>>(std::make_pair(data[1], data[2]), static_cast<MatrixOptions>(subTask));
            break;
        default:
            break;
        }
        break;
    case AllTasks::RingResidue:
        interface = new RingResidueInterface(data[1], data[2], static_cast<RingResidueOptions>(subTask), static_cast<ResidueType>(optional));
        break;
    case AllTasks::RingOfMembers:
        switch (static_cast<Set>(optional))
        {
        case Set::R:
            interface = new RingOfMembersInterface<double>(data[1], data[2], static_cast<RingOfMembersOptions>(subTask));
            break;
        case Set::Z:
            interface = new RingOfMembersInterface<int>(data[1], data[2], static_cast<RingOfMembersOptions>(subTask));
            break;
        case Set::Zn:
            interface = new RingOfMembersInterface<Zn>(data[1], data[2], static_cast<RingOfMembersOptions>(subTask));
            break;
        case Set::Zp:
            interface = new RingOfMembersInterface<Zp>(data[1], data[2], static_cast<RingOfMembersOptions>(subTask));
            break;
        case Set::C:
            interface = new RingOfMembersInterface<Complex<double>>(data[1], data[2], static_cast<RingOfMembersOptions>(subTask));
            break;
        case Set::Z_i:
            interface = new RingOfMembersInterface<Complex<int>>(data[1], data[2], static_cast<RingOfMembersOptions>(subTask));
            break;
        default:
            break;
        }
        break;
    case AllTasks::Complex:
        switch (static_cast<Set>(optional))
        {
        case Set::C:
            interface = new ComplexInterface<double>(data[1], data[2], static_cast<ComplexOptions>(subTask));
            break;            
        default:
            interface = new ComplexInterface<int>(data[1], data[2], static_cast<ComplexOptions>(subTask));
            break;
        }
        break;
    case AllTasks::BooleanFunction:
        interface = new BooleanFunctionInterface(data[1], data[2],
                static_cast<BooleanFunctionOptions>(subTask));
        break;
    case AllTasks::KeliTable:
        interface = new KeliTableInterface(data[1], data[2],
                static_cast<KeliTableOptions>(subTask));
        break;
    default:
        qDebug() << "Incorrect AllTasks type! Cannot catch it!";
        return;
    }

    for (int i = 0; i < data[0]; ++i)
    {
        interface->create();

        if (!mode) {
            tasks.push_back(interface->task());
            answers.push_back(interface->answer());
        }
        else
            tasksForTest.push_back(std::make_tuple(interface->task(), interface->answer(), SupCommands::Number, 0));
    }

    if (!mode)
    {
        descriptions.push_back(interface->description());
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }

    delete interface;
}
