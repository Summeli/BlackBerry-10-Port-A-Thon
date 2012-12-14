#include "boardutils.h"
#include <QtCore>
#include <QDebug>

BoardUtils::BoardUtils(QObject *parent) :
    QObject(parent)
{
    m_carmodel = new CarModel();
    //colors taken from gtlevel-0.4.c, line 143
    //transform them to QML color type
    m_colors.insert("DarkGreen", "darkgreen");
    m_colors.insert("purple3","mediumpurple");
    m_colors.insert("orange","orange");
    m_colors.insert("seagreen","seagreen");
    m_colors.insert("blue3","mediumblue");
    m_colors.insert("SteelBlue1","lightsteelblue");
    m_colors.insert("yellow","yellow");
    m_colors.insert("gold3","goldenrod");
    m_colors.insert("IndianRed1","indianred");
    m_colors.insert("SteelBlue4","steelblue");
    m_colors.insert("LightSkyBlue1","lightskyblue");
    m_colors.insert("SlateGray2","lightslategray");
    m_colors.insert("LavenderBlush1","lavenderblush");
    m_colors.insert("LemonChiffon1","lemonchiffon");
    m_colors.insert("MistyRose2","mistyrose");
    m_colors.insert("NavajoWhite4", "navajowhite");
    m_colors.insert("thistle2","thistle");
    m_colors.insert("red","red");

    //now let's load stuff from database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   // QString path = adjustPath("data/levels.db");
    QString path = "app/native/assets/levels.db";
    db.setDatabaseName(path);

    if (!db.open()) {
        //abort with error
        qDebug() << "Error opening database : " << db.lastError();
        //db.lastError();
    }
    QStringList tables = db.tables();
    if (!tables.contains("levels", Qt::CaseInsensitive)) {
        //abort with error
        QSqlError("sqlite", "data/levels.db doesn't contain tabe 'levels'");
    }
    m_query = QSqlQuery("", db);
    m_query.exec(QLatin1String("select count(id) from levels;"));
    m_query.next();
    m_numLevels = m_query.value(0).toInt();

    int lastLevel = m_sett.value("lastLevelID", 1).toInt();
    if(lastLevel<1 || lastLevel>m_numLevels) lastLevel=1;

    loadLevel(lastLevel);
}

void BoardUtils::debugPrintBoard()
{
    //printf here
    for(int i=0; i<BOARD_SIZE; i++) {
        for(int j=0; j<BOARD_SIZE; j++) {
            //print the matrix here:
            printf("%c", m_board[i][j]==0 ? '_' : 'X');
        }
        printf("\n");
    }
}

void BoardUtils::readBoardFromString(const QString& str)
{
    //do stuff
    //split string using space ' '
    m_carmodel->clear();

    memset(m_board, 0, BOARD_SIZE * BOARD_SIZE * sizeof(int));

    QStringList cars = str.split(' ');

    //first str is something else
    //not anymore: fix this annoying bug :D
    for(int i=0; i<cars.count(); i++) {
        //split it by ','
        QStringList s = cars.at(i).split(',');

        //now let's convert to int first 5
        Car *c = new Car(
                s.at(0).toInt(), //xpos
                s.at(1).toInt(), //ypos
                s.at(2).toInt(), //orientation
                s.at(3).toInt(), //isRed
                s.at(4).toInt(), //length
                colorTransform(s.at(5))          //color
                    );
        c->name = QString("car %1").arg(m_carmodel->rowCount());
        c->carIdx = m_carmodel->rowCount();
        m_carmodel->addCar(c);
        //now set the board

        if(c->orientation == 0)  { // vertically
            for(int j=0; j<c->length; j++) {
                m_board[c->x][c->y + j] = 1;
            }
        } else { //orientation=1, horizontally
            for(int j=0; j<c->length; j++) {
                m_board[c->x+j][c->y] = 1;
            }
        }
    }
    m_carCount = m_carmodel->rowCount();
    emit carModelChanged();
}

void BoardUtils::updateBoardMatrix()
{
    //update board matrix here
    memset(m_board, 0, BOARD_SIZE * BOARD_SIZE * sizeof(int));
    for(int i=0; i<m_carmodel->rowCount(); i++) {
        Car* c = m_carmodel->getCarAt(i);
        if(c->orientation == 0)  { // vertically
            for(int j=0; j<c->length; j++) {
                m_board[c->x][c->y + j] = 1;
            }
        } else { //orientation=1, horizontally
            for(int j=0; j<c->length; j++) {
                m_board[c->x+j][c->y] = 1;
            }
        }
    }
}

void BoardUtils::computeDragMinMaxXY(int carIdx)
{
    Q_UNUSED(carIdx);
    updateBoardMatrix();
    //parse matrix and set car drag min.max x,y
    for(int i=0; i<m_carmodel->rowCount(); i++) {
        Car* car = m_carmodel->getCarAt(i);
        //now let's parse the row or column of the matrix, depending of orientation
        // |X.CAR.| => already have orientation 1, so drag minX = 1, drag maxX = 5
        if(car->orientation == 1) { //horizontally
            int minX = car->x - 1;
            int maxX = car->x + car->length;
            while(minX >= 0 && m_board[minX][car->y]==0) minX--;
            while(maxX <= BOARD_SIZE - 1 && m_board[maxX][car->y]==0) maxX++;
            car->dragMinX = minX + 1;
            car->dragMaxX = maxX - car->length;
        } else { //same idea for vertically
            int minY = car->y - 1;
            int maxY = car->y + car->length;
            while(minY >= 0 && m_board[car->x][minY]==0) minY--;
            while(maxY <= BOARD_SIZE - 1 && m_board[car->x][maxY]==0) maxY++;
            car->dragMinY = minY + 1;
            car->dragMaxY = maxY - car->length ;
        }
    }
}

QString BoardUtils::colorTransform(const QString &color)
{
    //transform color from X11 color to QML color type
    return m_colors.value(color, "white");
}

int BoardUtils::getDragMinX(int carIdx)
{
    //do stuff
    return m_carmodel->getCarAt(carIdx)->dragMinX;
}

int BoardUtils::getDragMinY(int carIdx)
{
    //do stuff
    return m_carmodel->getCarAt(carIdx)->dragMinY;
}

int BoardUtils::getDragMaxX(int carIdx)
{
    //do stuff
    return m_carmodel->getCarAt(carIdx)->dragMaxX;
}

int BoardUtils::getDragMaxY(int carIdx)
{
    //do stuff
    return m_carmodel->getCarAt(carIdx)->dragMaxY;
}

void BoardUtils::updateCarPosition(int carIdx, int x, int y)
{
    Car *c = m_carmodel->getCarAt(carIdx);
    if(x>=0 && x<= BOARD_SIZE) c->x = x;
    if(y>=0 && y<= BOARD_SIZE) c->y = y;
    updateBoardMatrix();
}

QString BoardUtils::adjustPath(const QString &path)
{
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    if (!QDir::isAbsolutePath(path))
        return QCoreApplication::applicationDirPath()
                + QLatin1String("/../Resources/") + path;
#else
    QString pathInInstallDir;
    const QString applicationDirPath = QCoreApplication::applicationDirPath();
    pathInInstallDir = QString::fromAscii("%1/../%2").arg(applicationDirPath, path);

    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
#endif
    return path;
}

void BoardUtils::levelCleared(int levelId, int moves)
{
    //set personal best
    if(levelId==m_level.m_levelId) { //should always be true
        m_level.m_currentMoves = moves;
        if(moves < m_level.m_personalBest) {
            m_level.m_personalBest = moves;
            saveState();
        }
    }
}

void BoardUtils::saveState()
{
    m_sett.setValue("lastLevelID", m_level.m_levelId);
    m_sett.setValue("lastLevelMoves", m_level.m_currentMoves);
    m_sett.setValue("lastLevelState", getCurrentState());
    m_sett.sync();
}

QString BoardUtils::getCurrentState()
{
    QString ret("");
    for(int i=0; i<m_carCount; i++) {
        ret.append(m_carmodel->getCarAt(i)->toString()).append(" ");
    }
    return ret;
}

int BoardUtils::getCurrentLevel()
{
    return m_level.m_levelId;
}

void BoardUtils::startGame()
{
    //start game ???
}

void BoardUtils::restartLevel()
{
    //restart level
    readBoardFromString(m_level.m_levelData);
    m_level.m_currentMoves = 0;
    m_level.m_currentState = "";
}

bool BoardUtils::loadLevel(int levelId)
{
    if(levelId<=0) levelId = 1;
    if(levelId > m_numLevels) levelId = 1;

    //load level
    m_query.prepare(QLatin1String("select id, minMoves, initialState from levels where id=?"));
    m_query.addBindValue(levelId);
    m_query.exec();
    m_query.next();

    m_level.m_levelId = m_query.value(0).toInt();
    m_level.m_minMoves = m_query.value(1).toInt();
    m_level.m_levelData = m_query.value(2).toString();

    int lastLevel = m_sett.value("lastLevelID", -1).toInt();
    m_level.m_personalBest = m_sett.value(QString("level_%1").arg(lastLevel), 0).toInt();
    if(lastLevel == m_level.m_levelId) {
        //load from settings
        m_level.m_currentMoves = m_sett.value("lastLevelMoves", 0).toInt();
        m_level.m_currentState = m_sett.value("lastLevelState", "").toString();
    } else {
        m_level.m_currentMoves = 0;
        m_level.m_currentState = "";
    }
    if(!m_level.m_currentState.isEmpty()) {
        readBoardFromString(m_level.m_currentState);
    } else {
        readBoardFromString(m_level.m_levelData);
    }
    return true;
}

