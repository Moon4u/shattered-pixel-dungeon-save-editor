
#ifndef UTILS_H
#define UTILS_H

////////////////////////////////////////////////////////////////////////////////

/**
 * \class Utils
 * \brief A singleton class designed to help working wiht save files easier.
 *
 * This class uses QProcess to call 7zip in order to extract and archive the
 * save files. The class also keeps track of the shattered pixel dungeon's save
 * directory and the directory of the temporary, extracted, file saves (as json).
 *
 * \var Utils::m_pixelDungeonEditDir
 * A string to the direcory of the extracted files.
 *
 * \var Utils::m_pixelDungeonDataDir
 * A string to the directory of shattered pixel dungeon's save files.
 *
 * \var Utils::m_7zipAppPath
 * The path to the 7zip applicaiton.
 */

////////////////////////////////////////////////////////////////////////////////

#include <QDir>
#include <QByteArray>

////////////////////////////////////////////////////////////////////////////////

const QString	EDIT_DIR		= "Shattered Pixel Dungeon Edits";

////////////////////////////////////////////////////////////////////////////////

#define spd_utils	Utils::getInstance()

////////////////////////////////////////////////////////////////////////////////

class Utils
{
public:
	static Utils&	getInstance();
	void			updateSaveFiles( const QDir& shatteredSaveDir );

	QDir			pixelDungeonEditDir() const;
	QDir			pixelDungeonDataDir() const;
	const QString	app7zipAppPath() const;

	QByteArray		loadFile( const QString& loadFilePath, QString& outputFile );
	void			saveFile( const QString& archiveFilePath , const QJsonDocument& doc );

private:
	void			update7zipAppFilePath();
	void			updateShatteredPixelDataDir();
	void			updateShatteredPixelEditDir();

	void			moveArchivedFileToLocation( const QString& archivedFilePath ) const;
	const QString	storeJsonDataToFile( const QJsonDocument& doc, const QString& jsonFilePath ) const;

	QString			getEditDirPathFoFile( const QFileInfo& fileInfo ) const;
	QVector< QDir >	getPossibleDataPaths() const;

private:
	explicit	Utils();
	virtual		~Utils();

private:
	QDir	m_pixelDungeonEditDir;
	QDir	m_pixelDungeonDataDir;
	QString	m_7zipAppPath;
};
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::pixelDungeonDataDir Returns the path to shattered pixel
 * dungeon's data directory.
 */
inline
QDir	Utils::pixelDungeonDataDir() const
{
	return	m_pixelDungeonDataDir;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::pixelDungeonDataDir Returns the path to the directory used to
 * store temporary files.
 */
inline
QDir	Utils::pixelDungeonEditDir() const
{
	return	m_pixelDungeonEditDir;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::app7zipAppPath Returns the path to the 7zip application.
 */
inline
const QString	Utils::app7zipAppPath() const
{
	return	m_7zipAppPath;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Utils::getInstance Returns the singleton instance of the Utils class.
 */
inline
Utils&	Utils::getInstance()
{
	static Utils	instance;

	return	instance;
}
////////////////////////////////////////////////////////////////////////////////

#endif // UTILS_H
