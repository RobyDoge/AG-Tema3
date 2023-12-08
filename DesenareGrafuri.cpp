#include "DesenareGrafuri.h"
#include "TopologicalSorting.h"
#include "StronglyConnectedComponents.h"

#include <QPushButton>
DesenareGrafuri::DesenareGrafuri(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	connect(ui.buttonTopologicalSorting, &QPushButton::clicked, this, &DesenareGrafuri::OnButtonTopologicalSortingClicked);
	connect(ui.buttonStronglyConnectedComponents, &QPushButton::clicked, this, &DesenareGrafuri::OnButtonStronglyConnectedComponentsClicked);
	m_firstNode.setValue(-1);
	m_windowSCC = nullptr;
	m_windowTS = nullptr;
}

DesenareGrafuri::~DesenareGrafuri()= default;

void DesenareGrafuri::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton && ValidatePosition(e))
	{
		m_graph.addNode(e->pos());
		update();
	}
	else if (e->button() == Qt::LeftButton)
	{
		std::vector<Node> nodes = m_graph.getNodes();
		for (Node& n : nodes)
		{
			if (fabs(e->pos().x() - n.getX()) < 10 &&
				fabs(e->pos().y() - n.getY()) < 10)
			{
				if (m_firstNode.getValue() == -1)
					m_firstNode = n;
				else
				{
					if (m_firstNode.getValue() == n.getValue())
					{
						continue;
					}
					m_graph.addArch(Arch(m_firstNode, n), true);
					m_firstNode.setValue(-1);
				}
				update();
				break;
			}
		}
	}
}

void DesenareGrafuri::paintEvent(QPaintEvent* e)
{
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

	if (m_firstNode.getValue() >= 0)
	{
		const QRect r(m_firstNode.getX() - 10, m_firstNode.getY() - 10, 20, 20);
		QPen pen;
		pen.setColor(Qt::red);
		pen.setWidth(2);
		p.setPen(pen);
		p.drawEllipse(r);
		QString s;
		s.setNum(m_firstNode.getValue());
		p.drawText(r, Qt::AlignCenter, s);
	}
}

void DesenareGrafuri::OnButtonTopologicalSortingClicked()
{
	m_windowTS = new TopologicalSortDraw;
	m_windowTS->SetGraph(m_graph);
	m_windowTS->SetSortedVector(TopologicalSorting::TopologicalSort(m_graph));
	connect(m_windowTS, &TopologicalSortDraw::ReturnButtonClicked, this, &DesenareGrafuri::ReturnToMainWindow);

	m_windowTS->show();
	m_windowTS->Draw(m_graph.getNodes());
	hide();
}

void DesenareGrafuri::OnButtonStronglyConnectedComponentsClicked()
{
	m_windowSCC = new StronglyConnectedComponentsDraw;
	auto test = StronglyConnectedComponents::CreateStronglyConnectedComponents(m_graph);
	m_windowSCC->CreateNewGraph(m_graph, test);

	connect(m_windowSCC, &StronglyConnectedComponentsDraw::ReturnButtonClicked, this, &DesenareGrafuri::ReturnToMainWindow);
	m_windowSCC->show();
	hide();
}

void DesenareGrafuri::ReturnToMainWindow()
{
	if(m_windowSCC!=nullptr)
	{
		m_windowSCC->hide();
		show();
		delete m_windowSCC;
		m_windowSCC = nullptr;
	}
	else
	{
		m_windowTS->hide();
		show();
		delete m_windowTS;
		m_windowTS = nullptr;
	}
}


bool DesenareGrafuri::ValidatePosition(const QMouseEvent* e)
{
	const std::vector<Node> nodes = m_graph.getNodes();
	for (auto node : nodes)
	{
		constexpr int minDistance = 20;
		const int dx = e->pos().x() - node.getX();
		const int dy = e->pos().y() - node.getY();

		if (const double distance = std::sqrt(dx * dx + dy * dy); distance < minDistance)
		{
			return false;
		}
	}

	return true;
}
