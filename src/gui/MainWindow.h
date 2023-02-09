// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Part of wadtools
 * 
 * Copyright (c) 2023 spacebub
 * Authors:
 *	spacebub <spacebubs@proton.me>
 */
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QWidget>
#include "common/wad.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
 Q_OBJECT

 public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;
	
 private:
	Ui::MainWindow* ui;
	wad_t* loadedWad = nullptr;
	void refresh();
	
 private slots:
	void loadWad();
};

#endif //_MAINWINDOW_H_
