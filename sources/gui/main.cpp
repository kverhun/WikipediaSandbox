#include "SceneWidget.h"

#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QStatusBar>

int main(int i_argc, char** i_argv)
{
    std::cout << "Wikipedia Sandbox" << std::endl;

    QApplication app(i_argc, i_argv);
    QMainWindow wnd;
    wnd.setContentsMargins(5, 5, 5, 5);

    wnd.setCentralWidget(new SceneWidget(&wnd));

    auto* p_status_bar = new QStatusBar;
    p_status_bar->showMessage("test message");
    wnd.setStatusBar(p_status_bar);

    wnd.show();

    auto exit_code = app.exec();

    return exit_code;
}