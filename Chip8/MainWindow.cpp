#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent)
{
		CreateMenus();
		CreateConnects();
}

MainWindow::~MainWindow()
{
}

void MainWindow::CreateConnects()
{
		connect(m_action_exit, &QAction::triggered, [=]() { close(); });
}

void MainWindow::CreateMenus()
{
		m_menu_file = new QMenu("&File");
		m_action_exit = m_menu_file->addAction("Exit");

		menuBar()->addMenu(m_menu_file);
}