/**
 * Author: Kalle Lahtinen
 *
 * A sliding puzzle game. Game details and instructions are in Instructions.txt file
*/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include "clickableview.hpp"

const int WINDOW_MARGIN = 5;          // Space around puzzle
const int IMAGE_MARGIN = 3;           // Space between puzzle parts
const QString IMAGEPATH = "owl.jpeg"; // Puzzle image location

// Puzzle "grid" dimensions
const int IMAGE_COLUMNS = 4;
const int IMAGE_ROWS = 6;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @brief handleViewClick receives ClickableView::mouseClickDetected signal.
     *        Switches clicked QGraphicsPixmapItem with empty slot if one was clicked
     * @param clickPos is the clicked position of the view
     */
    void handleViewClick(const QPoint &clickPos);

private:
    ClickableView* puzzleView_; // puzzleView_ and
    QGraphicsScene* scene_;     // scene_ are used for displaying the puzzle

    // The location of the one empty puzzle tile
    QPoint empty_tile_location_;

    int sectionWidth_;          // Size of
    int sectionHeight_;         // puzzle piece

    /**
     * @brief divideImage creates subimage items from user image
     *        and adds displays them on scene_
     * @param originalImage is the puzzle image
     * @param rows is the number of puzzle rows
     * @param columns is the number of puzzle columns
     * @param subImages is a nested vector containing the created puzzle (sub)images
     */
    void divideImage(const QImage &originalImage, int rows, int columns,
                     QVector<QVector<QGraphicsPixmapItem*>> &subImages);

    /**
     * @brief display_puzzle converts the puzzle image to the correct aspect ratio
     *        and creates the puzzle sections using divideImage()
     */
    void display_puzzle();
};
#endif // MAINWINDOW_HPP
