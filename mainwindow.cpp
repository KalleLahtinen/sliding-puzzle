#include "custompixmap.hpp"
#include "mainwindow.hpp"
#include <QVBoxLayout>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include "clickableview.hpp"
#include <QThread>
#include <QEventLoop>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set the central widget
    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // Create a Scene and custom ClickableView for displaying puzzle image parts
    scene_ = new QGraphicsScene(this);
    puzzleView_ = new ClickableView(scene_);
    puzzleView_->setGeometry(0, 0, 0, 0);

    // Add GraphicsView to main layout
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(puzzleView_);

    // Create the puzzle display from user image
    displayPuzzle();

    // Connect the custom view click handler to action
    connect(puzzleView_, &ClickableView::mouseClickDetected, this, &MainWindow::slideToEmpty);}

MainWindow::~MainWindow() {}

void MainWindow::slideToEmpty(const QPoint &clickPos)
{
    // Clicked position as position on puzzle "grid"
    // on range (0-IMAGE_COLUMNS, 0-IMAGE_ROWS)
    int x = clickPos.x() / (sectionWidth_ + IMAGE_MARGIN);
    int y = clickPos.y() / (sectionHeight_ + IMAGE_MARGIN);

    // Get the clicked element
    QGraphicsItem* clicked_item = puzzleView_->itemAt(clickPos);

    // If there was a puzzle piece and clicked location
    if (clicked_item) {
        // If the clicked puzzle piece was on one of the four sides of the empty slot
        if ((x == empty_tile_location_.x() && abs(y - empty_tile_location_.y()) == 1)
            || (y == empty_tile_location_.y() && abs(x - empty_tile_location_.x()) == 1)) {

            CustomPixmap* animItem = qgraphicsitem_cast<CustomPixmap*>(clicked_item);
            // Switch the clicked piece with empty slot using QPropertyAnimation
            anim_ = new QPropertyAnimation(animItem, "pos");

            anim_->setDuration(SLIDE_TIME);
            anim_->setStartValue(clicked_item->pos());
            anim_->setEndValue(QPointF((empty_tile_location_.x() * IMAGE_MARGIN) + (empty_tile_location_.x() * sectionWidth_),
                                       (empty_tile_location_.y() * IMAGE_MARGIN) + (empty_tile_location_.y() * sectionHeight_)));
            anim_->start();

            // Disable clicks for the duration of the switch to prevent unwanted behavior
            puzzleView_->setEnabled(false);
            QTimer::singleShot(SLIDE_TIME, this, [=](){puzzleView_->setEnabled(true);});

            // Set empty tile location to puzzle piece's old location
            empty_tile_location_.setX(x);
            empty_tile_location_.setY(y);
        }
    } else {
        qDebug() << "No item was clicked.";
    }
}

void MainWindow::divideImage(const QImage &originalImage, int rows, int columns,
                             QVector<QVector<QGraphicsPixmapItem*>> &subimages) {
    // Get the size for subimages
    sectionWidth_ = originalImage.width() / columns;
    sectionHeight_ = originalImage.height() / rows;

    // Set correct vector size
    subimages.resize(rows);

    for (int i = 0; i < rows; ++i) {
        subimages[i].resize(columns);

        for (int j = 0; j < columns; ++j) {
            if (i == rows - 1 && j == columns - 1) {
                empty_tile_location_.setX(columns - 1);
                empty_tile_location_.setY(rows - 1);
                continue;
            }
            int x = j * sectionWidth_;
            int y = i * sectionHeight_;

            // Create CustomPixmap items from sections of puzzle image.
            // Derived class CustomPixmap enables animating item position
            QImage subimage = originalImage.copy(x, y, sectionWidth_, sectionHeight_);
            CustomPixmap* clickableItem = new CustomPixmap(QPixmap::fromImage(subimage));

            // Add created subImages into grid formation with margins
            // resembling original image on scene_ container
            scene_->addItem(clickableItem);
            QPoint point((j * IMAGE_MARGIN) + (j * sectionWidth_), (i * IMAGE_MARGIN) + (i * sectionHeight_));
            clickableItem->setPos(point);
        }
    }
}

void MainWindow::displayPuzzle()
{
    QImage originalImage(IMAGEPATH);

    // Check that the image could be loaded
    if (originalImage.isNull()) {
        qDebug() << "Failed to load the image";
        return;
    }

    // Get the needed image aspect ratio for creating square sub-images
    float aspectRatio = static_cast<float>(IMAGE_COLUMNS) / static_cast<float>(IMAGE_ROWS);

    // Scale the image to the correct aspect ratio
    int newHeigth = static_cast<int>(originalImage.height() * 0.15);
    int newWidth = newHeigth * aspectRatio;
    QImage scaledImage = originalImage.scaled(newWidth, newHeigth, Qt::IgnoreAspectRatio);

    QVector<QVector<QGraphicsPixmapItem*>> subimages = {};
    divideImage(scaledImage, IMAGE_ROWS, IMAGE_COLUMNS, subimages);
}
