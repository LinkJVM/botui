#include "FileActionEditKar.h"
#include "RootController.h"
#include "EditorWidget.h"

#include <kar.hpp>
#include <QDebug>

FileActionEditKar::FileActionEditKar()
	: FileAction("Edit")
{
	
}

bool FileActionEditKar::canHandle(const QString &path) const
{
	return Kiss::Kar::isValid(path);
}

bool FileActionEditKar::act(const QString &path, Device *device) const
{
	EditorWidget *editor = new EditorWidget(device);
	editor->setSavePath(path);
	editor->setArchive(Kiss::Kar::load(path));
	RootController::ref().presentWidget(editor);
	return true;
}

REGISTER_FILE_ACTION(FileActionEditKar)