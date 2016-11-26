#include "SceneWidget.h"

#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QStatusBar>
#include <QString>

int main(int i_argc, char** i_argv)
{
    std::cout << "Wikipedia Sandbox" << std::endl;

    QApplication app(i_argc, i_argv);
    QMainWindow wnd;
    wnd.setContentsMargins(5, 5, 5, 5);

    auto* p_scene_widget = new SceneWidget(&wnd);

    wnd.setCentralWidget(p_scene_widget);

    auto* p_status_bar = new QStatusBar;
    p_scene_widget->SetMouseMoveMessageDelegate([p_status_bar](const std::string& i_message) {p_status_bar->showMessage(QString::fromStdString(i_message)); });

    wnd.setStatusBar(p_status_bar);

    wnd.show();

    auto exit_code = app.exec();

    return exit_code;
}