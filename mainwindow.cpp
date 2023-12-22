#include "mainwindow.hpp"
#include <QVBoxLayout>
#include <QDebug>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    empty_tile_location_(0, 0)
{
    // Set the central widget
    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // Create a GraphicsView and Scene for displaying puzzle image parts
    scene_ = new QGraphicsScene(this);
    puzzleView_ = new QGraphicsView(scene_);
    puzzleView_->setGeometry(0, 0, 0, 0);

    // Add GraphicsView to main layout
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(puzzleView_);

    // Create the puzzle display from user image
    display_puzzle();
}

MainWindow::~MainWindow() {}

void MainWindow::divideImage(const QImage &originalImage, int rows, int columns,
                             QVector<QVector<QGraphicsPixmapItem*>> &subimages) {
    // Get the size for subimages
    int sectionWidth = originalImage.width() / columns;
    int sectionHeight = originalImage.height() / rows;

    // Set correct vector size
    subimages.resize(rows);

    for (int i = 0; i < rows; ++i) {
        subimages[i].resize(columns);

        for (int j = 0; j < columns; ++j) {
            int x = j * sectionWidth;
            int y = i * sectionHeight;

            // Create QGraphicsPixmapItems from sections of puzzle image
            QImage subimage = originalImage.copy(x, y, sectionWidth, sectionHeight);
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(subimage));

            // Add created subImages into grid formation with margins
            // resembling original image on scene_ container
            scene_->addItem(item);
            QPoint point((j * IMAGE_MARGIN) + (j * sectionWidth), (i * IMAGE_MARGIN) + (i * sectionHeight));
            item->setPos(point);
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
