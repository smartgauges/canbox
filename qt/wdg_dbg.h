#ifndef WDG_DBG_H
#define WDG_DBG_H

#include <QWidget>
#include "log_levels.h"

namespace Ui
{
	class dbg;
}

class wdg_dbg_t : public QWidget
{
	Q_OBJECT

	private:
		Ui::dbg * ui;

	public:
		wdg_dbg_t(QWidget *parent = 0);
		~wdg_dbg_t();

	private slots:
		void slt_btn_clr();
		void slt_btn_save();

	public slots:
		void slt_log(uint8_t level, const QString & log);
};

#endif

