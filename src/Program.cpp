#include "Program.h"
#include <kovan/general.h>

#include <QProcessEnvironment>
#include <QDebug>
#include <QMap>

Programm::Program(){
	m_processEnvironment.insert("BOOTCLASSPATH", "/usr/share/jamvm/classes.zip:/usr/share/classpath/glibj.zip:/usr/lib/linkjvm-java.jar:/usr/share/classpath/tools.zip");
	m_processEnvironment.insert("CLASSPATH", "/usr/share/jamvm/classes.zip:/usr/share/classpath/glibj.zip:/usr/lib/linkjvm-java.jar:/usr/share/classpath/tools.zip:.");
	m_processEnvironment.insert("LD_LIBRARY_PATH", "/usr/lib/classpath");
}

Program::~Program()
{
	stop();
}

bool Program::start(const QString& command, const QStringList &arguments)
{
	if(path.isEmpty()) return false;
	stop();
	m_process = new QProcess(this);
	m_process->setProcessEnvironment(m_processEnvironment);
	m_process->setProcessChannelMode(QProcess::MergedChannels);
	connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SIGNAL(finished(int, QProcess::ExitStatus)));
	connect(m_process, SIGNAL(readyRead()), SIGNAL(readyRead()));
	
	set_auto_publish(true);
	halt();
	set_auto_publish(false);
		
	m_process->start(path, arguments);
	if(!m_process->waitForStarted()) {
		delete m_process;
		m_process = 0;
		return false;
	}
	emit started();
	m_time.restart();
	return true;
}

void Program::stop()
{
	if(!m_process) return;
	m_process->terminate();
	if(!m_process->waitForFinished(2000)) m_process->kill();
	//unused: const int msecs = m_time.elapsed();
	// write(tr("Finished in %1 seconds").arg(msecs / 1000.0).toAscii());
	delete m_process;
	m_process = 0;
	
	set_auto_publish(true);
	halt();
	set_auto_publish(false);
}

bool Program::isRunning()
{
	return m_process ? m_process->state() != QProcess::NotRunning : false;
}

QProcess *Program::process() const
{
	return m_process;
}

Program *Program::instance()
{
	static Program s_program;
	return &s_program;
}

Program::Program()
	: m_process(0)
{
	m_time.start();
}

Program::Program(const Program&) {}
