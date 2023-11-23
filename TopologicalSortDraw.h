#pragma once
#include "Graph.h"
#include <QWidget>
#include "ui_TopologicalSortDraw.h"

class TopologicalSortDraw : public QWidget
{
	Q_OBJECT

public:
	TopologicalSortDraw(QWidget *parent = nullptr);

	void SetGraph(const Graph& graph);
	void SetSortedVector(const std::vector<int>& sortedVector);


	
	void Draw(const std::vector<Node>& initialNodes);
	void OnButtonPush();

	void paintEvent(QPaintEvent* e) override;
	~TopologicalSortDraw() override;


signals:
	void ReturnButtonClicked();

private:
	Graph CreateGraph(const std::vector<Node>& nodes, const std::vector<std::pair<std::vector<Node>, std::vector<Arch>>>& newGraphs);
	void DeleteDuplicates(std::vector<Node>& nodes);
	std::pair<std::vector<Node>, std::vector<Arch>> CreateGraph(const std::vector<std::pair<std::optional<int>, int>>& arches);
	std::vector<std::pair<std::vector<Node>, std::vector<Arch>>> CreateNewGraphs(size_t size);
	void SaveSortedVectorOnDisk();

	Ui::TopologicalSortDrawClass ui;
	Graph m_graph;
	std::vector<int> m_sortedVector;
};
