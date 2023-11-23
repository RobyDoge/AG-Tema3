#include "TopologicalSortDraw.h"
#include "DesenareGrafuri.h"

#include <QLabel>
#include <QVBoxLayout>
#include <set>
#include <fstream>

TopologicalSortDraw::TopologicalSortDraw(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.buttonReturn, &QPushButton::clicked, this, &TopologicalSortDraw::OnButtonPush);
}

void TopologicalSortDraw::SetGraph(const Graph& graph)
{
	m_graph = graph;
}

void TopologicalSortDraw::SetSortedVector(const std::vector<int>& sortedVector)
{
	m_sortedVector = sortedVector;
}

void TopologicalSortDraw::Draw(const std::vector<Node>& initialNodes)
{
	if (!m_sortedVector.empty())
	{
		SaveSortedVectorOnDisk();
		const Graph newGraph(CreateGraph(initialNodes, CreateNewGraphs(initialNodes.size())));
		m_graph = newGraph;
	}
	update();
}

void TopologicalSortDraw::OnButtonPush()
{
	emit ReturnButtonClicked();
}

TopologicalSortDraw::~TopologicalSortDraw() = default;

void TopologicalSortDraw::paintEvent(QPaintEvent* e)
{
	if (m_sortedVector.empty())
	{
		auto label = new QLabel(this);
		label->setText("!In Graf Sunt Prezente Cicluri!");
		label->setAlignment(Qt::AlignCenter);
		label->setFixedSize(200, 100);
		auto layout = new QVBoxLayout(this);
		layout->addWidget(label);
		setLayout(layout);
		return;
	}
	QPainter p(this);
	std::vector<Node> noduri = m_graph.getNodes();
	for (Node& n : noduri)
	{
		QRect r(n.getX() - 10, n.getY() - 10, 20, 20);
		p.drawEllipse(r);
		QString s;
		s.setNum(n.getValue());
		p.drawText(r, Qt::AlignCenter, s);
	}

	std::vector<Arch> arches = m_graph.getArches();
	for (Arch& a : arches)
	{
		p.drawLine(a.getFirstNode().getX(), a.getFirstNode().getY(),
		           a.getSecondNode().getX(), a.getSecondNode().getY());

		const double angle = std::atan2(a.getSecondNode().getY() - a.getFirstNode().getY(),
		                                a.getSecondNode().getX() - a.getFirstNode().getX());
		constexpr double arrowLength = 10.0;

		const double arrowX1 = a.getSecondNode().getX() - arrowLength * std::cos(angle - M_PI / 6);
		const double arrowY1 = a.getSecondNode().getY() - arrowLength * std::sin(angle - M_PI / 6);
		const double arrowX2 = a.getSecondNode().getX() - arrowLength * std::cos(angle + M_PI / 6);
		const double arrowY2 = a.getSecondNode().getY() - arrowLength * std::sin(angle + M_PI / 6);


		QPolygonF arrowhead;
		arrowhead.append(QPointF(a.getSecondNode().getX(), a.getSecondNode().getY()));
		arrowhead.append(QPointF(arrowX1, arrowY1));
		arrowhead.append(QPointF(arrowX2, arrowY2));

		p.setBrush(Qt::black);
		p.setPen(Qt::black);
		p.drawPolygon(arrowhead);
	}
}


Graph TopologicalSortDraw::CreateGraph(const std::vector<Node>& nodes,
                                       const std::vector<std::pair<std::vector<Node>, std::vector<Arch>>>& newGraphs)
{
	Graph graph;
	for (const auto node : nodes)
	{
		graph.addNode(node);
	}

	for (const auto& [newNodes, newArches] : newGraphs)
	{
		for (auto arch : newArches)
		{
			graph.addArch({
				              {
					              arch.getFirstNode().getValue(),
					              graph.getNodes()[arch.getFirstNode().getValue()].getX(),
					              graph.getNodes()[arch.getFirstNode().getValue()].getY()
				              },
				              {
					              arch.getSecondNode().getValue(),
					              graph.getNodes()[arch.getSecondNode().getValue()].getX(),
					              graph.getNodes()[arch.getSecondNode().getValue()].getY()
				              }
			              }, true);
		}
	}

	return graph;
}

void TopologicalSortDraw::DeleteDuplicates(std::vector<Node>& nodes)
{
	std::vector<Node> newNodes;
	newNodes.push_back(nodes[0]);
	for (int i = 1; i < nodes.size(); i++)
	{
		if (nodes[i].getValue() == nodes[i - 1].getValue())
		{
			continue;
		}
		newNodes.push_back(nodes[i]);
	}
	nodes = newNodes;
}

std::pair<std::vector<Node>, std::vector<Arch>> TopologicalSortDraw::CreateGraph(
	const std::vector<std::pair<std::optional<int>, int>>& arches)
{
	std::pair<std::vector<Node>, std::vector<Arch>> subGraph;
	for (const auto& [firstNode, secondNode] : arches)
	{
		if (firstNode == std::nullopt)
		{
			continue;
		}
		if (firstNode.has_value())
		{
			subGraph.first.emplace_back(firstNode.value(), 0, 0);
			subGraph.first.emplace_back(secondNode, 0, 0);
			Arch aux({firstNode.value(), 0, 0}, {secondNode, 0, 0});
			subGraph.second.emplace_back(aux);
		}
	}
	DeleteDuplicates(subGraph.first);
	return subGraph;
}

std::vector<std::pair<std::vector<Node>, std::vector<Arch>>> TopologicalSortDraw::CreateNewGraphs(size_t size)
{
	std::vector<std::pair<std::vector<Node>, std::vector<Arch>>> nodesAndArches;
	std::vector<std::pair<std::optional<int>, int>> newArch;
	for (auto& node : m_sortedVector)
	{
		bool elementAdded = false;
		if (newArch.empty())
		{
			newArch.emplace_back(std::nullopt, node);
			continue;
		}
		for (auto& [firstNode, secondNode] : newArch)
		{
			if (m_graph.GetAdjacencyMatrix()[secondNode][node] == 1)
			{
				newArch.emplace_back(secondNode, node);
				elementAdded = true;
				break;
			}
		}
		if (!elementAdded)
		{
			nodesAndArches.emplace_back(CreateGraph(newArch));
			newArch.clear();
			newArch.emplace_back(std::nullopt, node);
		}
	}
	nodesAndArches.emplace_back(CreateGraph(newArch));
	return nodesAndArches;
}

void TopologicalSortDraw::SaveSortedVectorOnDisk()
{
	std::ofstream fileOutput("Sorted Vector.txt");
	for (const auto aux : m_sortedVector)
	{
		fileOutput << aux << " ";
	}
	fileOutput.close();
}
