#pragma once
#ifndef STRONGLY_CONNECTED_COMPONENTS_DRAW_H
#define STRONGLY_CONNECTED_COMPONENTS_DRAW_H
#include "Graph.h"
#include <list>
#include <QWidget>
#include "ui_StronglyConnectedComponentsDraw.h"
#include <QPainter>


class StronglyConnectedComponentsDraw : public QWidget
{
	Q_OBJECT

public:
	StronglyConnectedComponentsDraw(QWidget *parent = nullptr);
	~StronglyConnectedComponentsDraw() override;

	void OnButtonPush();
	void CreateNewGraph(const Graph& graph, const std::list<std::list<int>>& stronglyConnectedComponents);
	std::tuple<int, int, int, int> archDrawXYOfSet(const std::pair<int, int>& firstNode,
		const std::pair<int, int>& secondNode);
	void paintEvent(QPaintEvent* event) override;
	
signals:
	void ReturnButtonClicked();
	
private:
	Ui::StronglyConnectedComponentsDrawClass ui;
	std::list<std::list<int>> m_stronglyConnectedComponents;
	Graph m_graph;
	constexpr static int NODE_SIZE{ 50 };
};

#endif //STRONGLY_CONNECTED_COMPONENTS_DRAW_H
