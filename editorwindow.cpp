#include "editorwindow.h"
#include "ui_editorwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include "codeeditor.h"

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::EditorWindow),
    chars_count(0), words_count(0)
{
    ui->setupUi(this);
    setStatusBar(true);

    connect(ui->widgetEditor, SIGNAL(statsCounted(int,int)), this, SLOT(setStats(int,int)));
    connect(ui->actionShow_statistics, SIGNAL(triggered(bool)), this, SLOT(setStatusBar(bool)));

    connect(ui->le_find, SIGNAL(textChanged(QString)), this, SLOT(findStr(QString)));

    connect(ui->widgetEditor, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(ui->widgetEditor, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Find))
    {
        ui->le_find->setFocus();
    }
}


void EditorWindow::setStatusBar(bool visible)
{
    if(visible)
    {
        statusBar()->setVisible(true);
        l_chars.setText("chars: " + QString::number(chars_count));
        l_words.setText("words: " + QString::number(words_count));
        statusBar()->addPermanentWidget(&l_words);
        statusBar()->addPermanentWidget(&l_chars);
    }
    else
        statusBar()->setVisible(false);
}


void EditorWindow::setStats(int chars, int words)
{
    chars_count = chars;
    words_count = words;

    l_chars.setText("chars: " + QString::number(chars_count));
    l_words.setText("words: " + QString::number(words_count));
}

void EditorWindow::findStr(QString to_find)
{

    QTextCursor tc = ui->widgetEditor->textCursor();
    tc.setPosition(0);
    ui->widgetEditor->setTextCursor(tc);

    ui->widgetEditor->find(to_find);
}


void EditorWindow::openFile()
{
    try
    {
        QFile file(ew_filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            throw(file.errorString());
        }
        else
        {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            ui->widgetEditor->setPlainText(in.readAll());

            file.close();
        }
    }
    catch(QString qstr)
    {
        QMessageBox::warning(0, "Error", qstr);
    }
}

void EditorWindow::saveFile(QString file_name)
{
    try
    {
        QFile file(file_name);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            throw(file.errorString());
        }
        else
        {
            QTextStream out(&file);
            out << ui->widgetEditor->toPlainText();

            file.flush();
            file.close();
        }
    }
    catch(QString qstr)
    {
        QMessageBox::warning(0, "Error", qstr);
    }
}

void EditorWindow::clearFile()
{
    ui->widgetEditor->setPlainText("");
}

void EditorWindow::on_actionCopy_triggered()
{
    ui->widgetEditor->copy();
}

void EditorWindow::on_actionPaste_triggered()
{
    ui->widgetEditor->paste();
}

void EditorWindow::on_actionCut_triggered()
{
    ui->widgetEditor->cut();
}

void EditorWindow::on_actionUndo_triggered()
{
    ui->widgetEditor->undo();
}

void EditorWindow::on_actionRedo_triggered()
{
    ui->widgetEditor->redo();
}

void EditorWindow::on_actionOpen_triggered()
{
    try
    {
        ew_filename = QFileDialog::getOpenFileName(this, tr("Open Document"), QDir::currentPath(),
        tr("JSON files (*.json);;Text files (*.txt)"));
        if(ew_filename == NULL)
            throw "You didn't chose file to open.";
        openFile();
    }
    catch(const char* str)
    {
        QString qstr = QString::fromUtf8(str);
        QMessageBox::warning(0, "Error", qstr);
    }
}

void EditorWindow::on_actionNew_triggered()
{
    clearFile();
}

void EditorWindow::on_actionSave_triggered()
{
    if(ew_filename != NULL)
    {
        saveFile(ew_filename);
    }
    else
    {
        on_actionSave_As_triggered();
    }
}

void EditorWindow::on_actionSave_As_triggered()
{
    try
    {
        ew_filename = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath(),
                                                        tr("JSON files (*.json);;Text files (*.txt)"));
        if(ew_filename == NULL)
        {
            throw "You didn't chose file to save.";
        }
        else
        {
            saveFile(ew_filename);
        }
    }
    catch(const char* str)
    {
        QString qstr = QString::fromUtf8(str);
        QMessageBox::warning(0, "Error", qstr);
    }

}

void EditorWindow::on_pb_forwardSearch_clicked()
{
    ui->widgetEditor->find(ui->le_find->text());
}

void EditorWindow::on_pb_backwardSearch_clicked()
{
    ui->widgetEditor->find(ui->le_find->text(), QTextDocument::FindBackward);
}


