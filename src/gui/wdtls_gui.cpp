// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Part of wadtools
 * 
 * Entry point for the gui application
 *
 * Copyright (c) 2023 spacebub
 * Authors:
 *	spacebub <spacebubs@proton.me>
 */
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication application(argc, argv);
	MainWindow mainWindow;

	mainWindow.show();
	return QApplication::exec();
}