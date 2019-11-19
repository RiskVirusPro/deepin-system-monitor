/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2018 Deepin, Inc.
 *               2011 ~ 2018 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <DApplication>
#include <DApplicationHelper>
#include <DHiDPIHelper>
#include <DPalette>
#include <QDebug>
#include <QPainter>

#include "constant.h"
#include "cpu_monitor.h"
#include "process/system_monitor.h"
#include "settings.h"
#include "smooth_curve_generator.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

using namespace Utils;

CpuMonitor::CpuMonitor(QWidget *parent)
    : QWidget(parent)
{
    iconDarkImage = DHiDPIHelper::loadNxPixmap(Utils::getQrcPath("icon_cpu_dark.svg"));
    iconLightImage = DHiDPIHelper::loadNxPixmap(Utils::getQrcPath("icon_cpu_light.svg"));

    int statusBarMaxWidth = Utils::getStatusBarMaxWidth();
    setFixedSize(statusBarMaxWidth, 250);
    waveformsRenderOffsetX = (statusBarMaxWidth - 140) / 2;

    cpuPercents = new QList<double>();
    for (int i = 0; i < pointsNumber; i++) {
        cpuPercents->append(0);
    }

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(render()));
    timer->start(30);

    DApplicationHelper *dAppHelper = DApplicationHelper::instance();
    connect(dAppHelper, &DApplicationHelper::themeTypeChanged, this, &CpuMonitor::changeTheme);
    m_themeType = dAppHelper->themeType();

    auto *sysmon = SystemMonitor::instance();
    if (sysmon) {
        connect(sysmon, &SystemMonitor::cpuStatInfoUpdated, this, &CpuMonitor::updateStatus);
    }
}

CpuMonitor::~CpuMonitor()
{
    delete cpuPercents;
    delete timer;
}

void CpuMonitor::changeTheme(DApplicationHelper::ColorType themeType)
{
    m_themeType = themeType;
}

void CpuMonitor::render()
{
    if (animationIndex < animationFrames) {
        animationIndex++;

        repaint();
    } else {
        timer->stop();
    }
}

void CpuMonitor::updateStatus(qreal cpuPercent, QVector<qreal>)
{
    cpuPercents->append(cpuPercent);

    if (cpuPercents->size() > pointsNumber) {
        cpuPercents->pop_front();
    }

    QList<QPointF> points;

    double cpuMaxHeight = 0;
    for (int i = 0; i < cpuPercents->size(); i++) {
        if (cpuPercents->at(i) > cpuMaxHeight) {
            cpuMaxHeight = cpuPercents->at(i);
        }
    }

    for (int i = 0; i < cpuPercents->size(); i++) {
        if (cpuMaxHeight < cpuRenderMaxHeight) {
            points.append(QPointF(i * 5, cpuPercents->at(i)));
        } else {
            points.append(QPointF(i * 5, cpuPercents->at(i) * cpuRenderMaxHeight / cpuMaxHeight));
        }
    }

    cpuPath = SmoothCurveGenerator::generateSmoothCurve(points);

    if (cpuPercents->last() != cpuPercents->at(cpuPercents->size() - 2)) {
        animationIndex = 0;
        timer->start(30);
    }
}

void CpuMonitor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    switch (m_themeType) {
        case DApplicationHelper::LightType:
            ringForegroundColor = "#2CA7F8";
            ringForegroundOpacity = 1;

            ringBackgroundColor = "#000000";
            ringBackgroundOpacity = 0.05;

            iconImage = iconLightImage;
            break;
        case DApplicationHelper::DarkType:
            ringForegroundColor = "#2CA7F8";
            ringForegroundOpacity = 1;

            ringBackgroundColor = "#2CA7F8";
            ringBackgroundOpacity = 0.1;

            iconImage = iconDarkImage;
            break;
        default:
            break;
    }

    // init colors
    auto *dAppHelper = DApplicationHelper::instance();
    auto palette = dAppHelper->applicationPalette();
    // TODO: change color
    textColor = palette.color(DPalette::Text);
    numberColor = palette.color(DPalette::Text);

    QFont font = painter.font();
    font.setPointSize(20);
    font.setWeight(QFont::Light);

    QFontMetrics fm(font);
    int titleWidth = fm.width(DApplication::translate("Process.Graph.View", "CPU"));

    int iconTitleWidth = iconImage.width() + iconPadding + titleWidth;

    painter.drawPixmap(QPoint((rect().x() + (rect().width() - iconTitleWidth) / 2) -
                                  titleAreaPaddingX - paddingRight,
                              iconRenderOffsetY),
                       iconImage);

    painter.setFont(font);
    painter.setPen(QPen(textColor));
    painter.drawText(QRect((rect().x() + (rect().width() - iconTitleWidth) / 2) +
                               iconImage.width() + iconPadding - titleAreaPaddingX - paddingRight,
                           rect().y() + titleRenderOffsetY, titleWidth, 30),
                     Qt::AlignCenter, DApplication::translate("Process.Graph.View", "CPU"));

    double percent = (cpuPercents->at(cpuPercents->size() - 2) +
                      easeInOut(animationIndex / animationFrames) *
                          (cpuPercents->last() - cpuPercents->at(cpuPercents->size() - 2)));

    setFontSize(painter, 15);
    painter.setPen(QPen(numberColor));
    painter.drawText(
        QRect(rect().x() - paddingRight, rect().y() + percentRenderOffsetY, rect().width(), 30),
        Qt::AlignCenter, QString("%1%").arg(QString::number(percent, 'f', 1)));

    drawLoadingRing(painter, rect().x() + rect().width() / 2 - paddingRight,
                    rect().y() + ringRenderOffsetY, ringRadius, ringWidth, 300, 150,
                    ringForegroundColor, ringForegroundOpacity, ringBackgroundColor,
                    ringBackgroundOpacity, percent / 100);

    painter.translate(waveformsRenderOffsetX, waveformsRenderOffsetY);
    painter.scale(1, -1);

    painter.setPen(QPen(QColor("#2CA7F8"), 2));
    painter.drawPath(cpuPath);
}
