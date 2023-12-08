#include "StronglyConnectedComponentsDraw.h"
#include <QPushButton>

StronglyConnectedComponentsDraw::StronglyConnectedComponentsDraw(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.buttonReturn, &QPushButton::clicked, this, &StronglyConnectedComponentsDraw::OnButtonPush);
}

StronglyConnectedComponentsDraw::~StronglyConnectedComponentsDraw()= default;

void StronglyConnectedComponentsDraw::OnButtonPush()
{
	emit ReturnButtonClicked();
}

void StronglyConnectedComponentsDraw::CreateNewGraph(const Graph& graph,
	const std::list<std::list<int>>& stronglyConnectedComponents)
{
	m_graph=graph;
	m_stronglyConnectedComponents=stronglyConnectedComponents;
	update();
}

void StronglyConnectedComponentsDraw::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	//list<elemente componente, <x,y>>
	std::vector < std::pair<std::list<int>, std::pair<int, int>>> stronglyConnectedComponentsAndCoordinates;
	constexpr QPoint STARTING_POSITION{ 10,30 };
	int xOffSet{ 0 };
	int yOffSet{ 0 };
	int counter{ 0 };
	for (const auto& connectedComponent : m_stronglyConnectedComponents)
	{
		if(xOffSet>3)
		{
			yOffSet = 1;
			xOffSet = 0;
		}
		QRect r(STARTING_POSITION.x()+xOffSet*100,STARTING_POSITION.y()+yOffSet*100, NODE_SIZE, NODE_SIZE);
		stronglyConnectedComponentsAndCoordinates.emplace_back(
			connectedComponent,
			std::make_pair(
				STARTING_POSITION.x() + xOffSet * 100,
				STARTING_POSITION.y() + yOffSet * 100
			)
		);
		xOffSet++;
		QPen nodeColor;
		QColor color;
		switch (counter%6)
		{
		case 0:
			color = Qt::blue;
			break;
		case 1:
			color = Qt::red;
			break;
		case 2:
			color = Qt::darkCyan;
			break;
		case 3:
			color = Qt::magenta;
			break;
		case 4:
			color = Qt::darkGray;
			break;
		default:
			color = Qt::green;
			break;
		}

		nodeColor.setColor(color);
		nodeColor.setWidth(2);
		p.setPen(nodeColor);
		p.drawEllipse(r);
		QString s;
		for (const auto& component : connectedComponent) {
			s.append(QString::number(component));
			s.append(",");
		}
		if (!s.isEmpty()) {
			s.chop(1);
		}
		p.drawText(r, Qt::AlignCenter, s);
		counter++;
	}
	QPen nodeColor;
	nodeColor.setColor(Qt::black);
	nodeColor.setWidth(2);
	p.setPen(nodeColor);
	for(int i=0;i<stronglyConnectedComponentsAndCoordinates.size()-1;i++)
	{
		auto Coordinates = archDrawXYOfSet(
			{
			stronglyConnectedComponentsAndCoordinates[i].second.first,
			stronglyConnectedComponentsAndCoordinates[i].second.second,
			},
			{
			stronglyConnectedComponentsAndCoordinates[i+1].second.first,
			stronglyConnectedComponentsAndCoordinates[i + 1].second.second,
			}
			);
		p.drawLine(stronglyConnectedComponentsAndCoordinates[i].second.first + std::get<0>(Coordinates),
		           stronglyConnectedComponentsAndCoordinates[i].second.second + std::get<1>(Coordinates),
		           stronglyConnectedComponentsAndCoordinates[i+1].second.first + std::get<2>(Coordinates),
		           stronglyConnectedComponentsAndCoordinates[i+1].second.second + std::get<3>(Coordinates));

	}
}


std::tuple<int, int, int, int> StronglyConnectedComponentsDraw::archDrawXYOfSet(const std::pair<int, int>& firstNode, const std::pair<int, int>& secondNode)
{

	if (firstNode.first == secondNode.first)
	{
		if (firstNode.second - secondNode.second < 0)
		{
			return{ NODE_SIZE / 2,NODE_SIZE,NODE_SIZE / 2,0 };
		}
		return{ NODE_SIZE / 2,0,NODE_SIZE / 2,NODE_SIZE };

	}
	if (firstNode.first - secondNode.first < 0)
	{
		return{ NODE_SIZE,NODE_SIZE / 2,0,NODE_SIZE / 2 };
	}

	return { 0,NODE_SIZE / 2,NODE_SIZE,NODE_SIZE / 2 };
}

