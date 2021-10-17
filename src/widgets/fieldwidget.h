#pragma once
#include <QGraphicsItem>
#include <QGraphicsView>
class FieldWidget : public QGraphicsView {
  Q_OBJECT
  static constexpr int gridMin = 0;
  static constexpr int gridMax = 100;
  static constexpr int gridSize = 10;
  static constexpr int gridStep = (gridMax - gridMin) / gridSize;
  static constexpr int lineWidth = 1;

  class Cross : public QGraphicsItem {
    int _x;
    int _y;
    QColor _color;

  public:
    explicit Cross(int x, int y, QColor color = QColor(Qt::black));
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
  };
  class Listener : public QGraphicsItem {
    int _x;
    int _y;
    std::function<void(int, int)> _callback;

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

  public:
    explicit Listener(int x, int y, std::function<void(int, int)> callback);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
  };
  void resizeEvent(QResizeEvent *event) override;

public:
  explicit FieldWidget();
  explicit FieldWidget(QColor gridColor);
  explicit FieldWidget(QWidget *parent);
  explicit FieldWidget(QColor gridColor, QWidget *parent);
public slots:
  void removeFieldItems(int x, int y);
  void addFieldCross(int x, int y, QColor color = QColor(Qt::black));
  void addFieldDot(int x, int y, QColor color = QColor(Qt::black));
signals:
  void click(int x, int y);
  void removeFieldItemsSignal(int x, int y);
  void addFieldCrossSignal(int x, int y, Qt::GlobalColor color = Qt::black);
  void addFieldDotSignal(int x, int y, Qt::GlobalColor color = Qt::black);
};
