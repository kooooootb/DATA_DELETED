Cells.cfg : параметры клеточного поля
Формат : {количество клеток по вертикали} {количество клеток по вертикали} {тип клетки(0,1,2,3), x, y} {тип клетки(0,1,2,3), x, y} {тип клетки(0,1,2,3), x, y}...

Opers.cfg : параметры оперативников
Формат : {имя}\n{начальный x} {начальный y} {коэф времени перезарядки} {сила} {точность}

В файле Parameters.h:
const int CELLSIZE = 20 -> размер клетки в пикселях(лучше четное)
const int WINDOWWIDTH_AMOUNT = 3;//odd, crashes if WINDOWWIDTH > 1920 -> количество клеток по горизонтали(лучше нечетное)
const int WINDOWHEIGHT_AMOUNT = 3;//odd, crashes if WINDOWHEIGHT > 1080 -> количество клеток по вертикали(лучше нечетное)
const int WINDOWWIDTH = CELLSIZE * WINDOWWIDTH_AMOUNT -> размер окна в пискелях по горизонтали
const int WINDOWHEIGHT = CELLSIZE * WINDOWHEIGHT_AMOUNT -> размер окна в пискелях по вертикали
