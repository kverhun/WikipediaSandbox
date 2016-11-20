#include <iostream>

#include <QApplication>
#include <QMainWindow>

int main(int i_argc, char** i_argv)
{
    std::cout << "Wikipedia Sandbox" << std::endl;

    QApplication app(i_argc, i_argv);
    QMainWindow wnd;
    wnd.show();

    auto exit_code = app.exec();

    return exit_code;
}