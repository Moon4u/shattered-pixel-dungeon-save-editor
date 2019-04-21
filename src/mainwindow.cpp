
#include <QDebug>

#include <QKeyEvent>
#include <QScrollBar>
#include <QFileSystemModel>

#include "utils.h"
#include "errors.h"
#include "qjsonmodel.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::MainWindow	Constructor for the main window class.
 * @param parent					The parent object.
 */
MainWindow::MainWindow( QWidget* parent )
	: QMainWindow( parent )
	, m_ui( new Ui::MainWindow )
	, m_fileSystemModel( nullptr )
	, m_jsonModel( nullptr )
	, m_newItemWidget( nullptr )
{
	m_ui->m_treeView	= nullptr;
	m_ui->setupUi( this );
	m_jsonModel			= new QJsonModel();
	m_fileSystemModel	= new QFileSystemModel();
	m_fileSystemModel->setRootPath( spd_utils.pixelDungeonDataDir().path() );
	setFileSystemModel();

	m_ui->m_treeView->setMouseTracking( true );
	m_ui->m_treeView->viewport()->installEventFilter( this );

	m_newItemWidget	= new NewItemWidget();

	updateButtons( NO_BUTTONS );

	m_ui->m_lblWarning->hide();

	connect( m_ui->m_btnBack, SIGNAL( clicked( bool ) ), this, SLOT( onBackClicked( bool ) ) );
	connect( m_ui->m_btnSave, SIGNAL( clicked( bool ) ), this, SLOT( onSaveClicked( bool ) ) );
	connect( m_ui->m_btnAddItem, SIGNAL( clicked( bool ) ), this, SLOT( onAddItemClicked( bool ) ) );
	connect( m_ui->m_btnRefresh, SIGNAL( clicked( bool ) ), this, SLOT( onRefreshClicked( bool ) ) );
	connect( m_ui->m_btnIdAllItems, SIGNAL( clicked( bool ) ), this, SLOT( onIdAllItemsClicked( bool ) ) );
	connect( m_ui->m_btnRevealDepth, SIGNAL( clicked( bool ) ), this, SLOT( onRevealDepthClicked( bool ) ) );
	connect( m_ui->m_btnRemoveCurses, SIGNAL( clicked( bool ) ), this, SLOT( onCleanseItemsClicked( bool ) ) );

	connect( m_newItemWidget,	SIGNAL( addItem( QJsonTreeItem*, int ) ),
			 this,				SLOT( onAddItem( QJsonTreeItem*, int ) ) );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::~MainWindow	Destructor.
 */
MainWindow::~MainWindow()
{
	delete m_ui;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::eventFilter	Event filter for the treeview's viewport.
 *
 * \note see bool QObject::eventFilter
 */
bool	MainWindow::eventFilter( QObject* watched, QEvent* event )
{
	if( watched != m_ui->m_treeView->viewport() )
		return	false;

	if( m_ui->m_treeView->model() != m_fileSystemModel )
		return false;

	if( event->type() != QEvent::MouseButtonDblClick )
		return	false;

	auto	mouseEvent	= static_cast< QMouseEvent* >( event );
	if( mouseEvent->button() != Qt::LeftButton )
		return false;

	auto	pos	= mouseEvent->pos();
	pos.setY( pos.y() + m_ui->m_treeView->horizontalScrollBar()->value() );

	QModelIndex	posIndex	= m_ui->m_treeView->indexAt( pos );
	auto		data		= m_fileSystemModel->index( posIndex.row(), 0, posIndex.parent() );
	auto		filePath	= data.data( QFileSystemModel::FilePathRole ).toString();

	loadArchiveAsJson( filePath );
	return	false;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::event	Events handler function. Used to detect Ctrl+S and
 * to check if there are any changes to the currently opened file.
 */
bool	MainWindow::event( QEvent* event )
{
	if( event->type() == QEvent::KeyPress )
	{
		QKeyEvent*	keyEvent	= static_cast< QKeyEvent* >( event );
		if( keyEvent->matches( QKeySequence::Save ) )
			onSaveClicked( true );

		return	QMainWindow::event( event );
	}

	if( event->type() == QEvent::WindowActivate )
		onWindowActivate();

	return	QMainWindow::event( event );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::setFileSystemModel	Sets the model for the tree view to
 * be the file system one.
 */
void	MainWindow::setFileSystemModel()
{
	m_ui->m_treeView->setModel( m_fileSystemModel );
	QModelIndex	idx	= m_fileSystemModel->index( m_fileSystemModel->rootPath() );
	m_ui->m_treeView->setRootIndex( idx );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::updateButtons	Updates button position based on given type.
 */
void	MainWindow::updateButtons( BUTTON_TYPES type )
{
	switch( type )
	{
		case	NO_BUTTONS:
			m_ui->m_btnBack->hide();
			m_ui->m_btnSave->hide();
			m_ui->m_btnAddItem->hide();
			m_ui->m_btnRefresh->hide();
			m_ui->m_btnIdAllItems->hide();
			m_ui->m_btnRevealDepth->hide();
			m_ui->m_btnRemoveCurses->hide();
			break;

		case	DEPTH_BUTTONS:
			m_ui->m_btnBack->show();
			m_ui->m_btnSave->show();
			m_ui->m_btnRefresh->show();
			m_ui->m_btnAddItem->hide();
			m_ui->m_btnIdAllItems->hide();
			m_ui->m_btnRevealDepth->show();
			m_ui->m_btnRemoveCurses->hide();
			break;

		case	GAME_BUTTONS:
			m_ui->m_btnBack->show();
			m_ui->m_btnSave->show();
			m_ui->m_btnRefresh->show();
			m_ui->m_btnAddItem->show();
			m_ui->m_btnIdAllItems->show();
			m_ui->m_btnRevealDepth->hide();
			m_ui->m_btnRemoveCurses->show();
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onFileOpened	Function that is called when a file is
 * opened.
 */
void	MainWindow::onFileOpened()
{
	const QString	DEPTH_STR	= "depth";
	const QString	GAME_STR	= "game";

	if( m_jsonFilePath.contains( DEPTH_STR ) )
	{
		updateButtons( DEPTH_BUTTONS );
		m_ui->m_treeView->setModel( m_jsonModel );
		return;
	}

	if( m_jsonFilePath.contains( GAME_STR ) )
	{
		updateButtons( GAME_BUTTONS );
		m_ui->m_treeView->setModel( m_jsonModel );
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::findInventoryTypeModelIndex	Helper function htat returns
 * the model idnex of the inventory array for a specific type of bag.
 * Can return inventory for MagicHolster, PotionBandolier, ScrollHolster or
 * VelvetPouch.
 * If the bag does not exist in the character's inventory returns invalid model
 * index.
 */
QModelIndex	MainWindow::findInventoryTypeModelIndex( int type )
{
	const char*		TYPE_NAMES[]	= { "MagicHolster", "PotionBandolier", "ScrollHolster", "VelvetPouch" };
	const QString	INVENTORY_NAME	= QString( TYPE_NAMES[ type ] );

	QModelIndex	inventory	= findInventoryModelIndex();
	if( ! inventory.isValid() )
		return QModelIndex();

	for( int i = 0; i < m_jsonModel->rowCount( inventory ); ++i )
	{
		auto	itemIndex				= m_jsonModel->index( i, 0, inventory );
		bool	isTheDesiredInventory	= false;
		for( int j = 0; j < m_jsonModel->rowCount( itemIndex ); ++j )
		{
			auto	propertyIndex	= m_jsonModel->index( j, 1, itemIndex );
			auto	propertyData	= m_jsonModel->data( propertyIndex, Qt::DisplayRole );
			if( propertyData.toString().contains( INVENTORY_NAME ) )
			{
				isTheDesiredInventory	= true;
				break;
			}
		}

		if( isTheDesiredInventory )
		{
			auto	tempModelIndex	= findInventoryJsonKey( itemIndex );
			if( tempModelIndex.isValid() )
				return	tempModelIndex;
		}
	}

	return	QModelIndex();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::findInventoryModelIndex Returns the model index for the
 * default inventory for the character. Returns invalid model index, if the
 * inventory is not found.
 */
QModelIndex	MainWindow::findInventoryModelIndex()
{
	const QString	HERO_STR		= "hero";

	for( int i = 0; i < m_jsonModel->rowCount(); ++i )
	{
		auto	heroIndex	= m_jsonModel->index( i, 0 );
		auto	heroData	= m_jsonModel->data( heroIndex, Qt::DisplayRole );
		if( heroData.toString() == HERO_STR )
		{
			auto	tempModelIndex	= findInventoryJsonKey( heroIndex );
			if( tempModelIndex.isValid() )
				return	tempModelIndex;
		}
	}

	return	QModelIndex();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::findInventoryJsonKey	Helper function that looks for
 * inventory data in the given model index.
 */
QModelIndex	MainWindow::findInventoryJsonKey( const QModelIndex& item )
{
	const QString	INVENTORY_STR	= "inventory";

	for( int j = 0; j < m_jsonModel->rowCount( item ); ++j )
	{
		auto	inventoryIndex	= m_jsonModel->index( j, 0, item );
		auto	inventoryData	= m_jsonModel->data( inventoryIndex, Qt::DisplayRole );
		if( inventoryData.toString() == INVENTORY_STR )
		{
			return	inventoryIndex;
		}
	}

	return	QModelIndex();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onBackWithSave A slot that is called when the user clicks
 * on 'Go back and save'.
 */
void	MainWindow::onBackWithSave( bool clicked )
{
	Q_UNUSED( clicked );

	spd_utils.saveFile( m_jsonFilePath, m_jsonModel->json() );

	setFileSystemModel();
	m_jsonFilePath.clear();
	updateButtons( NO_BUTTONS );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onBackWithoutSave	A slot that is called when the user
 * clicks on 'Go back whitout save'
 * @param clicked
 */
void	MainWindow::onBackWithoutSave( bool clicked )
{
	Q_UNUSED( clicked );

	setFileSystemModel();
	m_jsonFilePath.clear();
	updateButtons( NO_BUTTONS );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onBackCancel	An empy slot that is called when the user
 * clicks on cancel.
 */
void	MainWindow::onBackCancel( bool clicked )
{
	Q_UNUSED( clicked );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onBackClicked	A slot that is called when the back button
 * is clicked. Will open a message box prompting the user to save.
 */
void	MainWindow::onBackClicked( bool clicked )
{
	Q_UNUSED( clicked );

	const QString	MSG_BOX_TEXT		= "Any changes done will not be saved if you proceed.";
	const QString	SAVE_BTN_TEXT		= "Save";
	const QString	NO_SAVE_BTN_TEXT	= "Dont Save";
	const QString	CANCEL_BTN_TEXT		= "Cancel";

	QMessageBox	msgBox;
	msgBox.setText( MSG_BOX_TEXT );

	auto	saveBtn		= msgBox.addButton( SAVE_BTN_TEXT, QMessageBox::AcceptRole );
	auto	noSaveBtn	= msgBox.addButton( NO_SAVE_BTN_TEXT, QMessageBox::AcceptRole );
	auto	cancelBtn	= msgBox.addButton( CANCEL_BTN_TEXT, QMessageBox::RejectRole );

	connect( saveBtn, SIGNAL( clicked( bool ) ), this, SLOT( onBackWithSave( bool ) ) );
	connect( noSaveBtn, SIGNAL( clicked( bool ) ), this, SLOT( onBackWithoutSave( bool ) ) );
	connect( cancelBtn, SIGNAL( clicked( bool ) ), this, SLOT( onBackCancel( bool ) ) );

	msgBox.exec();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onSaveClicked A slot that is called when the user clicks
 * on the save button.
 */
void	MainWindow::onSaveClicked( bool clicked )
{
	Q_UNUSED( clicked );

	qDebug() << "Save initialized";
	spd_utils.saveFile( m_jsonFilePath, m_jsonModel->json() );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onAddItemClicked	A slot thtat is called when the user
 * clicks on the add item button.
 */
void	MainWindow::onAddItemClicked( bool clicked )
{
	Q_UNUSED( clicked );
	m_newItemWidget->show();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onIdAllItemsClicked A slot that is called when the user
 * clicks on the 'id all items' button.
 */
void	MainWindow::onIdAllItemsClicked( bool clicked )
{
	Q_UNUSED( clicked );

	const QString	KNOWN_STR	= "_known";

	for( int i = 0; i < m_jsonModel->rowCount(); ++i )
	{
		auto	keyIndex	= m_jsonModel->index( i, 0 );
		auto	keyData		= m_jsonModel->data( keyIndex, Qt::DisplayRole );
		if( keyData.toString().contains( KNOWN_STR ) )
		{
			auto	valueIndex	= m_jsonModel->index( i, 1 );
			m_jsonModel->setData( valueIndex, "true" );
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onRevealDepthClicked A slot that is called when the user
 * clicks on 'reveal depth' button.
 */
void	MainWindow::onRevealDepthClicked( bool clicked )
{
	Q_UNUSED( clicked );

	const QString	VISITED_KEY_STR		= "visited";
	const QString	VISITED_VALUE_STR	= "true";

	auto	levelIndex	= m_jsonModel->index( 0, 0 );
	for( int i = 0; i < m_jsonModel->rowCount( levelIndex ); ++i )
	{
		auto	keyIndex	= m_jsonModel->index( i, 0, levelIndex );
		auto	keyData		= m_jsonModel->data( keyIndex, Qt::DisplayRole );
		if( keyData == VISITED_KEY_STR )
		{
			auto treeItem	= static_cast< QJsonTreeItem* >( keyIndex.internalPointer() );
			for( int j = 0; j < treeItem->childCount(); ++j )
			{
				treeItem->child( j )->setValue( VISITED_VALUE_STR );
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onCleanseItemsClicked A slot thta is called when the user
 * clicks on 'cleanse items' bytton.
 */
void	MainWindow::onCleanseItemsClicked( bool clicked )
{
	Q_UNUSED( clicked );
	const QString	CURSED_STR	= "cursed";

	QModelIndex	inventoryIndex	= findInventoryModelIndex();
	for( int i = 0; i < m_jsonModel->rowCount( inventoryIndex ); ++i )
	{
		auto	inventoryItemIndex	= m_jsonModel->index( i, 0, inventoryIndex );

		for( int j = 0; j < m_jsonModel->rowCount( inventoryItemIndex ); ++j )
		{
			auto	itemPropertyIndex	= m_jsonModel->index( j, 0, inventoryItemIndex );
			auto	itemPropertyData	= m_jsonModel->data( itemPropertyIndex, Qt::DisplayRole );

			if( itemPropertyData.toString() == CURSED_STR )
			{
				auto	itemValueIndex	= m_jsonModel->index( j, 1, inventoryItemIndex );
				m_jsonModel->setData( itemValueIndex, "false" );
				break;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onRefreshClicked A slot that is called when the user
 * clicks on the refresh button.
 */
void	MainWindow::onRefreshClicked( bool clicked )
{
	Q_UNUSED( clicked );

	loadArchiveAsJson( m_originalFile.filePath() );
	m_ui->m_lblWarning->hide();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onAddItem A slot that is called when the user clicks on
 * add item (from the NewItemWidget window)
 */
void	MainWindow::onAddItem( QJsonTreeItem* itemRoot, int type )
{
	int	specialBag	= -1;
	switch( type )
	{
		case	NewItemWidget::POTION:
			specialBag	= INVENTORY_TYPE::POTION_BANDOLIER;
			break;

		case	NewItemWidget::SCROLL:
			specialBag	= INVENTORY_TYPE::SCROLL_HOLDER;
			break;

		case	NewItemWidget::WAND:
			specialBag	= INVENTORY_TYPE::MAGIC_HOLSTER;
			break;

		default:
			break;
	}

	QModelIndex	inventory	= findInventoryModelIndex();
	if( specialBag != -1 )
	{
		QModelIndex	tempInventory	= findInventoryTypeModelIndex( specialBag );
		if( tempInventory.isValid() )
			inventory	= tempInventory;
	}

	auto	item	= static_cast< QJsonTreeItem* >( inventory.internalPointer() );
	itemRoot->setParent( item );

	if( type == NewItemWidget::BAG )
		item->appendChild( itemRoot );
	else
		item->prependChild( itemRoot );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::loadArchiveAsJson Loads archive as json and sets it as a
 * model for the tree view.
 */
void	MainWindow::loadArchiveAsJson( const QString& filePath )
{
	QFileInfo	fileInfo( filePath );
	if( fileInfo.isDir() )
		return;

	m_originalFile			= fileInfo;
	m_originalFileLastEdit	= m_originalFile.lastModified();

	QString		extractedFile;
	QByteArray	jsonFile	= spd_utils.loadFile( filePath, extractedFile );
	if( jsonFile.isEmpty() )
		return;

	m_jsonModel->loadJson( jsonFile );
	m_jsonFilePath	= extractedFile;

	onFileOpened();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief MainWindow::onWindowActivate	A function that checks if the currently
 * open save file has been changed since it was last opened.
 */
void	MainWindow::onWindowActivate()
{
	if( ! m_originalFile.exists() )
		return;

	if( m_ui->m_treeView->model() != m_jsonModel )
		return;

	m_originalFile.refresh();
	if( m_originalFile.lastModified() == m_originalFileLastEdit )
		return;

	m_ui->m_lblWarning->show();
}
////////////////////////////////////////////////////////////////////////////////
