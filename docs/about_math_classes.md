# О добавлении математических структур и отладки в приложение AlgebraMadness 
## Введение
Для добавления своих математических модулей и классов небходимо придерживаться определенного *алгоритма* и знать некоторые *особенности приложения*.

Для отрисовки самих математических структур, в приложении используется **MathJax** - этакий малость урезанный LaTeX, позволящий преобразовывать LaTeX код в HTML страницу, которую и отображает приложение.

В силу того, что объем задач - большой и самый разнообразный, были реализована система классов и их интерфейсов, благодаря которой приложению совершенно необязательно постоянно менять свою структуру из за новых классов, ведь она опирается на конкретный шаблон, реализованный в классе-интерфейсе.

## Алгоритм
### Глава 1. Создание математического модуля
Для создания модуля, следует познакомится со структурой проекта:

    AlgebraMadness
    ├── AlgebraMadness.pro  - проектный файл
    ├── docs                - директория с документацией
    ...
    ├── interface           - каталог с классами-интерфейсами
    ├── libs                - директория с подключаемыми библиотеками
    │   ├── mathlib         - библиотека с математическими модулями
    │   └── qtmathjax       - библиотека с MathJax, адаптированная для Qt
    ├── LICENSE.md
    ├── README.md
    ├── resources           - каталог с ресурсами проекта (иконки, xml и прочее)
    └── sources             - основные исходники проекта

Отталкиваясь от данной структуры, видно, что реализованные математические модули хранятся в каталоге **./libs/mathlib**, а их интерфейсы в **./interface**.

Создать математический модуль - алгоритмичная задача. В процессе учитывайте, что вам нужно создать *все необходимые методы*, которые пригодятся для генерации задач и вычислении ответов.

Для примера рассмотрим *eulerfunction.h*:

    #ifndef EULERFUNCTION_H
    #define EULERFUNCTION_H
    #include "basemath.h"

    class EulerFunction {
    private:
        int number;

    public:
        explicit EulerFunction();
        explicit EulerFunction(int number);

        int solve() const;
        void set(int number);
        int get()   const;
    };

    #endif // EULERFUNCTION_H

Вот его реализация:

    #include "eulerfunction.h"

    EulerFunction::EulerFunction() : number(1) {}
    EulerFunction::EulerFunction(int number) : number(number)
    {
        if (number < 0)
        {
            std::cerr << "ERROR in eulerfunction.cpp: Number cannot be lower zero!" << std::endl;
            this->number = 1;
        }
    }

    int EulerFunction::solve() const 
    {
        auto data = decompositionToSimple(number);
        int result = 1;

        for (size_t i = 0; i < data.size(); ++i)
            result *= fastPower(data[i].first, data[i].second) - fastPower(data[i].first, data[i].second - 1);

        return result;
    }

    void EulerFunction::set(int number) { this->number = number; }

    int EulerFunction::get() const { return number; }

В итоге и получаем, что класс полностью оснащен всем необходимым:
- Различные конструкторы для инициализации задачи;
- Метод *set()* для инициализации задачи;
- Метод *get()* для получения текущей задачи;
- И конечно же - *solve()* выдаст нам ответ на задачу.

Все просто!

### Глава 2. Создание интерфейса
Отлично, класс создан, теперь необходимо создать интерфейс к этому классу. Каждый класс интерфейс должен быть наследником класса **TaskInterface**, который и определяет те методы, которые будут использоваться у **ЛЮБОГО** класса, реализованного в задаче.

Рассмотрим *interface.h*:

    #ifndef INTERFACE_H
    #define INTERFACE_H
    #include "include.h"

    /* Iterface for math classes */
    class TaskInterface {
    public:
        virtual ~TaskInterface() = default;

        virtual void create() = 0;
        virtual QString description() = 0;
        virtual QString task() = 0;
        virtual QString answer() = 0;
    };

    #endif // INTERFACE_H

Ничего лишнего, все самое необходимое. Нам интересны 4 чисто виртуальных метода (подлежат **ОБЯЗАТЕЛЬНОМУ переопределению!!!**):
- *create()* предполагает, что в нем будет происходить генерация задачи;
- в *description()* описывается задача на русском языке, понятная студенту;
- в *task()* формируется задача на математическом языке, а именно средствами **MathJax**;
- и разумеется, *answer()*, в нем мы предоставляем ответ к нашему заданию;

Для удобства, наименование класса интерфейса совпадает с математическим модулем, однако в начале приписывается буква **'i'**:

    eulerfunction.cpp -> ieulerfunction.cpp

По классике, рассмотрим пример с функцией Эйлера, класс ieulerfunction.h:

    #ifndef EULERFUNCTIONINTERFACE_H
    #define EULERFUNCTIONINTERFACE_H
    #include "interface.h"
    #include "eulerfunction.h"
    #include "basemath.h"

    enum class EulerFunctionOptions
    {
        Default, Primes,
        PrimesDegrees, MultiplyPrimes,
        MultiplyPrimesDegrees
    };

    class EulerFunctionInterface : public TaskInterface {
    private:
        int minNum, maxNum;
        EulerFunctionOptions option;
        QRandomGenerator *gen;
        EulerFunction function;

    public:
        explicit EulerFunctionInterface(int minNum, int maxNum, EulerFunctionOptions option);
        ~EulerFunctionInterface();

        void create();
        QString description();
        QString task();
        QString answer();
    };

    #endif // EULERFUNCTIONINTERFACE_H

Интересно заметить, что у каждого такого интерфейса появляется побочная структура - *enum class SomeOptions*, которая содержит в себе типы различных заданий. По этим различным типам заданий, привязанных к одному математическому классу, будет происходить генерация, например:

    Default - просто вычислить функцию Эйлера;
    Primes  - вычислить ФЭ, где элементы всегда простые;
    PrimesDegrees - аналогично предыдущему, однако элементы - простые в степени;
    MultiplyPrimes - произвдение простых;
    MultiplyPrimesDegrees - произведение простых в степени;

Будьте внимательны! Порядок, в котором вы расположите опции, имеет значение! Дальше это будет понятно при создании диалогового окна.

Ну и реализация:

    #include "ieulerfunction.h"

    EulerFunctionInterface::EulerFunctionInterface(int minNum, int maxNum, EulerFunctionOptions option)
        : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
    EulerFunctionInterface::~EulerFunctionInterface() { gen = nullptr; }

    void EulerFunctionInterface::create()
    {
        std::vector<std::pair<int, int>> data;
        bool isCorrect = false;

        switch (option)
        {
        case EulerFunctionOptions::Default:
            function.set(gen->bounded(minNum, maxNum));
            break;

        case EulerFunctionOptions::Primes:
            do {
                function.set(gen->bounded(minNum, maxNum));
            } while (!isPrime(function.get()));
            break;

        case EulerFunctionOptions::PrimesDegrees:
            do {
                function.set(gen->bounded(minNum, maxNum));
                data = decompositionToSimple(function.get());
            } while (data.size() != 1 || data[0].second == 1);
            break;

        case EulerFunctionOptions::MultiplyPrimes:
            do {
                function.set(gen->bounded(minNum, maxNum));

                data = decompositionToSimple(function.get());
                if (data.size() == 1) continue;

                isCorrect = true;
                for (size_t i = 0; i < data.size(); ++i)
                {
                    if (data[i].second != 1 || !isPrime(data[i].first))
                    {
                        isCorrect = false;
                        break;
                    }
                }

            } while (not isCorrect);
            break;

        case EulerFunctionOptions::MultiplyPrimesDegrees:
            do {
                function.set(gen->bounded(minNum, maxNum));

                data = decompositionToSimple(function.get());
                if (data.size() == 1) continue;

                isCorrect = true;
                for (size_t i = 0; i < data.size(); ++i)
                {
                    if (data[i].second == 1)
                    {
                        isCorrect = false;
                        break;
                    }
                }

            } while (not isCorrect);
            break;
        }
    }

    QString EulerFunctionInterface::description()
    {
        return QString("Вычислите функцию Эйлера");
    }

    QString EulerFunctionInterface::task()
    {
        return QString("\\varphi(" + QString::number(function.get()) + ")=~?");
    }

    QString EulerFunctionInterface::answer()
    {
        return QString::number(function.solve());
    }

Время внедрять наши классы в проект!

# Глава 3. Внедрение модуля в приложение
В этом этапе присутствуют три основных элемента: регистрация самого класса, создание диалогового окна и добавление в цикл генерации.

Начнем с регистрации:

1. Откройте файл *./resources/xml/tasks.xml*, он выглядит подобным образом:

        <?xml version="1.0" encoding="utf-8"?>
        <list>
            <task name="Алгебраические Структуры"   type="on"></task>
            <task name="Булевые Функции"            type="on"></task>
            <task name="Группы и их свойства"       type="off"></task>
            <task name="Группа Перестановок"        type="on"></task>
            <task name="Кольцо Вычетов"             type="on"></task>
            <task name="Кольцо Многочленов"         type="on"></task>
            <task name="Комплексные Числа"          type="on"></task>
            <task name="Матрицы"                    type="on"></task>
            <task name="Таблица Кэлли"              type="on"></task>
            <task name="Символ Лежандра"            type="on"></task>
            <task name="Символ Якоби"               type="on"></task>
            <task name="Функция Мёбиуса"            type="on"></task>
            <task name="Функция Эйлера"             type="on"></task>
        </list>

2. Добавьте свой класс:

        <?xml version="1.0" encoding="utf-8"?>
        <list>


            <task name="Алгебраические Структуры"   type="on"></task>
            <task name="Булевые Функции"            type="on"></task>
            ...
            <task name="Your Class"                 type="on"></task>
            ...
            <task name="Функция Мёбиуса"            type="on"></task>
            <task name="Функция Эйлера"             type="on"></task>
        </list>

3. Опция *"type"* - либо исключает вашу задачу из списка для генерации, либо включает. Доступные варианты - *"on"* и *"off"*.

4. Для понимания: данный файл парсится в классе ./sources/*generatorwindow.cpp*, функцией **GeneratorWindow::uploadListWidget()**. 
5. Теперь нужно добавить наш новый модуль в *enum class AllTasks*, по которому он и будет распознаваться. Расположен в ./sources/*environment.h*
   
        /* All task types in Algebra Madness */
        enum class AllTasks {
            EulerFunction       ,
            MebiusFunction      ,
            SymbolLegandre      ,
            SymbolJacobi        ,
            TranspositionGroup  ,
            Matrix              ,
            AlgebraStructures   ,
            GroupProperties     ,
            RingResidue         ,
            RingOfMembers       ,
            Complex             ,
            BooleanFunction     ,
            KeliTable           ,
        };

    Добавим свой YourClass:

        /* All task types in Algebra Madness */
        enum class AllTasks {
            EulerFunction       ,
            MebiusFunction      ,
            ...
            YourClass
            ...
            BooleanFunction     ,
            KeliTable           ,
        };

Теперь создадим форму для диалогового окна, для этого используем файл ./sources/*dialogbase.cpp*. Нас интересуют **4 функции**: *GenWidget::loadSettings()*, *DialogBase::uploadUI()*, *DialogBase::isHaveMoreGens()* и *BaseWidget::setExoticOptions()*. По порядку:

- в *DialogBase::uploadUI()* устанавливаются скелет вашего окна:

        void DialogBase::uploadUI()
        {
            ...
            switch (task) {
            case AllTasks::EulerFunction:
                addItem(Gen);
                addItem(Base, "По умолчанию");
                addItem(Base, "Простое");
                addItem(Base, "Степень простого");
                addItem(Base, "Произведение взаимно простых");
                addItem(Base, "Произведение степеней простых");
                break;
            ...
        }

    *Gen* создает виджет для установки значений, а *Base* виджет-чекбокс для выбора генерируемых заданий. 
    
    **ВНИМАНИЕ!** Задания установлены ровно в том порядке, в котором вы указывали в *SomeOptions* в вашем классе интерфейсе.

- в *DialogBase::isHaveMoreGens()* устанавливаются типы задач, которые требуют как минимум двух виджетов *Gen*:

        bool DialogBase::isHaveMoreGens()
        {
            switch (task) {
            case AllTasks::SymbolLegandre:
            case AllTasks::SymbolJacobi:
                return true;
        
            default:
                return false;
            }
        }

- в *GenWidget::loadSettings()* происходит предустановка значений в данных виджетах:

            void GenWidget::loadSettings(AllTasks task, const QString &optionName)
            {
                switch (task) {
                case AllTasks::EulerFunction:
                case AllTasks::MebiusFunction:
                    sbMin->setRange(1, 100);  // нижняя граница
                    sbMax->setRange(1, 9999); // верхняя граница
                    sbMin->setValue(1);       // текущее значение слева
                    sbMax->setValue(100);     // текущее значение справа
                    return;
                    ...
            }

- наконец *BaseWidget::setExoticOptions()*. Здесь указывается обработка дополнительных опций, требуемых некоторыми задачами, например матрицами. Матрицы могут быть различных типов - кольцо многочленов, комплексные и тд. Для установки своего ExoticOption добавьте его в файле **environment.h**. Расмотрим эту функцию с опциями по умолчанию, и в случае матриц:

     	void BaseWidget::setExoticOptions(const ExoticOption &type)
	    {
            switch (type) {
            ...
            case ExoticOption::MatrixDefault:
                exoticOption = static_cast<int>(Set::N);
                pb = new QPushButton("Выбрать алгебраическую структуру...", this);
                pb->setMenu(new QMenu(pb));
                pb->menu()->addAction(new QAction("Действительные", pb->menu()));
                pb->menu()->addAction(new QAction("Целые", pb->menu()));
                pb->menu()->addAction(new QAction("Кольцо вычетов", pb->menu()));
                pb->menu()->addAction(new QAction("Поле вычетов", pb->menu()));
                pb->menu()->addAction(new QAction("Комплексные", pb->menu()));
                pb->menu()->addAction(new QAction("Целые Гауссовы", pb->menu()));
                pb->setDisabled(true);
                layout()->addWidget(pb);
                connect(cb, &QCheckBox::clicked, [&](){
                    if (cb->isChecked()) pb->setEnabled(true);
                    else pb->setDisabled(true);
                    sb->setDisabled(true);
                    sb->setValue(0);
                });
                connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
                    pb->setText("Действительные");
                    exoticOption = static_cast<int>(Set::R);
                    sb->setEnabled(true);
                    sb->setValue(1);
                });
                connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
                    pb->setText("Целые");
                    exoticOption = static_cast<int>(Set::Z);
                    sb->setEnabled(true);
                    sb->setValue(1);
                });
                connect(pb->menu()->actions().at(2), &QAction::triggered, [&](){
                    pb->setText("Кольцо вычетов");
                    exoticOption = static_cast<int>(Set::Zn);
                    sb->setEnabled(true);
                    sb->setValue(1);
                });
                connect(pb->menu()->actions().at(3), &QAction::triggered, [&](){
                    pb->setText("Поле вычетов");
                    exoticOption = static_cast<int>(Set::Zp);
                    sb->setEnabled(true);
                    sb->setValue(1);
                });
                connect(pb->menu()->actions().at(4), &QAction::triggered, [&](){
                    pb->setText("Комплексные");
                    exoticOption = static_cast<int>(Set::C);
                    sb->setEnabled(true);
                    sb->setValue(1);
                });
                connect(pb->menu()->actions().at(5), &QAction::triggered, [&](){
                    pb->setText("Целые Гауссовы");
                    exoticOption = static_cast<int>(Set::Z_i);
                    sb->setEnabled(true);
                    sb->setValue(1);
                });
                break;
            ...

            case ExoticOption::None:
                connect(cb, &QCheckBox::clicked, [&](){
                    if (cb->isChecked()) {
                        sb->setEnabled(true);
                        sb->setValue(1);
                    } else {
                        sb->setDisabled(true);
                        sb->setValue(0);
                    }
                });
                break;
            }
        }

    В будущем планируется разумеется оптимизировать код, для более быстрого внедрения различных классов.

Ну и наконец, последний этап, добавим класс в цикл генерации. За генерацию ответственнем главный класс - *GeneratorWindow*.

    void GeneratorWindow::runTaskManager(const QString &task, bool closeMode)
	{
	    if (!ui->mainLayout->isEmpty() && !closeMode)
	        delete ui->mainLayout->takeAt(0)->widget();
	

        /* Создаем диалоговое окно в зависимости от типа задания */
	    DialogBase *window;
	    if (task == "Группа Перестановок")
	        window = new DialogBase(AllTasks::TranspositionGroup, closeMode, this);
	    else if (task == "Функция Эйлера")
	        window = new DialogBase(AllTasks::EulerFunction, closeMode, this);
        ...

        /* Добавляем свой класс */
        else if (task == "Your Class") // имя, которое вы задали в task.xml
	        window = new DialogBase(AllTasks::YourClass, closeMode, this); // тип, который вы задали в AllTasks

        ...

        /* Заполняем массив с заданиями/ответами/описаниями и отображаем на экран */
        for (int i = 0; i < data[0]; ++i) {
	        interface->create();
	
	        if (!mode) {
	            tasks.push_back(interface->task());
	            answers.push_back(interface->answer());
	        }
	        else
	        {
	            tasksForTest.push_back(std::make_tuple(interface->task(), interface->answer(),
	                                                   SupCommands::Number, 0));
	        }
	    }
	    if (!mode) {
	        descriptions.push_back(interface->description());
	        this->tasks.push_back(std::move(tasks));
	        isReadyRender();
	    } delete interface;
    }

## Заключение
Таким образом мы с вами создали примитивный математический модуль и успешно внедрили его в приложение. В более сложных случаях, смотрите исходный код ранее реализованных модулей (матрицы, кольцо вычетов, группы перестановок, алгебраические структуры). Они в каком то смысле уникальны, и решения созданные для них, могут быть полезны вам, при создании и внедрении собственных модулей!