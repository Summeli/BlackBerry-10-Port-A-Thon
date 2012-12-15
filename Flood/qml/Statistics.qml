/****************************************************************************
**
** Copyright (C) 2011 Kirill (spirit) Klochkov.
** Contact: klochkov.kirill@gmail.com
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

import QtQuick 1.0

Item {
    signal cleared()
    signal saved()

    function clearStatistics()
    {
        var db = internal.openDatabaseConnection();
        db.transaction(function(tx) { tx.executeSql('DELETE FROM statistics') });
        cleared();
    }

    function saveStatistics(boardSize, steps, win)
    {
        var db = internal.openDatabaseConnection();
        var bestBoardSteps = win > 0 ? steps : 0;
        db.transaction(function(tx) { tx.executeSql('INSERT INTO statistics VALUES(?, ?, ?)', [ boardSize, bestBoardSteps, win ]) });
        saved();
    }

    function bestBoard(boardSize)
    {
        var res = 0;
        var db = internal.openDatabaseConnection();
        db.transaction(function(tx) {
            var rows = tx.executeSql('SELECT MIN(steps) AS steps FROM statistics WHERE boardSize = ?', [ boardSize ]);
            if (rows.rows.length)
                res = rows.rows.item(0).steps;
        });
        return res;
    }

    function wins(boardSize)
    {
        var res = 0;
        var db = internal.openDatabaseConnection();
        db.transaction(function(tx) {
            var rows = tx.executeSql('SELECT COUNT(win) AS wins FROM statistics WHERE boardSize = ? AND win = 1', [ boardSize ]);
            if (rows.rows.length)
                res = rows.rows.item(0).wins;
        });
        return res;
    }

    function gamesPlayed(boardSize)
    {
        var res = 0;
        var db = internal.openDatabaseConnection();
        db.transaction(function(tx) {
            var rows = tx.executeSql('SELECT COUNT(1) AS gamesPlayed FROM statistics WHERE boardSize = ?', [ boardSize ]);
            if (rows.rows.length)
                res = rows.rows.item(0).gamesPlayed;
        });
        return res;
    }

    QtObject {
        id: internal

        function openDatabaseConnection()
        {
            var db = openDatabaseSync("floodStatisticsDB", "0.1", "The flood game statistics database.", 1000000);
            db.transaction(function(tx) { tx.executeSql('CREATE TABLE IF NOT EXISTS statistics(boardSize INTEGER, steps INTEGER, win INTEGER)') });
            return db;
        }
    }
}
