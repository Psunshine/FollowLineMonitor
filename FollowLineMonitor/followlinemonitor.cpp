#include "followlinemonitor.h"

FollowLineMonitor::FollowLineMonitor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	QwtPlotCanvas *canvas = new QwtPlotCanvas();

	canvas->setBorderRadius(10);



	for (int i = 0; i < 255; i++)
	{
		time[i] = i;
		val[i] = i;
	}

	ui.setupUi(this);

	//ʵ����
	curve = new QwtPlotCurve("Acc_X");
	curve->setRenderHint(QwtPlotItem::RenderAntialiased);
	curve->setPen(Qt::darkGreen, 1);
	curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	curve->setYAxis(QwtPlot::yLeft);
	//��������
	curve->setSamples(time, val, 255);
	//�ӵ�plot��plot��IDE����
	curve->attach(ui.qwtPlot);

	//���ڳ�ʼ������
	SerialInit();
	RecTextBrowserInit();
	SendTextBrowserInit();
}

FollowLineMonitor::~FollowLineMonitor()
{

}

void FollowLineMonitor::COMStatusUpdate()
{
	QString tmp;
	int tmpnum;

	my_serialport->setPortName(ui.comboBoxNum->currentText());

	tmp = ui.comboBoxBps->currentText();
	tmpnum = tmp.toInt();
	my_serialport->setBaudRate(tmpnum);

	tmp = ui.comboBoxBits->currentText();
	tmpnum = tmp.toInt();
	my_serialport->setDataBits((QSerialPort::DataBits)tmpnum);

	tmp = ui.comboBoxParity->currentIndex();
	tmpnum = tmp.toInt();
	QSerialPort::Parity parity;
	switch (tmpnum)
	{
	case 0:
		parity = QSerialPort::NoParity;
		break;
	case 1:
		parity = QSerialPort::EvenParity;
		break;
	case 2:
		parity = QSerialPort::OddParity;
		break;
	default:
		break;
	}
	my_serialport->setParity(parity);

	tmp = ui.comboBoxStopBit->currentIndex();
	tmpnum = tmp.toInt();
	if (tmpnum == 1.5)tmpnum = 3;
	my_serialport->setStopBits((QSerialPort::StopBits)tmpnum);

	my_serialport->setFlowControl(QSerialPort::NoFlowControl);



	if (my_serialport->isOpen())
	{
		ui.COMButton->setText(QStringLiteral("�����ѹر�,����򿪴���"));
		ui.COMButton->setIcon(QIcon(QStringLiteral(":/FollowLineMonitor/Resources/OFF.png")));
		my_serialport->close();
	}
	else
	{
		ui.COMButton->setText(QStringLiteral("�����Ѵ�,����رմ���"));
		ui.COMButton->setIcon(QIcon(QStringLiteral(":/FollowLineMonitor/Resources/ON.png")));
		my_serialport->open(QIODevice::ReadWrite);

		//timer = new QTimer(this);
		////connect(timer, SIGNAL(timeout()), this, SLOT(updateAA()));
		//timer->start(10);
	}
}

void FollowLineMonitor::SerialInit()
{
	///���ںű���
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		QSerialPort serial;
		serial.setPort(info);
		if (serial.open(QIODevice::ReadWrite))
		{
			ui.comboBoxNum->addItem(info.portName());
			serial.close();
		}
	}
	///������
	QString BpsStr;
	ui.comboBoxBps->addItem(BpsStr.setNum(1200));
	ui.comboBoxBps->addItem(BpsStr.setNum(2400));
	ui.comboBoxBps->addItem(BpsStr.setNum(4800));
	ui.comboBoxBps->addItem(BpsStr.setNum(9600));
	ui.comboBoxBps->addItem(BpsStr.setNum(19200));
	ui.comboBoxBps->addItem(BpsStr.setNum(38400));
	ui.comboBoxBps->addItem(BpsStr.setNum(57600));
	ui.comboBoxBps->addItem(BpsStr.setNum(115200));
	ui.comboBoxBps->setCurrentIndex(7);//Ĭ����ʾ115200

	///����λ
	QString BitsStr;
	ui.comboBoxBits->addItem(BitsStr.setNum(5));
	ui.comboBoxBits->addItem(BitsStr.setNum(6));
	ui.comboBoxBits->addItem(BitsStr.setNum(7));
	ui.comboBoxBits->addItem(BitsStr.setNum(8));
	ui.comboBoxBits->setCurrentIndex(3);//Ĭ����ʾ8

	//У��λ
	QString ParityStr;
	ui.comboBoxParity->addItem(ParityStr.sprintf("None"));
	ui.comboBoxParity->addItem(ParityStr.sprintf("Even"));
	ui.comboBoxParity->addItem(ParityStr.sprintf("Odd"));

	//У��λ
	QString StopStr;
	ui.comboBoxStopBit->addItem(StopStr.setNum(1));
	ui.comboBoxStopBit->addItem(StopStr.setNum(1.5));
	ui.comboBoxStopBit->addItem(StopStr.setNum(2));

	//���ڶ���
	my_serialport = new QSerialPort();
	connect(ui.COMButton, SIGNAL(clicked()), this, SLOT(COMStatusUpdate()));
	connect(ui.comboBoxNum, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxBps, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxBits, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxParity, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxStopBit, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
}

void FollowLineMonitor::StopCOM(int i)
{
	if (my_serialport->isOpen())
	{
		ui.COMButton->setText(QStringLiteral("�����ѹر�,����򿪴���"));
		ui.COMButton->setIcon(QIcon(QStringLiteral(":/FollowLineMonitor/Resources/OFF.png")));
		my_serialport->open(QIODevice::ReadWrite);
	}
}

void FollowLineMonitor::RecTextBrowserInit()
{
	if (PlotFlag)
		ui.checkBoxPlot->setChecked(true);
	else
		ui.checkBoxPlot->setChecked(false);

	if (hexRec)
		ui.checkBoxHexShow->setChecked(true);
	else
		ui.checkBoxHexShow->setChecked(false);

	connect(ui.checkBoxPlot, SIGNAL(stateChanged()), this, SLOT(UpDateCheckConfig()));
	connect(ui.checkBoxHexShow, SIGNAL(stateChanged()), this, SLOT(UpDateCheckConfig()));
	connect(ui.clearRecButton, SIGNAL(clicked()), this, SLOT(ClearRecData()));
	ClearRecData();
}

void FollowLineMonitor::SendTextBrowserInit()
{
	if (hexSend)
		ui.checkBoxHexSend->setChecked(true);
	else
		ui.checkBoxHexSend->setChecked(false);

	connect(ui.checkBoxHexSend, SIGNAL(stateChanged()), this, SLOT(UpDateCheckConfig()));
	connect(ui.clearSendButton, SIGNAL(clicked()), this, SLOT(ClearSendData()));
	ClearSendData();
}

void FollowLineMonitor::ClearRecData()
{
	ui.Rectext->clear();
}

void FollowLineMonitor::ClearSendData()
{
	ui.Sendtext->clear();
}

void FollowLineMonitor::UpDateCheckConfig()
{
	if (ui.checkBoxPlot->isChecked())
		PlotFlag = true;
	else
		PlotFlag = false;

	if (ui.checkBoxHexShow->isChecked())
		hexRec = true;
	else
		hexRec = false;

	if (ui.checkBoxHexSend->isChecked())
		hexSend = true;
	else
		hexSend = false;
}