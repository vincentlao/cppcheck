/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2016 Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "erroritem.h"

QErrorPathItem::QErrorPathItem(const ErrorLogger::ErrorMessage::FileLocation &loc)
    : file(QString::fromStdString(loc.getfile(false)))
    , line(loc.line)
    , col(loc.col)
    , info(QString::fromStdString(loc.getinfo()))
{
}

QErrorPathItem::QErrorPathItem(const QErrorPathItem &e)
    : file(e.file)
    , line(e.line)
    , col(e.col)
    , info(e.info)
{
}

ErrorItem::ErrorItem()
    : severity(Severity::none)
    , inconclusive(false)
    , cwe(-1)
{
}

ErrorItem::ErrorItem(const ErrorLogger::ErrorMessage &errmsg)
    : errorId(QString::fromStdString(errmsg._id))
    , severity(errmsg._severity)
    , inconclusive(errmsg._inconclusive)
    , summary(QString::fromStdString(errmsg.shortMessage()))
    , message(QString::fromStdString(errmsg.verboseMessage()))
    , cwe(errmsg._cwe.id)
{
    for (std::list<ErrorLogger::ErrorMessage::FileLocation>::const_iterator loc = errmsg._callStack.begin();
         loc != errmsg._callStack.end();
         ++loc) {
        errorPath << QErrorPathItem(*loc);
    }
}


QString ErrorItem::ToString() const
{
    QString str = errorPath.back().file + " - " + errorId + " - ";
    if (inconclusive)
        str += "inconclusive ";
    str += GuiSeverity::toString(severity) +"\n";
    str += summary + "\n";
    str += message + "\n";
    for (int i = 0; i < errorPath.size(); i++) {
        str += "  " + errorPath[i].file + ": " + QString::number(errorPath[i].line) + "\n";
    }
    return str;
}
