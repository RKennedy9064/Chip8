#pragma once

#include <QtWidgets\qmainwindow.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qmenubar.h>

class MainWindow : public QMainWindow
{
		Q_OBJECT

public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

private:
		void CreateConnects();
		void CreateMenus();

		QMenu *m_menu_file;
		QAction *m_action_exit;
};