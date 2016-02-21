#include "followlinemonitor.h"

FollowLineMonitor::FollowLineMonitor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	statusbar = statusBar();
	setStatusBar(statusbar);

	//������Χ������
	ui.qwtPlot->setCanvasBackground(QColor(45,45,48));
	ui.qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
	ui.qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 300, 0);//y�᷶Χ����

	//�Ŵ���Сzoomer
	d_zoomer[0] = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft,
		ui.qwtPlot->canvas());
	d_zoomer[0]->setRubberBand(QwtPicker::RectRubberBand);
	d_zoomer[0]->setRubberBandPen(QColor(Qt::green));
	d_zoomer[0]->setTrackerMode(QwtPicker::AlwaysOff);
	d_zoomer[0]->setMousePattern(QwtEventPattern::MouseSelect2,	Qt::RightButton, Qt::ControlModifier);
	d_zoomer[0]->setMousePattern(QwtEventPattern::MouseSelect3,	Qt::RightButton);
	d_zoomer[0]->setEnabled(true);
	d_zoomer[0]->zoom(0);

	/*d_zoomer[1] = new QwtPlotZoomer(QwtPlot::xTop, QwtPlot::yRight,
		ui.qwtPlot->canvas());
	d_zoomer[1]->setTrackerMode(QwtPicker::AlwaysOff);
	d_zoomer[1]->setRubberBand(QwtPicker::NoRubberBand);
	d_zoomer[1]->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier);
	d_zoomer[1]->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);
	d_zoomer[1]->setEnabled(true);
	d_zoomer[1]->zoom(0);*/

	//�϶�panner
	d_panner = new QwtPlotPanner(ui.qwtPlot->canvas());
	d_panner->setMouseButton(Qt::MidButton);
	d_panner->setEnabled(true);

	//�����ȡpicker
	d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
		ui.qwtPlot->canvas());
	d_picker->setStateMachine(new QwtPickerDragPointMachine());
	d_picker->setRubberBandPen(QColor(Qt::green));
	d_picker->setRubberBand(QwtPicker::NoRubberBand);
	d_picker->setTrackerPen(QColor(Qt::white));
	d_picker->setEnabled(true);

	


	//ʵ����
	curve = new QwtPlotCurve("Acc_X");
	curve->setPen(QColor(0,122,204));
	curve->setStyle(QwtPlotCurve::Lines);
	
	//�ӵ�plot��plot��IDE����
	curve->attach(ui.qwtPlot);

	//���ڳ�ʼ������
	SerialInit();
	RecTextBrowserInit();
	SendTextBrowserInit();

	timeqqq = 0;
	aaa = 0;
}

FollowLineMonitor::~FollowLineMonitor()
{

}

void FollowLineMonitor::COMStatusUpdate()
{
	//��ȡ������������
	currentSettings.name = ui.comboBoxNum->currentText();
	currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
		ui.comboBoxBps->itemData(ui.comboBoxBps->currentIndex()).toInt());
	currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
		ui.comboBoxBits->itemData(ui.comboBoxBits->currentIndex()).toInt());
	currentSettings.parity = static_cast<QSerialPort::Parity>(
		ui.comboBoxParity->itemData(ui.comboBoxParity->currentIndex()).toInt());
	currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
		ui.comboBoxStopBit->itemData(ui.comboBoxStopBit->currentIndex()).toInt());

	if (serialthread.isRunning())
	{
		//������ڶ�ʱ���ͣ��ȹرն�ʱ��
		if (timerrun)
		{
			timer->stop();
			ui.pushButtonTimeSend->setText(QStringLiteral("������ʱ����"));
			timerrun = false;
		}

		statusStr = QStringLiteral("�����ѹرգ�\t\t\t\t\t\t\t\t��������:%1,��������:%2").arg(serialthread.RxNumber()).arg(serialthread.TxNumber());
		statusbar->showMessage(statusStr);

		ui.COMButton->setText(QStringLiteral("�����ѹر�,����򿪴���"));
		ui.COMButton->setIcon(QIcon(QStringLiteral(":/FollowLineMonitor/Resources/OFF.png")));
		serialthread.terminate();
		serialthread.wait();
	}
	else
	{
		if (serialthread.start(currentSettings))
		{
			statusStr = QStringLiteral("�����Ѵ򿪣�\t\t\t\t\t\t\t\t��������:%1,��������:%2").arg(serialthread.RxNumber()).arg(serialthread.TxNumber());
			statusbar->showMessage(statusStr);

			ui.COMButton->setText(QStringLiteral("�����Ѵ�,����رմ���"));
			ui.COMButton->setIcon(QIcon(QStringLiteral(":/FollowLineMonitor/Resources/ON.png")));
		}
	}
}

void FollowLineMonitor::SearchCOM()
{
	///���ںű���
	ui.comboBoxNum->clear();
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
		ui.comboBoxNum->addItem(info.portName());
}

void FollowLineMonitor::SerialInit()
{
	//��������
	SearchCOM();
	
	///������
	ui.comboBoxBps->addItem(QStringLiteral("9600"), QSerialPort::Baud1200);
	ui.comboBoxBps->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
	ui.comboBoxBps->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
	ui.comboBoxBps->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
	ui.comboBoxBps->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
	ui.comboBoxBps->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
	ui.comboBoxBps->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
	ui.comboBoxBps->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
	ui.comboBoxBps->setCurrentIndex(7);//Ĭ����ʾ115200

	///����λ
	ui.comboBoxBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
	ui.comboBoxBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
	ui.comboBoxBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
	ui.comboBoxBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
	ui.comboBoxBits->setCurrentIndex(3);//Ĭ����ʾ8

	//У��λ
	ui.comboBoxParity->addItem(QStringLiteral("None"), QSerialPort::NoParity);
	ui.comboBoxParity->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
	ui.comboBoxParity->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
	ui.comboBoxParity->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
	ui.comboBoxParity->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

	//ֹͣλ
	ui.comboBoxStopBit->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
	ui.comboBoxStopBit->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
	ui.comboBoxStopBit->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

	//�ź��������
	connect(ui.COMButton, SIGNAL(clicked()), this, SLOT(COMStatusUpdate()));
	connect(ui.comboBoxNum, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxBps, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxBits, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxParity, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.comboBoxStopBit, SIGNAL(currentIndexChanged(int)), this, SLOT(StopCOM(int)));
	connect(ui.ResearchCom, SIGNAL(clicked()), this, SLOT(SearchCOM()));
	connect(&serialthread, SIGNAL(error(QString)), this, SLOT(ShowCOMErr(QString)));

	statusStr = QStringLiteral("�����ѹرգ�\t\t\t\t\t\t\t\t��������:%1,��������:%2").arg(0).arg(0);
	statusbar->showMessage(statusStr);
}

void FollowLineMonitor::StopCOM(int i)
{
	if (serialthread.isRunning())
	{
		statusStr = QStringLiteral("�����ѹرգ�\t\t\t\t\t\t\t\t��������:%1,��������:%2").arg(serialthread.RxNumber()).arg(serialthread.TxNumber());
		statusbar->showMessage(statusStr);

		ui.COMButton->setText(QStringLiteral("�����ѹر�,����򿪴���"));
		ui.COMButton->setIcon(QIcon(QStringLiteral(":/FollowLineMonitor/Resources/OFF.png")));
		serialthread.terminate();
		serialthread.wait();
	}
}

void FollowLineMonitor::RecTextBrowserInit()
{
	connect(ui.checkBoxStopShow, SIGNAL(stateChanged(int)), this, SLOT(StopShow()));
	connect(ui.checkBoxPlot, SIGNAL(stateChanged(int)), this, SLOT(PlotSet()));
	connect(ui.checkBoxHexShow, SIGNAL(stateChanged(int)), this, SLOT(RecHexShow()));
	connect(ui.checkBoxHexSend, SIGNAL(stateChanged(int)), this, SLOT(SendHexShow()));
	connect(ui.clearRecButton, SIGNAL(clicked()), this, SLOT(ClearRecData()));
	connect(&serialthread, SIGNAL(recdata(QByteArray)), this, SLOT(GetRecData(QByteArray)));
	ClearRecData();
}

void FollowLineMonitor::SendTextBrowserInit()
{
	timerrun = false;
	timer = new QTimer(this);
	connect(ui.pushButtonTimeSend, SIGNAL(clicked()), this, SLOT(TimerSet()));
	connect(timer, SIGNAL(timeout()), this, SLOT(SendData()));
	connect(ui.checkBoxHexSend, SIGNAL(stateChanged()), this, SLOT(UpDateCheckConfig()));
	connect(ui.clearSendButton, SIGNAL(clicked()), this, SLOT(ClearSendData()));
	connect(ui.SendButton, SIGNAL(clicked()), this, SLOT(SendData()));
	ClearSendData();
}

void FollowLineMonitor::ClearRecData()
{
	requestData.clear();
	RecStrHex.clear();
	ui.Rectext->clear();
}

void FollowLineMonitor::ClearSendData()
{
	sendarray.clear();
	sendarrayhex.clear();
	ui.Sendtext->clear();
}

void FollowLineMonitor::StopShow()
{
	if (ui.checkBoxStopShow->isChecked())
		stopshow = true;
	else
		stopshow = false;
}

void FollowLineMonitor::PlotSet()
{
	if (ui.checkBoxPlot->isChecked())
		PlotFlag = true;
	else
		PlotFlag = false;
}

void FollowLineMonitor::RecHexShow()
{
	if (ui.checkBoxHexShow->isChecked())
	{
		hexRec = true;
		ui.Rectext->setText(RecStrHex);
	}
	else
	{
		hexRec = false;
		ui.Rectext->setText(requestData);
	}
}

void FollowLineMonitor::SendHexShow()
{
	//�Ƿ�ʮ�����Ʒ���ѡ��
	if (ui.checkBoxHexSend->isChecked())
	{
		sendarray = ui.Sendtext->toPlainText().toLocal8Bit();
		QString str;
		ToHexStr(sendarray, &str);
		sendarrayhex = str.toLocal8Bit();

		hexSend = true;
		ui.Sendtext->setText(sendarrayhex);
	}
	else
	{
		sendarrayhex = ui.Sendtext->toPlainText().toLocal8Bit();
		QString str = sendarrayhex;
		ReverseToHexStr(str, &sendarray);

		hexSend = false;
		ui.Sendtext->setText(sendarray);
	}
}

void FollowLineMonitor::ShowCOMErr(const QString &s)
{
	QMessageBox::critical(this, QStringLiteral("���ڴ���"), s);
}

void FollowLineMonitor::GetRecData(const QByteArray rec)
{
	//��ȡ�µĴ�������׷����ĩβ
	requestData.append(rec);

	//���������ʾ������ͣˢ���ı�������Ӱ��ϴ�
	if (!stopshow)
	{
		if (hexRec)
		{
			ToHexStr(requestData, &RecStrHex);
			ui.Rectext->setText(RecStrHex);
		}
		else
			ui.Rectext->setText(requestData);

		//�������ĩβ��ʵ���Զ�����
		ui.Rectext->moveCursor(QTextCursor::End);
	}
	statusStr=QStringLiteral("�����Ѵ򿪣�\t\t\t\t\t\t\t\t��������:%1,��������:%2").arg(serialthread.RxNumber()).arg(serialthread.TxNumber());
	statusbar->showMessage(statusStr);
}

//ʮ��������ʾ
void FollowLineMonitor::ToHexStr(QByteArray input, QString *output)
{
	if (input.length() != 0)
	{
		QString tmpStr;
		for (int i = 0; i < input.length(); i++)
		{
			if (output->size()==0)
				tmpStr.sprintf("%2X", (unsigned char)input.at(i));
			else
				tmpStr.sprintf(" %2X", (unsigned char)input.at(i));
			output->append(tmpStr);
		}
	}
}

void FollowLineMonitor::ReverseToHexStr(QString input, QByteArray *output)
{
	if (!input.isEmpty())
	{
		QStringList list;
		QStringList list1;
		//�ָ�����
		list = input.split(" ");
		//ȡ����Ч����
		for each (QString str in list)
		{
			//��Ч�ַ�
			if ((str.at(0) >= '0') && (str.at(0) <= '9')
				|| (str.at(0) >= 'a') && (str.at(0) <= 'f')
				|| (str.at(0) >= 'A') && (str.at(0) <= 'F'))
			{
				if (str.size() > 1)
				{
					if ((str.at(1) >= '0') && (str.at(1) <= '9')
						|| (str.at(1) >= 'a') && (str.at(1) <= 'f')
						|| (str.at(1) >= 'A') && (str.at(1) <= 'F'))
					{
						QString tmp;
						tmp.append(str.at(0));
						tmp.append(str.at(1));
						tmp.toUpper();
						list1.append(tmp);
					}
				}
				else
				{
					QString tmp;
					tmp.append(str.at(0));
					tmp.toUpper();
					list1.append(tmp);
				}
			}
		}

		output->clear();
		char tmpchar;
		//ת��Ϊ��ֵ
		for each (QString str in list1)
		{
			char a0 = str.at(0).toLatin1(), a1 = 0;
			if ((a0 >= '0') && (a0 <= '9'))
				a0 = a0 - '0';
			else if ((a0 >= 'A') && (a0 <= 'F'))
				a0 = a0 - 'A' + 10;
			else
				a0 = a0 - 'a' + 10;

			if (str.size() > 1)
			{
				a1 = str.at(1).toLatin1();
				if ((a1 >= '0') && (a1 <= '9'))
					a1 = a1 - '0';
				else if ((a1 >= 'A') && (a1 <= 'F'))
					a1 = a1 - 'A' + 10;
				else
					a1 = a1 - 'a' + 10;

				tmpchar = a0 * 16 + a1;
			}
			else
			{
				tmpchar = a0;
			}
			output->push_back(tmpchar);

		}
	}
}

void FollowLineMonitor::SendData()
{	
	timeqqq++;
	if (timeqqq >= 2000)
	{
		aaa++;
		//timeqqq = 0;

		for (int i = 0; i < 1999-50; i++)
		{
			time[i] = time[i + 50];
			val[i] = val[i + 50];
		}
		time[1999 - 50 + timeqqq % 50]++;
		val[1999 - 50 + timeqqq % 50] = 100 + 100 * sin(timeqqq*6.28 / 255);
		plotnum = 2000 - 50 + timeqqq % 50;
	}
	else
	{
		val[timeqqq] = 100 + 100 * sin(timeqqq*6.28 / 255);
		time[timeqqq] = timeqqq;
		plotnum = timeqqq;
	}

	//��������
	curve->setSamples(time, val, plotnum);

	InputStr = ui.Sendtext->toPlainText();

	if (InputStr.size() == 0)
		return;
	else
	{
		if (!hexSend)//�ַ�������
		{						
			if (serialthread.SendArray(sendarray))
			{
				statusStr = QStringLiteral("�����Ѵ򿪣�\t\t\t\t\t\t\t\t��������:%1,��������:%2").arg(serialthread.RxNumber()).arg(serialthread.TxNumber());
				statusbar->showMessage(statusStr);
			}
		}
		else//ʮ�����Ʒ���
		{
			ReverseToHexStr(InputStr, &sendarrayhex);

			if (serialthread.SendArray(sendarrayhex))
			{
				statusStr = QStringLiteral("�����Ѵ򿪣�\t\t\t\t\t\t\t\t��������:%1,��������:%2").arg(serialthread.RxNumber()).arg(serialthread.TxNumber());
				statusbar->showMessage(statusStr);
			}
		}
	}
}

void FollowLineMonitor::TimerSet()
{
	timeout = ui.lineEditSendTimeout->text().toInt();
	if (!timerrun)
	{
		timer->start(timeout);
		ui.pushButtonTimeSend->setText(QStringLiteral("ֹͣ��ʱ����"));
		timerrun = true;
	}
	else
	{
		timer->stop();
		ui.pushButtonTimeSend->setText(QStringLiteral("ֹͣ��ʱ����"));
		timerrun = false;
	}
}

