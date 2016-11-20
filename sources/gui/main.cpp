#include "SceneWidget.h"

#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QLayout>

int main(int i_argc, char** i_argv)
{
    std::cout << "Wikipedia Sandbox" << std::endl;

    QApplication app(i_argc, i_argv);
    QMainWindow wnd;
    wnd.setContentsMargins(5, 5, 5, 5);

    wnd.setCentralWidget(new SceneWidget(&wnd));
    wnd.show();

    auto exit_code = app.exec();

    return exit_code;
}