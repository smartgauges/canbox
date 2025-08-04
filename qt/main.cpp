#include <QApplication>
#include <QTreeView>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QLoggingCategory>
#include <QThread>

#include "main.h"
#include "ui_main.h"

#include "conf.h"
#include "qcar.h"

static main_t *g_main = NULL;

extern "C" {
	void canbox_park_process();
	void canbox_process();
	void car_process(uint8_t);
	void canbox_rx_process(uint8_t ch);
	void car_init(enum e_car_t car, struct key_cb_t * cb);

	uint8_t get_rear_delay_state(void)
	{
		return 1;
	}

	enum e_car_t conf_get_car(void)
	{
		return e_car_anymsg;
	}

	enum e_canbox_t conf_get_canbox(void)
	{
		return (e_canbox_t)g_main->m_ui->cb_canbox->currentData().value<int>();
	}

	struct usart_t * hw_usart_get(void)
	{
		return NULL;
	}

	struct can_t * hw_can_get_mscan(void)
	{
		return NULL;
	}
	
	uint8_t hw_can_get_msg_nums(struct can_t * can)
	{
		(void)can;
		return 0;
	}

	uint8_t hw_can_get_msg(struct can_t * can, struct msg_can_t * msg, uint8_t idx)
	{
		(void)can;
		(void)msg;
		(void)idx;
		return 0;
	}

	uint8_t hw_can_set_speed(struct can_t * can, int speed)
	{
		(void)can;
		(void)speed;
		return 0;
	}

	int hw_usart_write(struct usart_t *, const uint8_t * ptr, int len)
	{
		return g_main->usart_write(ptr, len);
	}
}


int main(int argc, char *argv[])
{
	QLoggingCategory::setFilterRules("*.debug=true\n""qt.*.debug=false");
	QApplication a(argc, argv);

	main_t w;
	g_main = &w;
	w.show();

	return a.exec();
}

main_t::main_t(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::main)
{
	m_ui->setupUi(this);

	connect(m_ui->btn_about, &QToolButton::clicked, this, &main_t::slt_btn_about);

	m_ui->cb_canbox->addItem("Raise VW PW", e_cb_raise_vw_pq);
	m_ui->cb_canbox->addItem("Raise VW MQB", e_cb_raise_vw_mqb);
	m_ui->cb_canbox->addItem("OD BMW NBT EVO", e_cb_od_bmw_nbt_evo);
	m_ui->cb_canbox->addItem("HiWorld VW MQB", e_cb_hiworld_vw_mqb);

	//com
	connect(m_ui->wdg_com, &wdg_com_t::sig_log, m_ui->wdg_dbg, &wdg_dbg_t::slt_log);
	connect(m_ui->wdg_com, SIGNAL(sig_msg(const QByteArray &)), this, SLOT(slt_msg(const QByteArray &)));
	connect(this, SIGNAL(sig_msg(const QByteArray &)), m_ui->wdg_com, SLOT(slt_msg(const QByteArray &)));

	init_buttons();

	car_init(e_car_qcar, NULL);

	connect(&timer, &QTimer::timeout, this, &main_t::slt_timer, Qt::QueuedConnection);
	timer.setSingleShot(false);
	timer.start(500);
}

void main_t::slt_btn_about(int)
{
	QString os_type = QSysInfo::kernelType();
	QString os_version = QSysInfo::kernelVersion();
	QString qt_version = QString("%1(%2)").arg(QT_VERSION_STR).arg(qVersion());
	QString version = QString("%1").arg(__DATE__);

	QMessageBox::about(this, tr("About qCanBox"),
		tr("<h2>qCanBox ") + version + "</h2>"
		"<p>qCanBox is a emulator of canbus boxes."
		"<p>Copyright: &copy; 2025 smartgauges@gmail.com."
		"<p>Home Page: <a href=\"https://github.com/smartgauges\">github.com/smartgauges</a>"
		"<p>OS:" + os_type + " " + os_version + "");
}

void main_t::init_buttons()
{
	connect(m_ui->btn_door_fl, SIGNAL(clicked()), this, SLOT(slt_btns()));
	connect(m_ui->btn_door_fr, SIGNAL(clicked()), this, SLOT(slt_btns()));
	connect(m_ui->btn_door_rl, SIGNAL(clicked()), this, SLOT(slt_btns()));
	connect(m_ui->btn_door_rr, SIGNAL(clicked()), this, SLOT(slt_btns()));
	connect(m_ui->btn_bonnet, SIGNAL(clicked()), this, SLOT(slt_btns()));
	connect(m_ui->btn_tailgate, SIGNAL(clicked()), this, SLOT(slt_btns()));

	connect(m_ui->btn_belt, SIGNAL(clicked()), this, SLOT(slt_btns()));

}

void main_t::slt_btns()
{
	qcar_state[e_fl_door] = m_ui->btn_door_fl->isChecked() ? 1 : 0;
	qcar_state[e_fr_door] = m_ui->btn_door_fr->isChecked() ? 1 : 0;
	qcar_state[e_rl_door] = m_ui->btn_door_rl->isChecked() ? 1 : 0;
	qcar_state[e_rr_door] = m_ui->btn_door_rr->isChecked() ? 1 : 0;
	qcar_state[e_bonnet] = m_ui->btn_bonnet->isChecked() ? 1 : 0;
	qcar_state[e_tailgate] = m_ui->btn_tailgate->isChecked() ? 1 : 0;

	qcar_state[e_belt] = m_ui->btn_belt->isChecked() ? 1 : 0;
}

void main_t::slt_timer()
{
	car_process(1);
	//canbox_park_process();
	canbox_process();
}	

int main_t::usart_write(const uint8_t * ptr, int len)
{
	QByteArray ba((char *)ptr, len);
	emit sig_msg(ba);

	return len;
}

void main_t::slt_msg(const QByteArray & msg)
{
	for(int i = 0; i < msg.size(); i++)
		canbox_rx_process(msg[i]);
}

