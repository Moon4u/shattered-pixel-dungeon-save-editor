
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

////////////////////////////////////////////////////////////////////////////////

/**
 * \class MainWindow
 * \brief The main window class.
 *
 * This class contains single tree view and a few buttons.
 * When the window is shown, the tree view is first presented with filesystem
 * model, pointing towards the directory where the shattered pixel dungeon files
 * are stored. The save files are stored as a GZIP of a JSON data structure.
 * When the user double clicks on a save file, the JSON structure is extracted
 * to temporary file and the model is changed to QJsonModel. As a result, this
 * class contains two types of models: QFileSystemModel and QJsonModel.
 * The user can do changes to the temporary json structure and then save that
 * back to the save file. The data used in the json model is stored back to the
 * temporary file, this file is then archived back to the original save
 * location, replacing the original file.
 *
 * \var MainWIndow::m_ui
 * The user interface file, generated by the Qt framework.
 *
 * \var MainWIndow::m_fileSystemModel
 * Pointer to the file system model.
 *
 * \var MainWIndow::m_jsonModel
 * Pointer to the json model.
 *
 * \var MainWIndow::m_newItemWidget
 * Pointer to the new item widget.
 *
 * \var MainWIndow::m_jsonFile
 * String to the temporary json file path.
 *
 * \var MainWIndow::m_originalFile
 * QFileInfo of the original save file.
 *
 * \var MainWIndow::m_originalFileLastEdit
 * QDateTime of the last time the original file was loaded.
 */

////////////////////////////////////////////////////////////////////////////////

#include <QDir>
#include <QDateTime>
#include <QMainWindow>

#include "newitemwidget.h"

////////////////////////////////////////////////////////////////////////////////

class	QJsonModel;
class	QFileSystemModel;

namespace Ui {
	class MainWindow;
}
////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
	Q_OBJECT

	enum	BUTTON_TYPES
	{
		NO_BUTTONS = 0,
		GAME_BUTTONS,
		DEPTH_BUTTONS
	};

public:
	explicit MainWindow( QWidget* parent = nullptr );
	virtual ~MainWindow();

	virtual bool	eventFilter( QObject* watched, QEvent* event );
	virtual bool	event( QEvent* event );

private:
	void	setFileSystemModel();
	void	updateButtons( BUTTON_TYPES type );
	void	onFileOpened();

	enum	INVENTORY_TYPE {
		MAGIC_HOLSTER = 0,
		POTION_BANDOLIER,
		SCROLL_HOLDER,
		VELVET_POUCH
	};

	QModelIndex	findInventoryTypeModelIndex( int type );
	QModelIndex	findInventoryModelIndex();
	QModelIndex	findInventoryJsonKey( const QModelIndex& item );

private slots:
	void	onBackWithSave( bool clicked );
	void	onBackWithoutSave( bool clicked );
	void	onBackCancel( bool clicked );

	void	onBackClicked( bool clicked );
	void	onSaveClicked( bool clicked );
	void	onAddItemClicked( bool clicked );
	void	onIdAllItemsClicked( bool clicked );
	void	onRevealDepthClicked( bool clicked );
	void	onCleanseItemsClicked( bool clicked );
	void	onRefreshClicked( bool clicked );

	void	onAddItem( QJsonTreeItem* itemRoot , int type );

private:
	void	loadArchiveAsJson( const QString& filePath );
	void	onWindowActivate();

private:
	Ui::MainWindow*		m_ui;
	QFileSystemModel*	m_fileSystemModel;
	QJsonModel*			m_jsonModel;
	NewItemWidget*		m_newItemWidget;
	QString				m_jsonFilePath;
	QFileInfo			m_originalFile;
	QDateTime			m_originalFileLastEdit;
};
////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
