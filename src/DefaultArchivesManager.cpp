#include "DefaultArchivesManager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QFileSystemWatcher>

DefaultArchivesManager::DefaultArchivesManager(const QString &archivesPath,
	const QString &binariesPath, QObject *parent)
	: ArchivesManager(parent),
	m_archivesPath(archivesPath),
	m_binariesPath(binariesPath),
	m_watcher(new QFileSystemWatcher(this))
{
	m_watcher->addPath(m_archivesPath);
	connect(m_watcher, SIGNAL(directoryChanged(QString)),
		SIGNAL(refresh()));
}

QStringList DefaultArchivesManager::archives() const
{
	return QDir(m_archivesPath).entryList(QDir::NoDot | QDir::NoDotDot | QDir::Files);
}

kiss::KarPtr DefaultArchivesManager::archive(const QString &name) const
{
	return kiss::Kar::load(archivePath(name));
}

QString DefaultArchivesManager::binaryPath(const QString &name) const
{
	return QDir(m_binariesPath).filePath(name + binaryExtension(name));
}

bool DefaultArchivesManager::hasBinary(const QString &name) const
{
	const QString binPath = binaryPath(name);
	return QFile::exists(binPath) && QFileInfo(archivePath(name)).lastModified()
		< QFileInfo(binPath).lastModified();
}

bool DefaultArchivesManager::set(const QString &name, const kiss::KarPtr &archive)
{
	remove(name);
	bool ret = archive->save(archivePath(name));
	if(ret) emit archiveChanged(name);
	return ret;
}

bool DefaultArchivesManager::remove(const QString &name)
{
	bool success = true;
	success &= QFile::remove(archivePath(name));
	success &= !hasBinary(name) || QFile::remove(binaryPath(name));
	return success;
}

QString DefaultArchivesManager::archivePath(const QString &name) const
{
	return QDir(m_archivesPath).filePath(name);
}

QString DefaultArchivesManager::command(const QString &name) const {
	switch(programType(name)){
		case 1: return "java -jar " + binaryPath(name);
		default: return binaryPath(name);
	}
}

QString DefaultArchivesManager::binaryExtension(const QString& name) const{
	switch(programType(name)){
		case 1: return ".jar";
		default: return "";
	}
}

int DefaultArchivesManager::programType(const QString& name) const{
	return archive(name)->getPreference("program_type").toInt();
}
