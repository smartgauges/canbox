#include <QDateTime>
#include <QFileDialog>
#include <QDebug>

#include "wdg_dbg.h"
#include "ui_dbg.h"

wdg_dbg_t::wdg_dbg_t(QWidget * parent) : QWidget(parent), ui(new Ui::dbg)
{
	ui->setupUi(this);

	ui->cb_level->addItem("Warn", e_log_warn);
	ui->cb_level->addItem("Status", e_log_status);
	ui->cb_level->addItem("Info", e_log_info);
	ui->cb_level->addItem("Debug", e_log_debug);
	ui->cb_level->addItem("All", e_log_all);

	ui->cb_level->setCurrentIndex(e_log_all);

	connect(ui->btn_clr, &QPushButton::clicked, this, &wdg_dbg_t::slt_btn_clr);
	connect(ui->btn_save, &QPushButton::clicked, this, &wdg_dbg_t::slt_btn_save);
}

wdg_dbg_t::~wdg_dbg_t()
{
}

void wdg_dbg_t::slt_log(uint8_t lvl, const QString & txt)
{
	QVariant variant = ui->cb_level->currentData();
	int log_level = variant.value<int>();

	if (lvl > log_level)
		return;

	QDateTime dt = QDateTime::currentDateTime();
	QString st = dt.toString("hh:mm:ss.z");
	if (lvl == e_log_warn)
		ui->log->appendHtml(st + " " + "<font color = \"red\">" + txt.toHtmlEscaped() + "</font>");
	else if (lvl == e_log_debug)
		ui->log->appendHtml(st + " " + "<font color = \"blue\">" + txt.toHtmlEscaped() + "</font>");
	else
		ui->log->appendPlainText(st + " " + txt);
}

void wdg_dbg_t::slt_btn_clr()
{
	ui->log->clear();
}

void wdg_dbg_t::slt_btn_save()
{
	QDateTime dt = QDateTime::currentDateTime();
	QString st = dt.toString("yyyy-MM-dd-hhmmss");

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save log file"), QString(".") + QDir::separator() + "qcanbox-log-" + st + ".txt", tr("text (*.txt)"));

	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
		return;

	file.write(ui->log->toPlainText().toLocal8Bit());

	file.close();
}

