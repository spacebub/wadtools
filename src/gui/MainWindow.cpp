// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Part of wadtools
 * 
 * Copyright (c) 2023
 * Authors:
 *	spacebub <spacebubs@proton.me>
 */
#include <QPushButton>
#include <QString>
#include <QFileDialog>
#include "uic/ui_MainWindow.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->browseWadButton, &QPushButton::clicked, this, &MainWindow::loadWad);
	connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refresh);
}

MainWindow::~MainWindow()
{
	delete ui;
	if (loadedWad)
	{
		wad_free(loadedWad);
	}
}

void MainWindow::loadWad()
{
	QString wadPath = QFileDialog::getOpenFileName(
		nullptr,
		("Select WAD"),
		QDir::currentPath(),
		"WAD (*.WAD *.wad)");

	if (wadPath.isEmpty())
	{
		return;
	}

	ui->wadPathLineEdit->setText(wadPath);

	if (loadedWad)
	{
		wad_free(loadedWad);
		loadedWad = nullptr;
	}

	loadedWad = wad_open(wadPath.toUtf8());
	refresh();
}

void MainWindow::refresh()
{
	if (!loadedWad)
	{
		return;
	}

	ui->lumpNamesList->clear();
	ui->mapNamesList->clear();

	list_t* mapNames = wad_map_names(loadedWad);
	for (int i = 0; i < mapNames->length; ++i)
	{
		ui->mapNamesList->addItem((char*)mapNames->values[i]);
	}

	list_t* lumpNames = wad_lump_names(loadedWad);
	for (int i = 0; i < lumpNames->length; ++i)
	{
		ui->lumpNamesList->addItem((char*)lumpNames->values[i]);
	}

	list_free(lumpNames);
}