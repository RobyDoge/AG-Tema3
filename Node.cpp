#include "Node.h"

Node::Node()
{
}

Node::Node(int value, QPoint coordiantes)
{
    m_value = value;
    m_coordinates = coordiantes;
}

Node::Node(int value, int x, int y)
{
    m_value = value;
    m_coordinates.setX(x);
    m_coordinates.setY(y);
}

void Node::setValue(int value)
{
    m_value = value;
}

void Node::setCoordinates(QPoint coordinates)
{
    m_coordinates = coordinates;
}

void Node::setCoordinates(int x, int y)
{
    m_coordinates.setX(x);
    m_coordinates.setY(y);
}

void Node::setX(int x)
{
    m_coordinates.setX(x);
}

void Node::setY(int y)
{
    m_coordinates.setY(y);
}

QPoint Node::getCoordinates()
{
    return m_coordinates;
}

int Node::getX()
{
    return m_coordinates.x();
}

int Node::getY()
{
    return m_coordinates.y();
}

int Node::getValue()
{
    return m_value;
}