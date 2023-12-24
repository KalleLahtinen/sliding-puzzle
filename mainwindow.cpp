#include "mainwindow.hpp"
#include <QVBoxLayout>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include "clickableview.hpp"

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
    display_puzzle();

    // Connect the custom view click handler to action
    connect(puzzleView_, &ClickableView::mouseClickDetected, this, &MainWindow::handleViewClick);}

MainWindow::~MainWindow() {}

void MainWindow::handleViewClick(const QPoint &clickPos)
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

            // Switch the clicked puzzle piece with the empty slot
            QPoint old_empty(empty_tile_location_);
            empty_tile_location_.setX(x);
            empty_tile_location_.setY(y);
            clicked_item->setPos((old_empty.x() * IMAGE_MARGIN) + (old_empty.x() * sectionWidth_),
                                 (old_empty.y() * IMAGE_MARGIN) + (old_empty.y() * sectionHeight_));
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

            // Create QGraphicsPixmapItems from sections of puzzle image
            QImage subimage = originalImage.copy(x, y, sectionWidth_, sectionHeight_);
            QGraphicsPixmapItem* clickableItem = new QGraphicsPixmapItem(QPixmap::fromImage(subimage));

            // Add created subImages into grid formation with margins
            // resembling original image on scene_ container
            scene_->addItem(clickableItem);
            QPoint point((j * IMAGE_MARGIN) + (j * sectionWidth_), (i * IMAGE_MARGIN) + (i * sectionHeight_));
            clickableItem->setPos(point);
        }
    }
}

void MainWindow::display_puzzle()
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
