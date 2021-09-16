#include "fieldwidget.h"

FieldWidget::Cross::Cross(int x, int y, QColor color)
    : QGraphicsItem(), _x(x), _y(y), _color(color) {}

QRectF FieldWidget::Cross::boundingRect() const {
  return QRectF(_x * gridStep + lineWidth, _y * gridStep + lineWidth,
                gridStep - 2 * lineWidth, gridStep - 2 * lineWidth);
}

void FieldWidget::Cross::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *widget) {
  painter->setPen(QPen(QBrush(_color), lineWidth));
  painter->drawLine(_x * gridStep + lineWidth, _y * gridStep + lineWidth,
                    (_x + 1) * gridStep - lineWidth,
                    (_y + 1) * gridStep - lineWidth);
  painter->drawLine((_x + 1) * gridStep - lineWidth, _y * gridStep + lineWidth,
                    _x * gridStep + lineWidth, (_y + 1) * gridStep - lineWidth);
}

void FieldWidget::Listener::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  _callback(_x, _y);
}

FieldWidget::Listener::Listener(int x, int y,
                                std::function<void(int, int)> callback)
    : QGraphicsItem(), _x(x), _y(y), _callback(std::move(callback)) {}

QRectF FieldWidget::Listener::boundingRect() const {
  return QRectF(_x * gridStep + lineWidth, _y * gridStep + lineWidth,
                gridStep - 2 * lineWidth, gridStep - 2 * lineWidth);
}

void FieldWidget::Listener::paint(QPainter *painter,
                                  const QStyleOptionGraphicsItem *option,
                                  QWidget *widget) {}

void FieldWidget::resizeEvent(QResizeEvent *event) {
  fitInView(gridMin, gridMin, gridMax, gridMax,
            Qt::AspectRatioMode::KeepAspectRatio);
}

FieldWidget::FieldWidget() : FieldWidget(QColor(Qt::darkMagenta), nullptr) {}

FieldWidget::FieldWidget(QColor gridColor) : FieldWidget(gridColor, nullptr) {}

FieldWidget::FieldWidget(QWidget *parent)
    : FieldWidget(QColor(Qt::darkMagenta), parent) {}

FieldWidget::FieldWidget(QColor gridColor, QWidget *parent)
    : QGraphicsView(parent) {
  setScene(new QGraphicsScene(this));
  for (int i = 0; i <= gridSize; ++i) {
    scene()->addLine(i * gridStep, gridMin, i * gridStep, gridMax,
                     QPen(QBrush(gridColor), lineWidth));
    scene()->addLine(gridMin, i * gridStep, gridMax, i * gridStep,
                     QPen(QBrush(gridColor), lineWidth));
  }
  auto listener = [this](int x, int y) { emit click(x, y); };
  for (int i = 0; i < gridSize; ++i)
    for (int j = 0; j < gridSize; ++j)
      scene()->addItem(new Listener(i, j, listener));
}

void FieldWidget::removeFieldItems(int x, int y) {
  QRectF rect(x * gridStep + lineWidth, y * gridStep + lineWidth,
              gridStep - 2 * lineWidth, gridStep - 2 * lineWidth);
  auto items = scene()->items(rect);
  if (items.size() > 1) {
    for (auto item = items.begin(); item < items.end() - 1; ++item) {
      scene()->removeItem(*item);
      delete *item;
    }
  }
}

void FieldWidget::addFieldCross(int x, int y, QColor color) {
  scene()->addItem(new Cross(x, y, color));
}

void FieldWidget::addFieldDot(int x, int y, QColor color) {
  scene()->addEllipse(x * gridStep + gridStep / 2 - lineWidth,
                      y * gridStep + gridStep / 2 - lineWidth, lineWidth * 2,
                      lineWidth * 2, QPen(QBrush(color), lineWidth * 2));
}
