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
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

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

private:
	Ui::FollowLineMonitorClass ui;
	//����   
	QwtPlotCurve * curve;
	//QwtLegend *legend;
	QwtPlotGrid *grid;
	QwtPlotMarker *d_marker1, *d_marker2;

	QSerialPort *my_serialport;
	QByteArray requestData;
	QTimer *timer;

	double time[255];
	double val[255];

	//�Ƿ��������
	bool PlotFlag = false;
	//16���ƽ���
	bool hexRec = true;
	//16���Ʒ���
	bool hexSend = true;

private slots:
	void COMStatusUpdate();
	void StopCOM(int);
	void ClearRecData();
	void ClearSendData();
	void UpDateCheckConfig();
};

#endif // FOLLOWLINEMONITOR_H
