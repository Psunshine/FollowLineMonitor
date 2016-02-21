#ifndef FOLLOWLINEMONITOR_H
#define FOLLOWLINEMONITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_followlinemonitor.h"
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include "serialthread.h"
#include <qmessagebox.h>
#include <qstatusbar.h>

class FollowLineMonitor : public QMainWindow
{
	Q_OBJECT

public:
	FollowLineMonitor(QWidget *parent = 0);
	~FollowLineMonitor();

private:
	void SerialInit();
	void RecTextBrowserInit();
	void SendTextBrowserInit();
	void ToHexStr(QByteArray, QString*);
	void ReverseToHexStr(QString,QByteArray*);

private:
	Ui::FollowLineMonitorClass ui;
	//����   
	QwtPlotCurve * curve;
	//QwtLegend *legend;
	QwtPlotGrid *grid;
	QwtPlotMarker *d_marker1, *d_marker2;
	QwtPlotZoomer *d_zoomer[2];
	QwtPlotPanner *d_panner;
	QwtPlotPicker *d_picker;
	
	QByteArray requestData;
	QTimer *timer;
	QByteArray sendarray;
	QByteArray sendarrayhex;

	double time[2000];
	double val[2000];
	int timeqqq;
	int aaa;
	int plotnum;

	//�Ƿ��������
	bool PlotFlag = false;
	//16���ƽ���
	bool hexRec = true;
	//16���Ʒ���
	bool hexSend = true;
	//�Ƿ�ֹͣ��ʾ����
	bool stopshow = false;

	//�����߳�
	SerialThread serialthread;
	SerialThread::Settings currentSettings;

	//״̬��
	QStatusBar *statusbar;
	QString statusStr;

	//ʮ�������ַ���
	QString RecStrHex;
	//���Ϳ�������ַ���
	QString InputStr;
	//��ʱ����ʱ����(ms)
	ulong timeout;
	//��ʱ���Ƿ�������
	bool timerrun;
	

private slots:
	void SearchCOM();
	void COMStatusUpdate();
	void StopCOM(int);
	void ClearRecData();
	void ClearSendData();
	void StopShow();
	void PlotSet();
	void RecHexShow();
	void SendHexShow();
	void ShowCOMErr(const QString &s);
	void GetRecData(const QByteArray rec);
	void SendData();
	void TimerSet();
};

#endif // FOLLOWLINEMONITOR_H
