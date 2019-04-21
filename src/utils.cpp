
#include <QProcess>
#include <QJsonDocument>
#include <QStandardPaths>

#include "errors.h"
#include "utils.h"

////////////////////////////////////////////////////////////////////////////////

const char* const	APP_7ZIP_PATH			= "\"C:\\Program Files\\7-Zip\\7z.exe\"";
const QString		APP_7ZIP_START_ERROR	= "Failed to start 7zip.";

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::Utils Constructor.
 */
Utils::Utils()
{
	update7zipAppFilePath();
	updateShatteredPixelDataDir();
	updateShatteredPixelEditDir();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::~Utils Destructor.
 */
Utils::~Utils()
{
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::loadFile A function that extract save file and returns the
 * data as QByteArray and the path to the output file.
 */
QByteArray	Utils::loadFile( const QString& loadFilePath, QString& outputFile )
{
	const QString	APP_7ZIP_ARGS	= " -y e \"%1\" -o\"%2\" ";

	QByteArray	returnValue;

	QFileInfo	loadFileInfo( loadFilePath );
	if( ! loadFileInfo.isFile() )
		return	returnValue;

	QString	editFileDirPath	= getEditDirPathFoFile( loadFileInfo );

	QFileInfo	editFileInfo( editFileDirPath, loadFileInfo.baseName() );
	outputFile	= editFileInfo.filePath();

	QString	app7zArgs	= APP_7ZIP_ARGS.arg( loadFilePath, editFileDirPath );

	QProcess	app7z;
	app7z.setWorkingDirectory( loadFileInfo.dir().path() );
	app7z.start( m_7zipAppPath + app7zArgs );
	if( ! app7z.waitForStarted( 1000 ) )
	{
		ERROR_MSG( APP_7ZIP_START_ERROR );
		return	returnValue;
	}
	app7z.closeWriteChannel();
	if( ! app7z.waitForFinished( 1000 ) )
	{
		ERROR_MSG( APP_7ZIP_START_ERROR );
		return	returnValue;
	}

	QFileInfo	app7zOutputFileInfo( editFileInfo.filePath() );
	if( ! app7zOutputFileInfo.exists() )
	{
		ERROR_MSG( "Did not find file " + app7zOutputFileInfo.path() );
		return	returnValue;
	}

	QFile		app7zOutputFile( editFileInfo.filePath() );
	app7zOutputFile.open( QFile::ReadOnly );
	returnValue	= app7zOutputFile.readAll();

	return	returnValue;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::saveFile A method that saves json document to the a file, and
 * restores the saved file to the save's original location.
 */
void	Utils::saveFile( const QString& jsonFilePath, const QJsonDocument& doc )
{
	QString	archivedFileLocation	= storeJsonDataToFile( doc, jsonFilePath );
	moveArchivedFileToLocation( archivedFileLocation );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::update7zipAppFilePath A method that updates the path to the
 * 7zip application.
 */
void	Utils::update7zipAppFilePath()
{
	const char* const	ERROR_7ZIP_REQ_MSG	= "This app requires 7ZIP";

	QString	app_7zip_path( APP_7ZIP_PATH );
	app_7zip_path.remove( 0, 1 );
	app_7zip_path.remove( app_7zip_path.size() - 1, 1 );

	QFile	fileInfo( app_7zip_path );
	if( ! fileInfo.exists() )
		ERROR_MSG( ERROR_7ZIP_REQ_MSG );

	m_7zipAppPath	= APP_7ZIP_PATH;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::updateShatteredPixelDataDir Updates the shattered pixel dungeon
 * directory where save files are stored.
 */
void	Utils::updateShatteredPixelDataDir()
{
	const QString	SAVE_FILE_DIR_ERROR	= "Could not find shattered pixel dungeon save data directory";

	const QString	SHATTERED_BASE_DIR	= ".shatteredpixel";
	const QString	SHATTERED_SUB_DIR	= "Shattered Pixel Dungeon";
	auto	dirs	= getPossibleDataPaths();

	for( auto dir : dirs )
	{
		if( dir.exists( SHATTERED_BASE_DIR ) )
		{
			if( ! dir.cd( SHATTERED_BASE_DIR ) )
				continue;

			if( ! dir.cd( SHATTERED_SUB_DIR ) )
				continue;

			m_pixelDungeonDataDir	= dir;
			return;
		}
	}

	ERROR_MSG( SAVE_FILE_DIR_ERROR );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::updateShatteredPixelEditDir A method that updates the
 * directory for the temporary extracted files.
 */
void	Utils::updateShatteredPixelEditDir()
{
	const QString	DATA_DIR_NOT_SET_ERROR	= "Data dir is not set";

	if( ! m_pixelDungeonDataDir.exists() )
	{
		ERROR_MSG( DATA_DIR_NOT_SET_ERROR );
		return;
	}

	QDir	pixelDungeonDirPath	= m_pixelDungeonDataDir;
	pixelDungeonDirPath.cdUp();

	if( ! pixelDungeonDirPath.cd( EDIT_DIR ) )
	{
		pixelDungeonDirPath.mkdir( EDIT_DIR );
		pixelDungeonDirPath.cd( EDIT_DIR );
	}

	m_pixelDungeonEditDir	= pixelDungeonDirPath;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::moveArchivedFileToLocation A method that moves a file to it's
 * supposed location.
 */
void	Utils::moveArchivedFileToLocation( const QString& archivedFilePath ) const
{
	QFileInfo	archivedFileInfo( archivedFilePath );
	if( ! archivedFileInfo.exists() )
		return;

	QDir	copyDir	= m_pixelDungeonDataDir;
	if( archivedFileInfo.dir() != m_pixelDungeonEditDir.path() )
	{
		copyDir.cd( archivedFileInfo.dir().dirName() );
	}

	QFileInfo	existingFileInfo( copyDir, archivedFileInfo.fileName() );
	if( existingFileInfo.exists() )
	{
		QFile	existingFile( existingFileInfo.filePath() );
		existingFile.remove();
		existingFile.close();
	}

	QFile	archivedFile( archivedFilePath );
	archivedFile.open( QFile::ReadWrite );
	archivedFile.rename( existingFileInfo.filePath() );
	archivedFile.close();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::storeJsonDataToFile A method that stores json document to a
 * file.
 */
const QString	Utils::storeJsonDataToFile( const QJsonDocument& doc, const QString& jsonFilePath ) const
{
	const QString	APP_7ZIP_START_ERROR	= "Failed to start 7zip.";

	QString	archivedFilePath;

	if( jsonFilePath.isEmpty() )
	{
		ERROR_MSG( "No file loaded" );
		return	archivedFilePath;
	}

	QFileInfo	jsonFile( jsonFilePath );
	QString		tempFilePath	= jsonFilePath + ".dat";

	QString	data( doc.toJson( QJsonDocument::Compact ) );
	if( data.isEmpty() )
	{
		ERROR_MSG( "Data is empty" );
		return	archivedFilePath;
	}

	QFile::remove( jsonFilePath );
	QFile	currentJsonFile( jsonFilePath );
	currentJsonFile.open( QFile::WriteOnly );
	currentJsonFile.write( data.toUtf8() );
	currentJsonFile.close();

	QFile		file( tempFilePath );
	QFileInfo	fileInfo( tempFilePath );
	if( fileInfo.exists() )
	{
		file.remove();
		file.close();
	}

	QString	s7zipCommand( m_7zipAppPath );
	s7zipCommand += " -tgzip a \"" + tempFilePath + "\" \"" + jsonFilePath + "\"";

	QProcess s7zip;
	s7zip.start( s7zipCommand );
	if( ! s7zip.waitForStarted( 1000 ) )
	{
		ERROR_MSG( APP_7ZIP_START_ERROR );
		return	archivedFilePath;
	}
	s7zip.closeWriteChannel();
	if( ! s7zip.waitForFinished( 1000 ) )
	{
		ERROR_MSG( APP_7ZIP_START_ERROR );
		return	archivedFilePath;
	}

	archivedFilePath	= tempFilePath;
	return	archivedFilePath;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::getEditDirPathFoFile A method that takes save file, and returns
 * a proper location for the extracted file.
 */
QString	Utils::getEditDirPathFoFile( const QFileInfo& fileInfo ) const
{
	QDir	dir			= fileInfo.dir();
	QDir	outputDir	= m_pixelDungeonEditDir;
	if( dir.path() != m_pixelDungeonDataDir.path() )
	{
		QString	dirName	= dir.dirName();
		outputDir.mkdir( dirName );
		outputDir.cd( dirName );
	}

	return	outputDir.path();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::getPossibleDataPaths Returns the paths to
 *	C:/Users/<user>/AppData/Local
 *	C:/Users/<user>/AppData/LocalLow
 *	C:/Users/<user>/AppData/Roaming
 */
QVector< QDir >	Utils::getPossibleDataPaths() const
{
	const QString	defaultDirs[]	=
	{
		"Local",
		"LocalLow",
		"Roaming"
	};

	QDir	dir( QStandardPaths::writableLocation( QStandardPaths::DataLocation ) );

	dir.cdUp();
	dir.cdUp();

	QVector< QDir >	directories;
	for( auto dirName : defaultDirs )
	{
		if( dir.exists( dirName ) )
		{
			QDir	tempDir( dir );
			tempDir.cd( dirName );
			directories.push_back( tempDir );
		}
	}

	return	directories;
}
////////////////////////////////////////////////////////////////////////////////
