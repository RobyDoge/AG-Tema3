#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_DesenareGrafuri.h"

#include "StronglyConnectedComponentsDraw.h"
#include "TopologicalSortDraw.h"
#include "Graph.h"
#include <QMouseEvent>


class DesenareGrafuri : public QMainWindow
{
    Q_OBJECT

public:
    DesenareGrafuri(QWidget *parent = nullptr);
    ~DesenareGrafuri();

    void mouseReleaseEvent(QMouseEvent* e) override;
    void paintEvent(QPaintEvent* e) override;
    void OnButtonTopologicalSortingClicked();
    void OnButtonStronglyConnectedComponentsClicked();

private:
    bool ValidatePosition(const QMouseEvent* e);
    void ReturnToMainWindow();

    Ui::DesenareGrafuriClass ui;
    Graph m_graph;
    Node m_firstNode;
    TopologicalSortDraw* m_windowTS;
    StronglyConnectedComponentsDraw* m_windowSCC;
};
