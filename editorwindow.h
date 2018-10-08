#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QLabel>


namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);

    ~EditorWindow();

    void keyPressEvent(QKeyEvent *event);

    void openFile();

    void saveFile(QString file_name);

    void clearFile();

private slots:
    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void setStatusBar(bool);

    void setStats(int, int);

    void findStr(QString);


    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_pb_forwardSearch_clicked();

    void on_pb_backwardSearch_clicked();


private:
    Ui::EditorWindow *ui;
    QString ew_filename;
    unsigned chars_count;
    unsigned words_count;
    QLabel l_chars;
    QLabel l_words;
};

#endif // EDITORWINDOW_H
