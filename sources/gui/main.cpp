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
#include <QCheckBox>
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

    TClusterizationsPtr _ReadClusterizationsFromFiles(const std::string& i_directory_name, size_t i_number_of_files)
    {
        TClusterizationsPtr result = std::make_shared<std::vector<GraphClusterization::TClusterMap>>();
        for (size_t i = 0; i < i_number_of_files; ++i)
        {
            const std::string clusterization_file_full_name = i_directory_name + "/cl_" + std::to_string(i) + ".csv";
            std::ifstream input_file_stream(clusterization_file_full_name);
            if (!input_file_stream)
                result->push_back({});
            else
                result->push_back(GraphsIO::ReadClusterizationFromStream(input_file_stream));
        }
        return result;
    }
}

int main(int i_argc, char** i_argv)
{
    std::cout << "Wikipedia Sandbox" << std::endl;

    system("pause");

    QApplication app(i_argc, i_argv);
    QMainWindow wnd;
    wnd.setContentsMargins(5, 5, 5, 5);
    

    if (i_argc < 2)
        return 1;

    std::string directory_name(i_argv[1]);
    auto p_graph = _ReadGraphFromFile(directory_name);
    auto p_topology = _ReadTopologyFromFile(directory_name);
    auto p_description = _ReadDescriptionFromFile(directory_name);

    TClusterizationsPtr p_clusterization;
    if (i_argc > 2)
        p_clusterization = _ReadClusterizationsFromFiles(directory_name, std::stoi(i_argv[2]));

    auto* p_scene_widget = new SceneWidget(&wnd, std::make_unique<UiController>(p_graph, p_topology, p_description, p_clusterization));

    {
        auto* p_scene_and_panel = new QWidget;
        auto* p_hor_layout = new QHBoxLayout;
        p_scene_and_panel->setLayout(p_hor_layout);
        
        p_hor_layout->addWidget(p_scene_widget, 1);

        auto* p_right_panel_widget = new QWidget;

        auto* p_right_panel = new QVBoxLayout;
        auto* p_checkbox_draw_edges = new QCheckBox;
        p_checkbox_draw_edges->setText("Draw links always");
        p_scene_widget->SetDrawEdges(p_checkbox_draw_edges->isChecked());
        QObject::connect(p_checkbox_draw_edges, &QCheckBox::stateChanged, [p_scene_widget, p_checkbox_draw_edges](int) 
        {
            p_scene_widget->SetDrawEdges(p_checkbox_draw_edges->isChecked());
        });
        p_right_panel->addWidget(p_checkbox_draw_edges);
        p_right_panel_widget->setLayout(p_right_panel);

        p_hor_layout->addWidget(p_right_panel_widget);

        wnd.setCentralWidget(p_scene_and_panel);
    }


    auto* p_status_bar = new QStatusBar;
    p_scene_widget->SetMouseMoveMessageDelegate([p_status_bar](const std::string& i_message) {p_status_bar->showMessage(QString::fromStdString(i_message)); });

    wnd.setStatusBar(p_status_bar);
    wnd.resize(QDesktopWidget().availableGeometry(&wnd).size() * 0.7);
    wnd.show();

    auto exit_code = app.exec();

    return exit_code;
}