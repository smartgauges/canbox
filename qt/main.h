#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMap>
#include <QTimer>
#include <QDialog>

namespace Ui {
	class main;
}

namespace Ui {
	class about;
}

class main_t : public QMainWindow
{
	Q_OBJECT

	public:
		main_t(QWidget *parent = 0);
		int usart_write(const uint8_t * ptr, int len);

	public:
		Ui::main *m_ui;

	private:
		QTimer timer;
		
		void init_buttons();

	signals:
		void sig_log(uint8_t lvl, const QString & log);
		void sig_msg(const QByteArray & ba);

	private slots:
		void slt_btn_about(int);
		void slt_timer();
		void slt_msg(const QByteArray & msg);
		void slt_btns();
};

class about_t : public QDialog
{
	Q_OBJECT

	public:
		about_t(QWidget *parent = 0);

	private:
		Ui::about *m_ui;
};

#endif
