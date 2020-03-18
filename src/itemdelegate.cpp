/*
 * Copyright (C) 2018 ~ 2019 PandaOS.
 *
 * Author:     rekols <rekols@foxmail.com>
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

#include "itemdelegate.h"
#include "listmodel.h"
#include <QApplication>

const QRect itemBoundingRect(const QRect &itemRect)
{
    const int w = itemRect.width();
    const int h = itemRect.height();
    const int sub = qAbs((w - h) / 2);

    if (w == h)
        return itemRect;
    else if (w > h)
        return itemRect - QMargins(sub, 0, sub, 0);
    else
        return itemRect - QMargins(0, 0, 0, sub * 2);
}

const QPixmap getThemeIcon(const QString &iconName, const int size)
{
    const auto ratio = qApp->devicePixelRatio();
    QPixmap pixmap;

    const QIcon icon = QIcon::fromTheme(iconName, QIcon::fromTheme("application-x-desktop"));
    pixmap = icon.pixmap(QSize(size, size));

    if (qFuzzyCompare(pixmap.devicePixelRatioF(), 1.0)) {
        pixmap = pixmap.scaled(QSize(size, size) * ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixmap.setDevicePixelRatio(ratio);
    }

    return pixmap;
}

ItemDelegate::ItemDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{

}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = itemBoundingRect(option.rect);

    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setPen(Qt::transparent);

    if(option.state & QStyle::State_MouseOver) {
        painter->setBrush(QColor(255, 255, 255, 50));
        painter->drawRoundedRect(rect, 10, 10);
    }

    int iconSize = rect.width() * 0.4;
    QPixmap iconPixmap = getThemeIcon(index.data(ListModel::AppIconRole).toString(), iconSize);
    int iconLeftMargin = (rect.width() - iconPixmap.width()) / 2;
    int iconTopMargin = rect.height() * 0.12;
    painter->drawPixmap(rect.x() + iconLeftMargin,
                        rect.y() + iconTopMargin,
                        iconPixmap.width(),
                        iconPixmap.height(), iconPixmap);

    QTextOption appNameOption;
    appNameOption.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    appNameOption.setWrapMode(QTextOption::WordWrap);

    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::black);

    int textTopMargin = rect.width() * 0.63;
    QRect textRect = QRect(rect.x(),
                           rect.y() + textTopMargin,
                           rect.width(),
                           rect.height());
    QString appName = index.data(ListModel::AppNameRole).toString();
    painter->setPen(Qt::white);
    painter->drawText(textRect, Qt::TextWordWrap | Qt::AlignHCenter, appName);
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    return index.data(ListModel::ItemSizeHintRole).toSize();
}
