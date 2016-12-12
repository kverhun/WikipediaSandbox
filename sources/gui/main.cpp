#include "SceneWidget.h"
#include "UiController.h"

#include "../Libraries/Graphs/Graph.h"
#include "../Libraries/GraphsIO/GraphIO.h"

#include <iostream>
#include <fstream>
#include <memory>

#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QStatusBar>
#include <QString>
#include <QDesktopWidget>

namespace
{
    const std::string g_graph_file_name = "Graph.csv";
    const std::string g_topology_file_name = "Topology.csv";
    const std::string g_description_file_name = "Description.csv";

    std::shared_ptr<Graphs::Graph> _ReadGraphFromFile(const std::string& i_directory_name)
    {
        const std::string graph_full_file_name = i_directory_name + "/" + g_graph_file_name;

        std::ifstream input_file_stream(graph_full_file_name);
        auto p_graph = GraphsIO::ReadGraphFromStream(input_file_stream);
        return std::shared_ptr<Graphs::Graph>(p_graph.release());
    }

    std::shared_ptr<Graphs::TGraphTopology> _ReadTopologyFromFile(const std::string& i_directory_name)
    {
        const std::string topology_full_file_name = i_directory_name + "/" + g_topology_file_name;
        std::ifstream input_file_stream(topology_full_file_name);
        if (!input_file_stream)
            return nullptr;
        return std::make_shared<Graphs::TGraphTopology>(GraphsIO::ReadGraphTopologyFromStream(input_file_stream));
    }

    std::shared_ptr<std::map<Graphs::Graph::TVertex, std::string>> _ReadDescriptionFromFile(const std::string& i_directory_name)
    {
        const std::string description_full_name = i_directory_name + "/" + g_description_file_name;
        std::ifstream input_file_stream(description_full_name);
        if (!input_file_stream)
            return nullptr;
        return std::make_shared<std::map<Graphs::Graph::TVertex, std::string>>(GraphsIO::ReadGraphVerticesDescriptionFromStream(input_file_stream));
    }
}

int main(int i_argc, char** i_argv)
{
    std::cout << "Wikipedia Sandbox" << std::endl;
    QApplication app(i_argc, i_argv);
    QMainWindow wnd;
    wnd.setContentsMargins(5, 5, 5, 5);
    wnd.resize(QDesktopWidget().availableGeometry(&wnd).size() * 0.7);

    if (i_argc < 2)
        return 1;

    std::string directory_name(i_argv[1]);
    auto p_graph = _ReadGraphFromFile(directory_name);
    auto p_topology = _ReadTopologyFromFile(directory_name);
    auto p_description = _ReadDescriptionFromFile(directory_name);

    auto* p_scene_widget = new SceneWidget(&wnd, std::make_unique<UiController>(p_graph, p_topology, p_description));

    wnd.setCentralWidget(p_scene_widget);

    auto* p_status_bar = new QStatusBar;
    p_scene_widget->SetMouseMoveMessageDelegate([p_status_bar](const std::string& i_message) {p_status_bar->showMessage(QString::fromStdString(i_message)); });

    wnd.setStatusBar(p_status_bar);

    wnd.show();

    auto exit_code = app.exec();

    return exit_code;
}