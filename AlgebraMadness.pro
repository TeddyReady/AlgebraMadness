#####################################################
#########      PROJECT    SUBMODULES       ##########
#####################################################
include(libs/qtmathjax/qtmathjax.pri)
include(libs/mathlib/mathlib.pri)

#####################################################
#########      PROJECT   CONFIGURATION     ##########
#####################################################
QT          += core gui webkitwidgets widgets xml
TARGET       = Algebra_Madness
TEMPLATE     = app
CONFIG      += c++17
INCLUDEPATH += $$PWD/interface $$PWD/sources
DEFINES     += QT_DEPRECATED_WARNINGS
LIBS        += -L/usr/local/lib -lpoppler-qt5

#####################################################
#########       PROJECT    SOURCES         ##########
#####################################################
HEADERS  +=                                         \
    sources/codeeditor.h                            \
    sources/dialogmanual.h                          \
    sources/dialogbase.h                            \
    sources/dialoglatexprinter.h                    \
    sources/dialogtesttimer.h                       \
    sources/dialogresults.h                         \
    sources/environment.h                           \
    sources/generatorwindow.h                       \
    sources/pdfviewer.h                             \
    sources/registerspainter.h                      \
    sources/registerwidget.h                        \
    sources/taskitem.h                              \
    sources/testmode.h                              \
    sources/dialogfontsize.h                        \
    sources/include.h                               \
    sources/turingmachine.h                         \
                                                    \
    interface/ibooleanfunction.h                    \
    interface/ikelitable.h                          \
    interface/ialgebrastructures.h                  \
    interface/ieulerfunction.h                      \
    interface/imatrix.tpp                           \
    interface/imebiusfunction.h                     \
    interface/interface.h                           \
    interface/isymbollegandre.h                     \
    interface/itranspositiongroup.h                 \
    interface/icomplex.tpp                          \
    interface/iringofmembers.tpp                    \
    interface/iringresidue.h                        \

SOURCES  +=                                         \
    sources/codeeditor.cpp                          \
    sources/dialogmanual.cpp                        \
    sources/dialogbase.cpp                          \
    sources/dialoglatexprinter.cpp                  \
    sources/dialogtesttimer.cpp                     \
    sources/dialogresults.cpp                       \
    sources/main.cpp                                \
    sources/generatorwindow.cpp                     \
    sources/pdfviewer.cpp                           \
    sources/registerspainter.cpp                    \
    sources/registerwidget.cpp                      \
    sources/taskitem.cpp                            \
    sources/testmode.cpp                            \
    sources/dialogfontsize.cpp                      \
    sources/turingmachine.cpp                       \
                                                    \
    interface/ibooleanfunction.cpp                  \
    interface/ikelitable.cpp                        \
    interface/ialgebrastructures.cpp                \
    interface/ieulerfunction.cpp                    \
    interface/imebiusfunction.cpp                   \
    interface/isymbollegandre.cpp                   \
    interface/itranspositiongroup.cpp               \
    interface/icomplex.cpp                          \
    interface/imatrix.cpp                           \
    interface/iringofmembers.cpp                    \
    interface/iringresidue.cpp                      \

FORMS  +=                                           \
    sources/dialogbase.ui                           \
    sources/dialoglatexprinter.ui                   \
    sources/dialogmanual.ui                         \
    sources/dialogtesttimer.ui                      \
    sources/dialogresults.ui                        \
    sources/generatorwindow.ui                      \
    sources/testmode.ui                             \
    sources/dialogfontsize.ui                       \

RESOURCES +=                                        \
    resources/resourses.qrc                         \
